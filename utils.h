
#ifndef UTILS_H
#define UTILS_H

#define DEBUG 1

#include <string>
#include <vector>

std::string trim(const std::string&);
std::vector<std::string> tokenize_path(const std::string &);
std::vector<std::string> split(const std::string&, char);
std::string join(const std::vector<std::string>&, std::string);
std::string get_env(const std::string&);
void debug(const std::string&);
bool file_exists (const std::string& name);
std::string fmt_ascii7_to_stdstr(const unsigned char *);
void write_fmt_char8_into_uchar8(unsigned char*, const char *);

#endif
