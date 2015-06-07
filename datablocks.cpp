
#include "datablocks.h"

#define BLOCKS_AMOUNT  4086

DataBlocks::DataBlocks():m_datablocks(0){
  m_datablocks = new Block[BLOCKS_AMOUNT];
}

DataBlocks::~DataBlocks(){
  delete []m_datablocks;
}

