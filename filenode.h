#ifndef FILENODE_H
#define FILENODE_H

#define MAX_FILENAME 32

typedef struct ListNode {
  char name[MAX_FILENAME];
  int type;
  int nlinks;

  void* data;
  struct ListNode *files;

  struct ListNode* next;
  struct ListNode* prev;
} FileNode;


FileNode* get_first_node(FileNode* dir, int type);
FileNode* get_next_node(FileNode* node);
int del_node(FileNode* dir, FileNode* node);
int add_node(FileNode* dir, const char* name, int type);
FileNode* find_node(FileNode* dir, const char* name);
int init_root(FileNode **root_node, int type);

#endif
