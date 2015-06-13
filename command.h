#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

#include "filesystem.h"

class Command {
  public:
    static Command *parse(const std::string&);

    Command(const std::string&, const std::string&);
    virtual ~Command();

    virtual bool validate();
    virtual void run(FileSystem&);

    std::string name;
    std::string help_text;
    std::vector<std::string> opts;
};

#endif
