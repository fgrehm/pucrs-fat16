
#include "filesystem.h"
#include "utils.h"

#include <iostream>

#include <stdio.h>
#include <string.h>

FileSystem::FileSystem(const std::string &partfname):part_filename(partfname){
}

FileSystem::~FileSystem(){
}

void FileSystem::debug(){

  std::cout << "Debugging the filesystem class..." << std::endl;
  this->init();
  //this->load();
  this->makedir("/home");

}

int FileSystem::init(){

  // mvtodo: remove fat.part before "formatting"

  // step 1: writeout 1024 0xbb's
  unsigned char bootblock[1024];
  memset(bootblock, 0xbb, sizeof(bootblock));
  if (!writeblock(bootblock, 0)){
    // mvtodo: tratar erro aqui
  }

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
    if (!writeblock(&(fatheader[i*1024]), (i+1)*1024)){
      // mvtodo: tratar erro aqui
    }
  }

  // step 3: writeout blank rest
  unsigned char blankrest[4185088];
  memset(blankrest, 0x00, sizeof(blankrest));
  for (unsigned int i=0; i<4087; i++){
    // mvtodo: howcome (i+1)*1024 passed the python script test??
    if (!writeblock(blankrest, (i+9)*1024)){
      // mvtodo: tratar erro aqui
    }
  } 

  return RET_OK;

}

int FileSystem::load(){

  for (unsigned int i=0; i<8; i++){

    if (!readblock(&(fat[i*1024]), (i+1) * 1024)){
      // TODO: Handle errors when file manipulation goes wrong
    }

  }

  return RET_OK;

}

int FileSystem::makedir(const std::string &path){

  std::vector<std::string> ret = tokenize_path(path);

  for (unsigned int i=0; i<ret.size(); i++){

    int const rid = find_free_rootdir();
    if (rid == -1){
      return RET_ROOTDIR_FULL;
    }
 
    if (i==0){ // root dir
    } else {
    }

  }

  return RET_OK;
}

int FileSystem::listdir(const std::string &path, std::vector<std::string> &result){
  (void)path;
  (void)result;
  return -1;
}

int FileSystem::createfile(const std::string &path){
  (void)path;
  return -1;
}

int FileSystem::unlink(const std::string &path){
  (void)path;
  return -1;
}

int FileSystem::write(const std::string &path, const std::string &content){
  (void)path;
  (void)content;
  return -1;
}

int FileSystem::append(const std::string &path, const std::string &content){
  (void)path;
  (void)content;
  return -1;
}

int FileSystem::read(const std::string &path, std::string &content){
  (void)path;
  (void)content;
  return -1;
}

int FileSystem::find_free_rootdir() const {



  for (unsigned int i=0; i<sizeof(rootdir); i++){
    if (rootdir[i].filename[0] == 0x00){
      return i;
    }
  }
  return -1;
}

bool FileSystem::readblock(void *into, const unsigned int offset) const {

  FILE* fd = fopen(part_filename.c_str(), "rb");
  // mvtodo: treat errors
  fseek(fd, offset, SEEK_SET);
  fread(into, 1, 1024, fd);
  fclose(fd);

  return true;
}

bool FileSystem::writeblock(void *buf, const unsigned int offset){

  FILE* fd = fopen(part_filename.c_str(), "ab");
  // mvtodo: treat errors
  fseek(fd, offset, SEEK_SET);
  fwrite(buf, 1, 1024, fd);
  fclose(fd);

  return true;

}

