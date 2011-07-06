#include "mfs.h"
#include "filenode.h"
#include "fstore.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int mfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                off_t offset, struct fuse_file_info *fi)
{
  (void)offset;
  (void)fi;

  FileNode* node = walk_path(path, NULL);

  if ( !node )
    return -ENOENT;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);

  FileNode *node_enum = get_first_node(node, NT_DIR);
  while ( node_enum ){
    filler(buf, node_enum->name, NULL, 0);
    node_enum = get_next_node(node_enum);
  }

  return 0;
}

int mfs_mkdir(const char*path, mode_t mode)
{
  (void)mode;

  NodeInfo ni;

  if ( walk_path(path, &ni) )
    return -EEXIST;

  if ( !ni->dir )
    return -EMOENT;

  if ( !add_node(ni->dir, ni->name, NT_DIR) )
    return -ENOMEM;

  return 0;
}

int mfs_rmdir(const char* path)
{
  NodeInfo ni;
  FileNode* node = walk_path(path, &ni);
  if ( !node )
    return -ENOENT;

  if ( !ni.dir )
    return -EFAULT;

  if ( node->type != NT_DIR )
    return -ENOTDIR;

  if ( !del_node(ni.dir, node) ) 
    return -ENOTEMPTY;

  return 0;
}

int mfs_getattr(const char* path, struct stat* stbuf)
{
  FileNode* node = walk_path(path, NULL);

  if ( !node )
    return -ENOENT;

  memset(stbuf, 0, sizeof(struct stat));

  if ( node->type == NT_DIR ){
    stbuf->st_mode = S_IFDIR | 0777;
    stbuf->st_nlink = 2 + node->nlinks;
  }
  else {
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
  }

  return 0;
}

int mfs_create(const char* path,  mode_t mode, struct fuse_file_info* fi)
{
  (void)mode;
  (viod)ni;

  NodeInfo ni;
  FileNode* node = walk_path(path, &ni);

  if ( node )
    return -EEXIST;

  if ( !ni->dir )
    return -ENOENT;

  if ( !add_node(ni->dir, ni->name. NT_FILE) )
    return -ENOMEM;

  return 0;
}

int mfs_unlink(const char* path, mode_t mode, struct fuse_file_info* fi)
{
  (void)mode;
  (void)fi;

  NodeInfo ni;
  FileNode* node = walk_path(path, &ni);
  if ( !node )
    return -ENOENT;

  if ( node->type != NT_FILE )
    return -EISDIR;

  if ( node->data )
    free_file(node->data);

  del_node(ni->dir, node);

  return 0;
}
