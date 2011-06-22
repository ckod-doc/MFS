#include "filenode.h"
#include <stdlib.h>
#include <string.h>

int init_root(FileNode **root_node, int type)
{
  FileNode* node = malloc(sizeof(FileNode));
  if ( !node ) return 0;  

  strncpy(node->name, "/", MAX_FILENAME);
  node->next = NULL;
  node->prev = NULL;
  node->files = NULL;
  node->data = NULL;
  node->type = type;

  *root_node = node;

  return 1;
}

int del_node(FileNode* dir, FileNode* node)
{
  if ( node->files ) return 0;

  if ( node->prev ) node->prev->next = node->next;
  else dir->files = node->next;
  if ( node->next ) node->next->prev = node->prev;

  if ( node->type == dir->type ) dir->nlinks--;

  free(node);

  return 1;
}

int add_node(FileNode* dir, const char* name, int type)
{
  FileNode* node = malloc(sizeof(FileNode));
  if ( !node ) return 0;

  node->next = NULL;
  node->prev= NULL;
  node->files = NULL;
  node->data =NULL;
  node->nlinks = 0;
  strncpy(node->name, name, MAX_FILENAME);
  node->type = type;

  if ( dir->files ){
    node->next = dir->files;
    dir->files->prev = node;
    dir->files = node;
  }
  else
    dir->files = node;

  dir->nlinks++;

  return 1;
}

FileNode* find_node(FileNode* dir, const char* name)
{
  FileNode* node = dir->files;

  while ( node ){
    if ( strcmp(name, node->name) == 0 ) break;
    node = node->next;
  }

  return node;
}

FileNode* get_first_node(FileNode* dir, int type)
{
  if ( dir && dir->type == type) 
    return dir->files;
  
  return NULL;
}

FileNode* get_next_node(FileNode* node)
{
  if ( node ) 
    return node->next;

  return NULL;
}


