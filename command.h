#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

class Command {
  public:
    static Command *parse(const std::string&);

    Command(const std::string&, const std::string&);
    virtual ~Command();

    virtual bool validate() = 0;
    virtual void run() = 0;

    std::string name;
    std::string error_message;
    std::vector<std::string> opts;
};

class BaseCommand : public Command {
  public:
    BaseCommand(const std::string &n, const std::string& o) : Command(n, o) {}

    virtual bool validate();
    virtual void run();
};

class ExitCommand : public BaseCommand {
  public:
    ExitCommand(const std::string &n, const std::string& o) : BaseCommand(n, o) {}

    void run();
};

#endif
