
#include "dir.h"

#define FILENAME_BYTES     18
#define RESERVED_BYTES     7
#define FIRST_BLOCK_BYTES  2
#define FILESIZE_BYTES     4

Dir::Dir():m_filename(0), m_attributes(0x00), m_reserved(0), m_first_block(0), m_filesize(0){

  m_filename = new unsigned char[FILENAME_BYTES];
  //m_attributes
  m_reserved = new unsigned char[RESERVED_BYTES];
  m_first_block = new unsigned char[FIRST_BLOCK_BYTES];
  m_filesize = new unsigned char[FILESIZE_BYTES];

}

Dir::~Dir(){
  delete []m_filename;
  delete []m_reserved;
  delete []m_first_block;
  delete []m_filesize;
}

