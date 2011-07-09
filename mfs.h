#ifndef MFS_H
#define MFS_H

#include <fuse.h>
#include <sys/types.h>
#include "namep.h"

int mfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                off_t offset, struct fuse_file_info *fi);
int mfs_mkdir(const char*path, mode_t mode);
int mfs_rmdir(const char* path);
int mfs_getattr(const char* path, struct stat* stbuf);
int mfs_create(const char* path,  mode_t mode, struct fuse_file_info* fi);
int mfs_unlink(const char* path);

#endif
