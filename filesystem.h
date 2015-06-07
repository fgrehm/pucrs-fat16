
#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include "fat16.h"
#include "rootdir.h"
#include "datablocks.h"

#include <vector>
#include <string>

#define RET_OK 0
#define RET_NONEXISTANT_PATH 1
#define RET_NONEXISTANT_FILE 2
#define RET_DIR_ALREADY_EXISTS 3
#define RET_FILE_ALREADY_EXISTS 4
#define RET_FS_FULL 5
#define RET_DIR_NOT_EMPTY 6

class FileSystem {

  public:

    FileSystem();
    ~FileSystem();

    /* Filesystem interface begin */
    int init(); // resets the virtual FS and writes it to the real disk
    int load(); // loads the virtual FS from the real disk

    int listdir(const std::string &, std::vector<std::string> &); // reads the contents of a path
    int makedir(const std::string &); // makes a new dir on the given path
    int createfile(const std::string &); // creates a file on the given path
    int unlink(const std::string &); // deletes file or dir on the given path

    int write(const std::string &, const std::string &); // writes string to the given file
    int append(const std::string &, const std::string &); // appends string to the given file
    int read(const std::string &, std::string &); // reads the given file onto the given string
    /* Filesystem interface end */

  private:

};

#endif // _FILESYSTEM_H_

