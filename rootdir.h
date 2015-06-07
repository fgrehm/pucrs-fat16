
#ifndef _ROOTDIR_H_
#define _ROOTDIR_H_

#include "dir.h"

class RootDir {

  public:

    RootDir();
    ~RootDir();

  private:
    RootDir operator=(const RootDir&);
    RootDir(const RootDir&);

    Dir *m_dirs;

};

#endif // _ROOTDIR_H_

