#include <stdlib.h>
#include <string.h>
#include "membuf.h"

MemBuf* create_mbuf(size_t grow)
{
  MemBuf *vec = malloc(sizeof(MemBuf));
  if ( vec ){
    vec->data = NULL;
    vec->size = vec->len = 0;
    vec->step = grow;
  }

  return vec;
}

void free_mbuf(MemBuf* vec)
{
  if ( vec->data ) free(vec->data);
  free(vec);
}

int append_mbuf(MemBuf* vec, char* src, size_t len)
{
   size_t available = vec->size - vec->len; 

  if ( len > available ){
    size_t sz = (len < vec->step)?vec->step:len;
    char* tmp_p = realloc(vec->data, vec->size + sz);
    if ( !tmp_p ) return 0;

    vec->data = tmp_p;
    vec->size += sz;
  }

  memcpy(vec->data + vec->len, src, len);
  vec->len += len;

  return 1;
}

size_t copy_mbuf(MemBuf* vec, char* dst, size_t len, size_t offset)
{
  if (len > offset) 
    return 0;

  size_t plen = vec->len - offset;
  size_t sc = (plen > len)?len:plen;

  memcpy(dst, vec->data+offset, sc);

  return sc;
}

