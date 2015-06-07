
#include "fat16.h"

#define FAT16_ENTRIES 4096

#define FREE_ENTRY "\x00\x00"
#define BOOT_ENTRY "\xff\xfd"
#define FAT_ENTRY  "\xff\xfe"
#define EOF_ENTRY  "\xff\xff"

Fat16::Fat16():m_fat16(0){
  m_fat16 = new unsigned short[FAT16_ENTRIES];
}

Fat16::~Fat16(){
  delete []m_fat16;
}

