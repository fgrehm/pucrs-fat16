
#include "filesystem.h"

#include <iostream>

#include <stdio.h>
#include <string.h>

FileSystem::FileSystem(const std::string &partfname):m_partfilename(partfname), m_fat(), m_rootdir(), m_datablocks(){
}

FileSystem::~FileSystem(){
}

void FileSystem::debug(){

  std::cout << "Debugging the filesystem class..." << std::endl;
  this->init();

}

int FileSystem::init(){

  FILE* fd = fopen(m_partfilename.c_str(), "wb+");

  // step 1: writeout 1024 0xbb's
  unsigned char bootblock[1024];
  memset(bootblock, 0xbb, sizeof(bootblock));
  fwrite(bootblock, 1, sizeof(bootblock), fd);

  // step 2: writeout the fat "header"
  unsigned char fatheader[20] = {0xff, 0xfd,
                                 0xff, 0xfe,
                                 0xff, 0xfe,
                                 0xff, 0xfe,
                                 0xff, 0xfe,
                                 0xff, 0xfe,
                                 0xff, 0xfe,
                                 0xff, 0xfe,
                                 0xff, 0xfe,
                                 0xff, 0xff};

  fwrite(fatheader, 1, sizeof(fatheader), fd);

  // step 3: writeout blank rest
  unsigned char blankrest[4193260];
  memset(blankrest, 0x00, sizeof(blankrest));
  fwrite(blankrest, 1, sizeof(blankrest), fd);

  fclose(fd);

  return RET_OK;

}

int FileSystem::load(){
  return -1;
}

int FileSystem::listdir(const std::string &path, std::vector<std::string> &result){
  (void)path;
  (void)result;
  return -1;
}

int FileSystem::makedir(const std::string &path){
  (void)path;
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

