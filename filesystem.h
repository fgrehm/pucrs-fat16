
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
/* Interface return codes end */

/* Data structures begin */

typedef struct {
  unsigned char filename[18];
  unsigned char attributes;
  unsigned char reserved[7];
  unsigned char first_block;
  unsigned char size;
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

    const std::string partfilename;
    unsigned short fat[4096];
    unsigned char datablocks[1024];
    dir_entry_t rootdir[32];

};

#endif // _FILESYSTEM_H_

