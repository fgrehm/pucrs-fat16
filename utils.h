
#ifndef UTILS_H
#define UTILS_H

#define DEBUG 1

#include <string>
#include <vector>

std::string trim(const std::string&);
std::vector<std::string> tokenize_path(const std::string &);
std::vector<std::string> split(const std::string&, char);
std::string join(const std::vector<std::string>&, std::string);
void debug(const std::string&);

#endif
