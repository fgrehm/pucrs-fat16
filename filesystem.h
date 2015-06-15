
#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <vector>
#include <string>
#include <string.h>

/* Interface return codes begin */
#define RET_OK 0
#define RET_NONEXISTANT_PATH 1
#define RET_NONEXISTANT_FILE 2
#define RET_DIR_ALREADY_EXISTS 3
#define RET_FILE_ALREADY_EXISTS 4
#define RET_FS_FULL 5
#define RET_DIR_NOT_EMPTY 6
#define RET_CANNOT_INITIALIZE 7
#define RET_FAT_FULL 8
#define RET_NOT_INITIALIZED 9
#define RET_NO_SUCH_PARENT 10
#define RET_INTERNAL_ERROR 11
/* Interface return codes end */

#define CHECK_INIT if (!initialized) {return RET_NOT_INITIALIZED;}

/* Data structures begin */

struct dir_entry_t {

  dir_entry_t():attributes(0x00){
    clear();
  }

  dir_entry_t(const dir_entry_t &other):attributes(0x00){
    copyFrom(other);
  }

  dir_entry_t& operator=(const dir_entry_t &other){
    copyFrom(other);
    return *this;
  }

  void copyFrom(const dir_entry_t &other){
    memcpy(this->filename, other.filename, sizeof(this->filename));
    this->attributes = other.attributes;
    memcpy(this->reserved, other.reserved, sizeof(this->reserved));
    memcpy(this->first_block, other.first_block, sizeof(this->first_block));
    memcpy(this->size, other.size, sizeof(this->size));
  }

  void clear(){
    memset(filename, 0x00, sizeof(filename));
    memset(reserved, 0x00, sizeof(reserved));
    memset(first_block, 0x00, sizeof(first_block));
    memset(size, 0x00, sizeof(size));
  }

  ~dir_entry_t(){}

  unsigned char filename[18];
  unsigned char attributes;
  unsigned char reserved[7];
  unsigned char first_block[2];
  unsigned char size[4];

};

/* Data structures end */

class FileSystem {

  public:

    FileSystem(const std::string &);
    ~FileSystem();

    void debug(); // for debugging purposes only

    /* Filesystem interface begin */
    int init(); // resets the virtual FS and writes it to the real disk
    int load(); // loads the virtual FS from the real disk

    int makedir(const std::string &); // makes a new dir on the given path
    int listdir(const std::string &, std::vector<std::string> &); // reads the contents of a path
    int createfile(const std::string &); // creates a file on the given path
    int unlink(const std::string &); // deletes file or dir on the given path

    int write(const std::string &, const std::string &); // writes string to the given file
    int append(const std::string &, const std::string &); // appends string to the given file
    int read(const std::string &, std::string &); // reads the given file onto the given string
    /* Filesystem interface end */

  private:

    int find_free_fat() const;
    int find_free_in_dir(const dir_entry_t *) const;
    int find_match_in_dir(const std::string &name, const dir_entry_t *dir) const;
    bool has_in_dir(const std::string &name, const dir_entry_t *dir) const;

    int traverse_path(const std::string&, const unsigned short);
    void follow_fat_erase(const unsigned char *);
    void lay_file_contents(const unsigned short, const std::string &);
    std::string get_file_contents(const unsigned short, const unsigned int);
    void dumpfat();
    void dumprootdir();

    void readblock(void*, const unsigned int) const;
    void writeblock(const void*, const unsigned int);
    void createdummy() const;

    const std::string part_filename;
    unsigned char fat[8192];

    bool initialized;

};

#endif // _FILESYSTEM_H_

