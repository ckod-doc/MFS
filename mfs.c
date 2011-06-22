#include "mfs.h"
#include "filenode.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int mfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                off_t offset, struct fuse_file_info *fi)
{
  (void)offset;
  (void)fi;

  FileNode* node = walk_path(path, FN_LAST, NULL);

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

  if ( walk_path(path, FN_LAST, NULL) )
    return -EEXIST;

  if ( !create_dir(path) )
    return -ENOENT;

  return 0;
}

int mfs_rmdir(const char* path)
{
  NodeInfo ni;
  FileNode* node = walk_path(path, FN_LAST, &ni);
  if ( !node )
    return -ENOENT;

  if ( !ni.dir )
    return -EFAULT;

  if ( !del_node(ni.dir, node) ) 
    return -ENOTEMPTY;

  return 0;
}

int mfs_getattr(const char* path, struct stat* stbuf)
{
  FileNode* node = walk_path(path, FN_LAST, NULL);

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
