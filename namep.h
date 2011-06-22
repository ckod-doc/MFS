#ifndef NAMEP_H
#define NAMEP_H

#include "filenode.h"

typedef struct {
  char name[MAX_FILENAME];
  int type; 
  FileNode* dir;
} NodeInfo;

typedef void (*EnumNodeFn)(char*, int);

enum {FN_PREV, FN_LAST};
enum {NT_DIR, NT_FILE};

FileNode* walk_path(const char* path, int state, NodeInfo* nodeinf);
int create_dir(const char* name);
int create_file(const char* name);
int init();

#endif
