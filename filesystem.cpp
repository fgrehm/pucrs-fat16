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

  const std::string new_dir_name = utils_basename(path);
  dir_entry_t new_dir_struct;

  unsigned short cluster_offset = traverse_path(path, ROOTDIR_OFFSET);
  dir_entry_t dir_cluster[32];
  readblock(dir_cluster, cluster_offset);
 
  if (has_in_dir(new_dir_name, dir_cluster)){
    return RET_DIR_ALREADY_EXISTS;
  }

  unsigned short d_idx = find_free_in_dir(dir_cluster);

  //fmt_ushort_into_uchar8pair(&(fat[fid]), 0xffff);
  fmt_char8_into_uchar8(new_dir_struct.filename, new_dir_name.c_str());
  new_dir_struct.attributes = 1;
  //fmt_ushort_into_uchar8pair(new_dir_struct.first_block, fid);
  fmt_uint_into_uchar8quad(new_dir_struct.size, 1024);

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

bool FileSystem::has_in_dir(const std::string &name, const dir_entry_t *dir) const {

  for (unsigned int i=0; i<32; i++){
    if (dir[i].filename[0] != 0x00){
      std::string rd_str = fmt_ascii7_to_stdstr(dir[i].filename);
      if (rd_str == name){
        return true;
      }
    }
  }

  return false;
}

int FileSystem::find_free_in_dir(const dir_entry_t *dir) const{
  for (unsigned int i=0; i<32; i++){
    if (dir[i].filename[0] == 0x00){
      return i;
    }
  }
  return -1;
}

int FileSystem::find_free_fat() const {
  for (unsigned int i=0; i<sizeof(fat)/2; i+=2){
    if (fat[i] == 0x00){
      return i;
    }
  }
  return -1;
}

// percorre um path e devolve o cluster que esta o ultimo elemento.
unsigned short FileSystem::traverse_path(const std::string &path, const unsigned short offset){

  unsigned short ret = -1;
  std::vector<std::string> path_sep = tokenize_path(path);
  std::string next = popleft_path(path);

  if (path_sep.size() == 1){
    return offset;
  }

  /*dir_entry_t current[32];
  readblock(current, offset);*/

  for (unsigned int i=0; i<path_sep.size(); i++){
  }

  // writeblock(...)?

  return ret;

}

