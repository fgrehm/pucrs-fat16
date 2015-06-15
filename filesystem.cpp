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

  std::string str1k;
  std::string str2k;
  for (unsigned int i=0; i<1024; i++){
    str1k += "a";
    str2k += "bb";
  }

  this->makedir("/home");
  this->makedir("/home/box");
  this->makedir("/home/box/bolo");
  this->createfile("/home/box/torto.txt");
  this->createfile("/home/box/cusco.txt");
  this->write("/home/box/cusco.txt", str2k);
  this->unlink("/home/box/torto.txt");
  std::vector<std::string> resp;
  this->listdir("/home/box", resp);
  for (unsigned int i=0; i<resp.size(); i++){
    std::cout << "ls: " << resp[i] << std::endl;
  }
  int stop=1; // mvdebug

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

  dir_entry_t new_dir_struct;
  dir_entry_t dir_cluster[32];
  unsigned short cluster_offset = 0;
  const std::string new_dir_name = utils_basename(path);

  int cof_i = traverse_path(path, ROOTDIR_OFFSET);
  if (cof_i == -1){
      std::string aux = "Could not follow path: ";
      aux += path;
      throw FSExcept(aux, RET_INTERNAL_ERROR);
  } else{
    cluster_offset = cof_i;
  }

  readblock(dir_cluster, cluster_offset);

  if (has_in_dir(new_dir_name, dir_cluster)){
    return RET_DIR_ALREADY_EXISTS;
  }

  const unsigned short d_idx = find_free_in_dir(dir_cluster);
  unsigned short f_idx = 0;

  int test_aux = find_free_fat();
  if (test_aux == -1){
    return RET_FAT_FULL;
  } else {
    f_idx = test_aux;
  }

  fmt_ushort_into_uchar8pair(&(fat[f_idx*2]), 0xffff);
  fmt_char8_into_uchar8(new_dir_struct.filename, new_dir_name.c_str());
  new_dir_struct.attributes = 1;
  fmt_ushort_into_uchar8pair(new_dir_struct.first_block, f_idx);
  fmt_uint_into_uchar8quad(new_dir_struct.size, 1024);

  memcpy(&dir_cluster[d_idx], &new_dir_struct, sizeof(dir_entry_t));
  dumpfat();
  writeblock(dir_cluster, cluster_offset);

 return RET_OK;

}

int FileSystem::listdir(const std::string &path, std::vector<std::string> &result){
  CHECK_INIT 

  dir_entry_t dir_cluster[32];
  unsigned short cluster_offset = 0;

  std::string banzai = path;
  banzai += "/xoxoxo";
  int cof_i = traverse_path(banzai, ROOTDIR_OFFSET); // BANZAI!!!!!!
  if (cof_i == -1){
      std::string aux = "Could not follow path: ";
      aux += path;
      throw FSExcept(aux, RET_INTERNAL_ERROR);
  } else{
    cluster_offset = cof_i;
  }
  readblock(dir_cluster, cluster_offset);

  std::vector<std::string> ret;
  for (unsigned int i=0; i<32; i++){
    if (dir_cluster[i].filename[0] != 0x00){
      std::string aux = fmt_ascii7_to_stdstr(dir_cluster[i].filename);
      ret.push_back(aux);
    }
  }
  result = ret;

  return RET_OK;
}

int FileSystem::createfile(const std::string &path){
  CHECK_INIT 

  dir_entry_t new_dir_struct;
  dir_entry_t dir_cluster[32];
  unsigned short cluster_offset = 0;
  const std::string new_dir_name = utils_basename(path);

  int cof_i = traverse_path(path, ROOTDIR_OFFSET);
  if (cof_i == -1){
      std::string aux = "Could not follow path: ";
      aux += path;
      throw FSExcept(aux, RET_INTERNAL_ERROR);
  } else{
    cluster_offset = cof_i;
  }

  readblock(dir_cluster, cluster_offset);

  if (has_in_dir(new_dir_name, dir_cluster)){
    return RET_DIR_ALREADY_EXISTS;
  }

  const unsigned short d_idx = find_free_in_dir(dir_cluster);
  unsigned short f_idx = 0;

  int test_aux = find_free_fat();
  if (test_aux == -1){
    return RET_FAT_FULL;
  } else {
    f_idx = test_aux;
  }

  fmt_ushort_into_uchar8pair(&(fat[f_idx*2]), 0xffff);
  fmt_char8_into_uchar8(new_dir_struct.filename, new_dir_name.c_str());
  new_dir_struct.attributes = 0;
  fmt_ushort_into_uchar8pair(new_dir_struct.first_block, f_idx);
  fmt_uint_into_uchar8quad(new_dir_struct.size, 0);

  memcpy(&dir_cluster[d_idx], &new_dir_struct, sizeof(dir_entry_t));
  dumpfat();
  writeblock(dir_cluster, cluster_offset);

  return RET_OK;
}

