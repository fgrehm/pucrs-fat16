
#include "filesystem.h"

#include <iostream>
#include <fstream>

FileSystem::FileSystem(const std::string &partfname):m_partfilename(partfname), m_fat(), m_rootdir(), m_datablocks(){
}

FileSystem::~FileSystem(){
}

void FileSystem::debug(){

  std::cout << "Debugging the filesystem class..." << std::endl;
  this->init();

}

int FileSystem::init(){

  // step 1: writeout 1024 0xbb's
  std::ofstream partfile;
  partfile.open("./fat.part");
  partfile.close();

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

