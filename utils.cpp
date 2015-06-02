//#include <iostream>
#include <string>
//#include <locale>

// trim from both ends
std::string trim(const std::string &s) {
  std::string::const_iterator it = s.begin();
  while (it != s.end() && isspace(*it))
    it++;

  std::string::const_reverse_iterator rit = s.rbegin();
  while (rit.base() != it && isspace(*rit))
    rit++;

  return std::string(it, rit.base());
}
