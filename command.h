#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

class Command {
  public:
    static Command* parse(const std::string &);

    Command();
    ~Command();

    std::string cmd;
    std::vector<std::string> opts;
};

#endif
