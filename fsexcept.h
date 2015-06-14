
#ifndef _EXCEPT_H_
#define _EXCEPT_H_

#include <string>

class FSExcept {

  public:
    FSExcept(const std::string &, const int);
    std::string message;
    int code;

};

#endif // _EXCEPT_H_

