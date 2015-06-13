#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "utils.h"

std::string get_env(const std::string &var) {
  char *char_str = getenv(var.c_str());
  if (char_str == NULL)
    return "";
  return std::string(char_str);
}

void debug(const std::string& msg) {
  (void)msg; // Mute warnings
#ifdef DEBUG
  std::cout << "[DEBUG] " << msg << std::endl;
#endif
}

bool file_exists(const std::string& path) {
  struct stat fileStat;
  if (stat(path.c_str(), &fileStat)) {
    return false;
  }
  if (!S_ISREG(fileStat.st_mode)) {
    return false;
  }
  return true;
}

// Trim from both ends
// Based on http://ideone.com/nFVtEo
std::string trim(const std::string &s) {
  std::string::const_iterator it = s.begin();
  while (it != s.end() && isspace(*it))
    it++;

  std::string::const_reverse_iterator rit = s.rbegin();
  while (rit.base() != it && isspace(*rit))
    rit++;

  return std::string(it, rit.base());
}

std::vector<std::string> tokenize_path(const std::string &path){

  std::string i_path = path;
  std::vector<std::string> ret;

  // paths must be input in the format home/file.txt/ for the algorithm to work
  // i.e. no / in the beginning and / present in the end
  if (i_path[0] == '/'){
    i_path = i_path.substr(1, i_path.size()-1);
  }
  if (i_path[i_path.size()-1] != '/'){
    i_path += '/';
  }

  std::string partial;
  for (unsigned int i=0; i<i_path.size(); i++){
    if (i_path[i] == '/'){
      ret.push_back(partial);
      partial="";
    } else {
      partial += i_path[i];
    }
  }

  return ret;
}

// http://stackoverflow.com/a/236803/2738223
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    if (item != "")
      elems.push_back(item);
  }
  return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

std::string join(const std::vector<std::string> &vec, std::string delim) {
  std::stringstream ss;
  std::vector<std::string>::const_iterator iter = vec.begin();
  while(iter != vec.end()) {
    ss << *iter;
    ++iter;

    if (iter != vec.end())
      ss << delim;
  }
  return ss.str();
}

std::string fmt_ascii7_to_stdstr(const unsigned char *cstr){
  const char *cast = (const char*)cstr;
  std::string ret = cast;
  return ret;
}

void write_fmt_char8_into_uchar8(unsigned char *dest, const char *orig){
  strcpy((char*)dest, orig); 
}

