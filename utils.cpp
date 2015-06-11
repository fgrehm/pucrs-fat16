#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "utils.h"

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
  std::vector<std::string> ret;
  for (int i=0; i<path.size(); i++){
    if (path[i] == '/'){
      // ...
      // mvtodo: implement
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
