
#include "block.h"

#define BLOCK_BYTES  1024

Block::Block():m_data(0){
  m_data = new unsigned char[BLOCK_BYTES];
}

Block::~Block(){
  delete []m_data;
}

