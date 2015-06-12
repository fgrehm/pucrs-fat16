#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "utils.h"

void debug(const std::string& msg) {
  (void)msg; // Mute warnings
#ifdef DEBUG
  std::cout << "[DEBUG] " << msg << std::endl;
#endif
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
