#ifndef MEMBUF_H
#define MEMBUF_H

typedef struct{
  char* data;
  size_t size;
  size_t len;
  size_t step;
} MemBuf;

MemBuf* create_mbuf(size_t grow);
void free_mbuf(MemBuf* vec);
int append_mbuf(MemBuf* vec, char* src, size_t len);
size_t copy_mbuf(MemBuf* vec, char* dst, size_t len, size_t offset);

#endif
