#include "namep.h"
#include <string.h>

static FileNode* root = NULL;

int init()
{
  return init_root(&root, NT_DIR);
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

FileNode* walk_path(const char* path, NodeInfo* nodeinf)
{
  FileNode* last_node = NULL;
  FileNode* prev_node = NULL;
  char buf[MAX_FILENAME];
  int name_type;

  if ( *path == '/' ){
    last_node = root;

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
	if ( *path  ){
	  prev_node = NULL;
	  break;
	} 
	else if ( nodeinf ){
	  strncpy(nodeinf->name, buf, MAX_FILENAME);
	  nodeinf->type = name_type;
	}
      } // if ( !last_node )
    } // while ( *path )
  } // if ( *path == '/' )

  if ( nodeinf ) nodeinf->dir = prev_node;
  
  return last_node;
}


