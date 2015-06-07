
#ifndef _FAT16_H_
#define _FAT16_H_

class Fat16 {

  public:

    Fat16();
    ~Fat16();

  private:

    Fat16(const Fat16&);
    Fat16 operator= (const Fat16&);
    unsigned short *m_fat16; 

};

#endif // _FAT16_H_

