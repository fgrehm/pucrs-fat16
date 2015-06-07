
#include "fat16.h"

#define FAT16_ENTRIES 4096

#define FREE_ENTRY 0x0000
#define BOOT_ENTRY 0xfffd
#define FAT_ENTRY  0xfffe
#define EOF_ENTRY  0xffff

Fat16::Fat16():m_fat16(0){
  m_fat16 = new unsigned short[FAT16_ENTRIES];
}

Fat16::~Fat16(){
  delete []m_fat16;
}

