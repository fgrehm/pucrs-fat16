#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

class Command {
  public:
    static Command parse(const std::string&);

    Command(const std::string&, const std::string&);
    ~Command();

    bool validate();
    void run();

    std::string name;
    std::string error_message;
    std::vector<std::string> opts;
};

#endif
