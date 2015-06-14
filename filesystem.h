
#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <vector>
#include <string>

/* Interface return codes begin */
#define RET_OK 0
#define RET_NONEXISTANT_PATH 1
#define RET_NONEXISTANT_FILE 2
#define RET_DIR_ALREADY_EXISTS 3
#define RET_FILE_ALREADY_EXISTS 4
#define RET_FS_FULL 5
#define RET_DIR_NOT_EMPTY 6
#define RET_CANNOT_INITIALIZE 7
#define RET_ROOTDIR_FULL 8
#define RET_NOT_INITIALIZED 9
/* Interface return codes end */

#define CHECK_INIT if (!initialized) {return RET_NOT_INITIALIZED;}

/* Data structures begin */

typedef struct {
  unsigned char filename[18];
  unsigned char attributes;
  unsigned char reserved[7];
  unsigned char first_block[2];
  unsigned char size[4];
} dir_entry_t;

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

    // auxiliary operations
    int find_free_rootdir() const;
    int find_free_fat() const;
    bool has_in_rootdir(const std::string&) const;

    void dumpfat();
    void dumprootdir();
    bool readblock(void*, const unsigned int) const;
    bool writeblock(const void*, const unsigned int);
    void createdummy() const;

    const std::string part_filename;
    unsigned char fat[8192];
    dir_entry_t rootdir[32];

    bool initialized;

};

#endif // _FILESYSTEM_H_

