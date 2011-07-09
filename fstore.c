#include <stdlib.h>
#include <string.h>
#include "fstore.h"

static void decode_addr(off_t addr, off_t* base, size_t *offset)
{
  *base = addr >> SECTOR_SIZE_BITS;
  *offset = addr & (SECTOR_SIZE-1);
}

FileStore* alloc_file()
{
  FileStore* fs = malloc(sizeof(FileStore));
  if ( fs ){
    fs->head = NULL;
    fs->size = 0;
  }

  return fs;
}

void free_file(FileStore* fs)
{
  Sector* curr_sector;

  while ( fs->head ){
    curr_sector = fs->head;
    fs->head = fs->head->next;
    free(curr_sector);
  }

  free(fs);
}

static Sector* get_sector(FileStore* fs, Sector* begin_sector, off_t sector_key)
{
  Sector* curr_sector;
  Sector* prev_sector = NULL;

  if ( !begin_sector || begin_sector->key > sector_key ) 
    curr_sector = fs->head;
  else
    curr_sector = begin_sector;

  while ( curr_sector ){
    if ( curr_sector->key >= sector_key ){
      if ( curr_sector->key == sector_key ) 
	return curr_sector;

      break;
    }

    prev_sector = curr_sector;
    curr_sector = curr_sector->next;
  }

  if ( curr_sector && curr_sector->key == sector_key ) 
    return curr_sector;

  Sector* new_sector = malloc(sizeof(Sector));
  if ( new_sector ){
    new_sector->key = sector_key;
    new_sector->next = curr_sector;
    memset(new_sector->data, 0, SECTOR_SIZE);
    if ( prev_sector ) prev_sector->next = new_sector;
    else
      fs->head = new_sector;
  }
    
  return new_sector;
}

static Sector* find_max_or_eq(Sector* last_sector, off_t sector_key)
{
  Sector* curr_sector = last_sector;

  while ( curr_sector && curr_sector->key < sector_key )
    curr_sector = curr_sector->next;

  return curr_sector;
}

size_t write_file(FileStore* fs, const char* buf, size_t len, off_t offset)
{
  off_t num_sector;
  size_t  sector_pos;
  size_t src_len = len;
  size_t pos = 0;
 
  decode_addr(offset, &num_sector, &sector_pos);

  Sector* curr_sector = get_sector(fs, fs->head, num_sector);
  if ( curr_sector ){
    size_t fs_avail = SECTOR_SIZE - sector_pos;
    size_t nbyte_cpy;

    nbyte_cpy = (fs_avail < src_len)?fs_avail:src_len;
    memcpy(curr_sector->data + sector_pos, buf, nbyte_cpy);

    pos = nbyte_cpy;
    src_len -= nbyte_cpy;

    while ( (curr_sector = get_sector(fs, curr_sector, ++num_sector)) && src_len > 0 ){
      nbyte_cpy = (SECTOR_SIZE < src_len)?SECTOR_SIZE:src_len;
      memcpy(curr_sector->data, buf + pos, nbyte_cpy);
      pos += nbyte_cpy;
      src_len -= nbyte_cpy;
    }
  }

  off_t new_file_size = offset + pos;
  if ( new_file_size > fs->size ) fs->size = new_file_size;

  return pos;
}

size_t read_file(FileStore* fs, char* buf, size_t len, off_t offset)
{
  if ( offset >= (off_t)fs->size ) 
    return 0;

  off_t num_sector;
  size_t sector_pos;
  size_t pos = 0;
  size_t cpy_file_len = fs->size - offset;
  size_t src_len = (len < cpy_file_len)?len:cpy_file_len;
 

  decode_addr(offset, &num_sector, &sector_pos);

  size_t nbytes_cpy;
  Sector* curr_sector = fs->head;

  while ( curr_sector && src_len > 0 ){
    if ( curr_sector->key >= num_sector ){
      size_t fs_avail = SECTOR_SIZE - sector_pos;
      nbytes_cpy = (fs_avail < src_len)?fs_avail:src_len;

      if ( curr_sector->key == num_sector )
	memcpy(buf+pos, curr_sector->data + sector_pos, nbytes_cpy);
      else
	memset(buf+pos, 0, nbytes_cpy);

      pos += nbytes_cpy;
      src_len -= nbytes_cpy;
      sector_pos = 0;
      ++num_sector;
    }
    else
      curr_sector = find_max_or_eq(curr_sector, num_sector);
  }

  return pos;
}
