#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "filesystem.h"
#include "utils.h"
#include "fsexcept.h"

#define BOOT_OFFSET       0*1024
#define FAT_OFFSET        1*1024
#define ROOTDIR_OFFSET    9*1024
#define DATA_OFFSET       10*1024

FileSystem::FileSystem(const std::string &partfname):part_filename(partfname), initialized(false){
  memset(fat, 0x00, sizeof(fat));
}

FileSystem::~FileSystem(){
  dumpfat();
}

void FileSystem::debug(){

  ::debug("Debugging the filesystem class...");
  if (this->init() != RET_OK){
    ::debug("Init failed.");
  }
  if (this->load() != RET_OK){
    ::debug("Load failed.");
  }
  this->makedir("/home");
  //this->makedir("/home/box");

}

int FileSystem::init(){

  remove(part_filename.c_str());
  createdummy();

  // step 1: writeout 1024 0xbb's
  unsigned char bootblock[1024];
  memset(bootblock, 0xbb, sizeof(bootblock));
  writeblock(bootblock, 0);

  // step 2: writeout the fat "header"
  unsigned char fatheader[8192] = {0xff, 0xfd,
                                   0xff, 0xfe,
                                   0xff, 0xfe,
                                   0xff, 0xfe,
                                   0xff, 0xfe,
                                   0xff, 0xfe,
                                   0xff, 0xfe,
                                   0xff, 0xfe,
                                   0xff, 0xfe,
                                   0xff, 0xff};

  memset(fatheader+20, 0x00, sizeof(fatheader)-20);
  for (unsigned int i=0; i<8; i++){
    writeblock(&(fatheader[i*1024]), (i+1)*1024);
  }

  // step 3: we no longer have to writeout the blank rest because we now have a whole blankened dummy file from the beginning

  return RET_OK;
}

int FileSystem::load(){

  for (unsigned int i=0; i<8; i++){
    readblock(&(fat[i*1024]), (i+1) * 1024);
  }

  initialized = true;
  return RET_OK;

}

int FileSystem::makedir(const std::string &path){
  CHECK_INIT 

  // mvdebug begin
  // so pra poder recompilar
  //dir_entry_t rootdir[32];
  //readblock(rootdir, ROOTDIR_OFFSET);
  // mvdebug end

  /*try {
    parent = findparent(path);
  } catch (const FSExcept &ex) {
    if (ex.code == RET_NO_SUCH_PARENT){
      // no such parent. error. this aint no mkdir -p
      ::debug("Asked to makedir but cannot find parent");
      return RET_NO_SUCH_PARENT;
    }
  }*/

  // lets check if this is a dupe
  /*if (has_in_rootdir(sep_path[0])){
    return RET_DIR_ALREADY_EXISTS; 
  }*/

  // its not a dupe. lets see if we can find a slot for it 
  /*int const rid = find_free_rootdir();
  if (rid == -1){
    // no space. forget it.
    return RET_ROOTDIR_FULL;
  }*/

  const std::string new_dir_name = utils_basename(path);
  dir_entry_t new_dir_struct;

  // theres space. lets create it. first, find a slot in the fat
  int fid = find_free_fat();
  if (fid == -1){
    return RET_FS_FULL;
  }

  // theres space, and fat also has space. add it
  fmt_ushort_into_uchar8pair(&(fat[fid]), 0xffff);
  fmt_char8_into_uchar8(new_dir_struct.filename, new_dir_name.c_str());
  new_dir_struct.attributes = 1;
  fmt_ushort_into_uchar8pair(new_dir_struct.first_block, fid);
  fmt_uint_into_uchar8quad(new_dir_struct.size, 0);

  //mvtodo: dumprootdir();
  dumpfat();
  return RET_OK;

}

int FileSystem::listdir(const std::string &path, std::vector<std::string> &result){
  CHECK_INIT 

  (void)path;
  (void)result;
  return -1;
}

int FileSystem::createfile(const std::string &path){
  CHECK_INIT 
  (void)path;
  return -1;
}

int FileSystem::unlink(const std::string &path){
  CHECK_INIT 
  (void)path;
  return -1;
}

int FileSystem::write(const std::string &path, const std::string &content){
  CHECK_INIT 
  (void)path;
  (void)content;
  return -1;
}

