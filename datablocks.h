
#ifndef _DATABLOCKS_H_
#define _DATABLOCKS_H_

#include "block.h"

class DataBlocks {

  public:

    DataBlocks();
    ~DataBlocks();

  private:
    DataBlocks(const DataBlocks&);
    DataBlocks operator=(const DataBlocks&);

    Block *m_datablocks;

};

#endif // _DATABLOCKS_H_

