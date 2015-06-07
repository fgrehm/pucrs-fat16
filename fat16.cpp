
#include "fat16.h"

#define FAT16_ENTRIES 4096

Fat16::Fat16():m_fat16(0){
  m_fat16 = new unsigned short[FAT16_ENTRIES];
}

Fat16::~Fat16(){
  delete []m_fat16;
}

