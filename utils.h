
#ifndef UTILS_H
#define UTILS_H

#define DEBUG 1

#include <string>
#include <vector>

std::string trim(const std::string&);
std::vector<std::string> tokenize_path(const std::string &);
std::string popleft_path(const std::string &);
std::vector<std::string> split(const std::string&, char);
std::string join(const std::vector<std::string>&, std::string);
std::string get_env(const std::string&);
void debug(const std::string&);
void print_exception(const std::string&, int);
bool file_exists (const std::string& name);
std::string fmt_ascii7_to_stdstr(const unsigned char *);
void fmt_char8_into_uchar8(unsigned char*, const char *);
void fmt_ushort_into_uchar8pair(unsigned char*, const unsigned short);
unsigned short fmt_uchar8pair_to_ushort(const unsigned char*);
unsigned int fmt_uchar8quad_to_uint(const unsigned char*);
void fmt_uint_into_uchar8quad(unsigned char *, const unsigned int);
std::string utils_dirname(const std::string&);
std::string utils_basename(const std::string&);

#endif
