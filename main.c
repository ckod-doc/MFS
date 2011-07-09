#include "mfs.h"

static struct fuse_operations mfs_oper = {
    .getattr	= mfs_getattr,
    .readdir	= mfs_readdir,
    .mkdir      = mfs_mkdir,
    .rmdir      = mfs_rmdir,
    .create     = mfs_create,
    .unlink     = mfs_unlink,
    .read       = mfs_read,
    .write      = mfs_write
};

int main(int ac, char** av)
{
  if ( !init_fs() )
    return 1;

  return fuse_main(ac, av, &mfs_oper);
}
