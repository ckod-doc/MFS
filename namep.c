#include "namep.h"
#include <string.h>

static FileNode* root = NULL;

int init()
{
  return init_root(&root, NT_DIR);
}

int create_dir(const char* path)
{
  NodeInfo ni;
  FileNode* node = walk_path(path, FN_PREV, &ni);
  if ( !node ) return 0;

  return add_node(node, ni.name, NT_DIR);
}

int create_file(const char* path)
{
  NodeInfo ni;
  FileNode* node = walk_path(path, FN_PREV, &ni);
  if ( !node ) return 0;

  return add_node(node, ni.name, NT_FILE);
}

static int get_name(const char **path, char *buf, size_t len)
{
  size_t num = 0;
  const char *str = *path;

  while ( num < len ){
    if ( !*str || *str == '/' ){
      *buf = 0;
      *path = str;
      return 1;
    }
    *buf++ = *str++;
    ++num; 
  }

  return 0;
}

FileNode* walk_path(const char* path, int state, NodeInfo* nodeinf)
{
  FileNode* last_node = NULL;
  FileNode* prev_node = NULL;
  char buf[MAX_FILENAME];
  int name_type;

  if ( *path == '/' )
    last_node = root;
  else
    return NULL;

  while ( *path == '/' ) ++path;

  while ( *path ){
    name_type = NT_FILE;
    if ( !get_name(&path, buf, MAX_FILENAME) ) return NULL;
    if ( *path == '/' ){
      name_type = NT_DIR;
      while ( *path == '/' ) ++path;
    }

    prev_node = last_node;
    last_node = find_node(prev_node, buf);
    if ( !last_node ){
      if ( !*path && state == FN_PREV ){
	if ( nodeinf ){
	  strncpy(nodeinf->name, buf, MAX_FILENAME);
	  nodeinf->type = name_type;
	}
      } else 
	return NULL;
    }
  }

  if ( nodeinf ) nodeinf->dir = prev_node;
  
  return (state == FN_PREV)?prev_node:last_node;
}


