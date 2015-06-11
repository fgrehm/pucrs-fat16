#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

class Command {
  public:
    static Command* parse(const std::string&);

    Command(const std::string&, const std::string&);
    ~Command();

    std::string name;
    std::vector<std::string> opts;
};

#endif
