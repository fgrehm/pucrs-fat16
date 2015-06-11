#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

class Command {
  public:
    static Command *parse(const std::string&);

    Command(const std::string&, const std::string&);
    virtual ~Command();

    virtual bool validate();
    virtual void run();

    std::string name;
    std::string error_message;
    std::vector<std::string> opts;
};

#endif
