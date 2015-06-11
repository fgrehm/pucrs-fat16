#include <iostream>
#include <stdlib.h>
#include <string>

#include "utils.h"
#include "command.h"

Command::Command(const std::string &n, const std::string &o) : name(n), opts() {
  // fgtodo: Tratar `write "foo  bar" /dir/file` corretamente, split nao vai ser suficiente
  opts = split(o, ' ');
}
Command::~Command() {}

Command* Command::parse(const std::string& input) {
  std::string cmd_name = trim(input);
  std::string raw_opts;

  if (cmd_name.find(' ') != std::string::npos) {
    std::string cmd = cmd_name;
    cmd_name = cmd.substr(0, cmd.find(' '));
    raw_opts = cmd.substr(cmd_name.length()+1, cmd.length());
  }

  return new Command(cmd_name, raw_opts);
}
