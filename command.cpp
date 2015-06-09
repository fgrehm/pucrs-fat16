#include "utils.h"
#include "command.h"

Command::Command() {}
Command::~Command() {}

Command* Command::parse(const std::string& input) {
  std::string cmd = trim(input);
  Command* ret = new Command();

  ret->cmd = cmd.substr(0, cmd.find(' '));

  return ret;
}
