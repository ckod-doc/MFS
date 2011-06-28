#ifndef FSTORE_H
#define FSTORE_H

#include <sys/types.h>

#define SECTOR_SIZE 512
#define SECTOR_SIZE_BITS 9

typedef struct SectorNode {
  size_t key;
  char data[SECTOR_SIZE];
  struct SectorNode *next;
} Sector;

typedef struct {
  size_t size;
  Sector* head;
}FileStore;

FileStore* alloc_file();
void free_file(FileStore* fs);
size_t write_file(FileStore* fs, char* buf, size_t len, off_t offset);
size_t read_file(FileStore* fs, char* buf, size_t len, off_t offset);

#endif
