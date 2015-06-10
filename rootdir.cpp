
#include "rootdir.h"

#define DIR_ENTRIES 32

RootDir::RootDir():m_dirs(0){
  m_dirs = new Dir[DIR_ENTRIES];
}

RootDir::~RootDir(){
  delete []m_dirs;
}

