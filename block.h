
#ifndef _BLOCK_H_
#define _BLOCK_H_

class Block {

  public:

    Block();
    ~Block();

  private:
    Block(const Block&);
    Block operator=(const Block&);

    unsigned char *m_data;

};

#endif // _BLOCK_H_