int FileSystem::unlink(const std::string &path){
  CHECK_INIT 

  dir_entry_t dir_cluster[32];
  unsigned short cluster_offset = 0;

  int cof_i = traverse_path(path, ROOTDIR_OFFSET);
  if (cof_i == -1){
    std::string aux = "Could not follow path: ";
    aux += path;
    throw FSExcept(aux, RET_INTERNAL_ERROR);
  } else{
    cluster_offset = cof_i;
  }
  readblock(dir_cluster, cluster_offset);

  std::string target = utils_basename(path);
  int rm_i = find_match_in_dir(target, dir_cluster);
  if (rm_i == -1){
    std::string aux = "Could not follow path: ";
    aux += path;
    throw FSExcept(aux, RET_INTERNAL_ERROR);
  }

  // da baixa no arquivo
  dir_cluster[rm_i].filename[0] = 0x00;
  follow_fat_erase(dir_cluster[rm_i].first_block);

  writeblock(dir_cluster, cluster_offset);
  dumpfat();
  return RET_OK;
}

int FileSystem::write(const std::string &path, const std::string &content){
  CHECK_INIT 

  dir_entry_t dir_cluster[32];
  unsigned short cluster_offset = 0;
  const unsigned int new_fsize = content.size();
  if (new_fsize == 0){
    return RET_OK; // we dont care
  }

  int cof_i = traverse_path(path, ROOTDIR_OFFSET);
  if (cof_i == -1){
    std::string aux = "Could not follow path: ";
    aux += path;
    throw FSExcept(aux, RET_INTERNAL_ERROR);
  } else{
    cluster_offset = cof_i;
  }
  readblock(dir_cluster, cluster_offset);

  std::string target = utils_basename(path);
  int wr_i = find_match_in_dir(target, dir_cluster);
  if (wr_i == -1){
    std::string aux = "Could not follow path: ";
    aux += path;
    throw FSExcept(aux, RET_INTERNAL_ERROR);
  }

  fmt_uint_into_uchar8quad(dir_cluster[wr_i].size, new_fsize);
  writeblock(dir_cluster, cluster_offset);

  // mvtodo: follow fat and lay file contents

  return RET_OK;
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
  if (fd_write == NULL){
    throw FSExcept("Cannot create dummy file.part", RET_INTERNAL_ERROR);
  }

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
  if (!initialized)
    return;

  for (unsigned int i=0; i<8; i++){
    writeblock(&(fat[i*1024]), (i+1)*1024);
  }
}

bool FileSystem::has_in_dir(const std::string &name, const dir_entry_t *dir) const {
  int ret = find_match_in_dir(name, dir);
  if (ret == -1){
    return false;
  } else {
    return true;
  }
}

int FileSystem::find_match_in_dir(const std::string &name, const dir_entry_t *dir) const {
  for (unsigned int i=0; i<32; i++){
    if (dir[i].filename[0] != 0x00){
      std::string rd_str = fmt_ascii7_to_stdstr(dir[i].filename);
      if (rd_str == name){
        return i;
      }
    }
  }

  return -1;
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
      return (i/2);
    }
  }
  return -1;
}

// percorre um path e devolve o offset do cluster do diretorio pai (ultimo)
int FileSystem::traverse_path(const std::string &path, const unsigned short offsetstart){

  int ret = offsetstart;

  std::vector<std::string> sep = tokenize_path(path);
  std::string nextpath = popleft_path(path);

  if (sep.size() == 1){
    // we are at the parent. return offset here
    return ret;
  } else {
    // we need to go deeper...

    dir_entry_t current_cluster[32];
    readblock(current_cluster, offsetstart);
    int md_i = find_match_in_dir(sep[0], current_cluster);
    if (md_i == -1){
      std::string aux = "Could not follow path: ";
      aux += path;
      throw FSExcept(aux, RET_INTERNAL_ERROR);
    }

    if (current_cluster[md_i].attributes != 1){
      std::string aux = "Could not follow path (non-dir in the middle!): ";
      aux += path;
      throw FSExcept(aux, RET_INTERNAL_ERROR);
    }

    unsigned short fid = fmt_uchar8pair_to_ushort(current_cluster[md_i].first_block);
    ret = traverse_path(nextpath, fid*1024);
    return ret;

  }

}

void FileSystem::follow_fat_erase(const unsigned char *fb){

  // persegue um first block (fb) na fat e vai liberando na fat.
  // esta funcao nao dumpeia a fat pro disco. chamar manualmente depois!

  unsigned short fid = fmt_uchar8pair_to_ushort(fb);
  if (fat[fid*2] == 0xff && fat[(fid*2)+1] == 0xff){ // EOF
    // limpa e sai...
    fat[fid*2] = 0x00;
    fat[(fid*2)+1] = 0x00;
    return;
  } else {
    // limpa e persegue
    unsigned char auxbuf[2];
    auxbuf[0] = fat[fid*2];
    auxbuf[1] = fat[(fid*2)+1];
    fat[fid*2] = 0x00;
    fat[(fid*2)+1] = 0x00;
    follow_fat_erase(auxbuf);
  }

}

unsigned short FileSystem::lay_file_contents(const unsigned short fid_start){
  // mvtodo
  return -1;
}

