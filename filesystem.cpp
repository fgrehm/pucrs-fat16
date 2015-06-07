
#include "filesystem.h"

FileSystem::FileSystem(){
}

FileSystem::~FileSystem(){
}

int FileSystem::init(){
}

int FileSystem::load(){
}

int FileSystem::listdir(const std::string &path, std::vector<std::string> &result){
}

int FileSystem::makedir(const std::string &path){
}

int FileSystem::createfile(const std::string &path){
}

int FileSystem::unlink(const std::string &path){
}

int FileSystem::write(const std::string &path, const std::string &content){
}

int FileSystem::append(const std::string &path, const std::string &content){
}

int FileSystem::read(const std::string &path, std::string &content){
}

