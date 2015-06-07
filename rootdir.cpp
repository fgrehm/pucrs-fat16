
#include "rootdir.h"

RootDir::RootDir():m_dirs(0){
  m_dirs = new Dir[32];
}

RootDir::~RootDir(){
  delete []m_dirs;
}

