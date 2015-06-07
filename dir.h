
#ifndef _DIR_H_
#define _DIR_H_

class Dir {
  public:
    Dir();
    ~Dir();
  private:
    Dir(const Dir&);
    Dir operator= (const Dir&);

    unsigned char *m_filename;
    unsigned char m_attributes;
    unsigned char *m_reserved;
    unsigned char *m_first_block;
    unsigned char *m_filesize;

};

#endif // _DIR_H_