int FileSystem::append(const std::string &path, const std::string &content){
  CHECK_INIT 
  (void)path;
  (void)content;
  return -1;
}

int FileSystem::read(const std::string &path, std::string &content){
  CHECK_INIT 

  (void)path;
  (void)content;
  return -1;
}

void FileSystem::readblock(void *into, const unsigned int offset) const {

  FILE* fd = fopen(part_filename.c_str(), "rb");
  if (fd == NULL){
    throw FSExcept("Cannot open for reading", RET_INTERNAL_ERROR);
  }
  fseek(fd, offset, SEEK_SET);
  fread(into, 1, 1024, fd);
  fclose(fd);

}

void FileSystem::writeblock(const void * buf, const unsigned int offset){

  // we have to read and overwrite the whole file :S 
  unsigned char *wholebuffer = 0;
  long fsize = 0;

  // open up partition file
  FILE* fd_read = fopen(part_filename.c_str(), "rb");
  if (fd_read == NULL){
    throw FSExcept("Cannot open for reading.", RET_INTERNAL_ERROR);
  }

  // get its filesize
  fseek(fd_read, 0L, SEEK_END);
  fsize = ftell(fd_read);
  //fseek(fd_read, 0L, SEEK_SET);
  rewind(fd_read);

  // allocate whole buffer (to store the whole file, yes!) and read into it
  wholebuffer = (unsigned char*)calloc(fsize, 1);
  fread(wholebuffer, 1, fsize, fd_read);
  fclose(fd_read);

  // change wholebuff contents with what this function was given
  memcpy(wholebuffer+offset, buf, 1024);

  // and finally write the whole thing back
  FILE* fd_write = fopen(part_filename.c_str(), "wb");
  if (fd_write == NULL){
    throw FSExcept("Cannot open for writing.", RET_INTERNAL_ERROR);
  }

  // write, close, dealloc.. clenaup code.
  fwrite(wholebuffer, 1, fsize, fd_write);
  fclose(fd_write);
  free(wholebuffer);

}

void FileSystem::createdummy() const {

  FILE* fd_write = fopen(part_filename.c_str(), "wb+");
  // mvtodo: fopen error treating

  // MV: for some reason, using more than a handful of K's on the stack was causing a segfault.
  // took hours to figure it out, but theres a really DUMB solution down below
  const unsigned long total = 4*1024*1024; // 4 mbs
  const unsigned int piece = total / 16;

  unsigned char dummybuf[piece];
  memset(dummybuf, 0x00, sizeof(dummybuf));
  for (unsigned int i=0; i<total/piece; i++){
    fwrite(dummybuf, 1, sizeof(dummybuf), fd_write);
  }

  fclose(fd_write);

}

void FileSystem::dumpfat() {
  for (unsigned int i=0; i<8; i++){
    writeblock(&(fat[i*1024]), (i+1)*1024);
  }
}

/*bool FileSystem::has_in_rootdir(const std::string &dir) const {

  for (unsigned int i=0; i<32; i++){
    if (rootdir[i].filename[0] != 0x00){
      std::string rd_str = fmt_ascii7_to_stdstr(rootdir[i].filename);
      if (rd_str == dir){
        return true;
      }
    }
  }

  return false;
}

int find_free_in_rootdir(const dir_entry_t *) const;
int FileSystem::find_free_rootdir() const {
  for (unsigned int i=0; i<32; i++){
    if (rootdir[i].filename[0] == 0x00){
      return i;
    }
  }
  return -1;
}*/

int FileSystem::find_free_fat() const {
  for (unsigned int i=0; i<sizeof(fat)/2; i+=2){
    if (fat[i] == 0x00){
      return i;
    }
  }
  return -1;
}

dir_entry_t FileSystem::findparent(const std::string &path){
  std::vector<std::string> sep_path = tokenize_path(path);

  //dir_entry_t *parent = rootdir;
  dir_entry_t *parent = 0;
  for (unsigned int i = 0; i < sep_path.size(); i++){
    if (i == sep_path.size()-1) {
      break;
    }

    

  }

  if (parent == 0){
    throw FSExcept("Parent cannot be found", RET_NO_SUCH_PARENT);
  }

  return dir_entry_t(*parent);
}

dir_entry_t FileSystem::findentry(const std::string &path_basename, const dir_entry_t &parent){

}

