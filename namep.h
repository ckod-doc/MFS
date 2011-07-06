#ifndef NAMEP_H
#define NAMEP_H

#include "filenode.h"

typedef struct {
  char name[MAX_FILENAME];
  int type; 
  FileNode* dir;
} NodeInfo;

typedef void (*EnumNodeFn)(char*, int);

enum {NT_DIR, NT_FILE};

FileNode* walk_path(const char* path, NodeInfo* nodeinf);
int init();

#endif
