#include <iostream>
#include <stdlib.h>
#include <string>

#include "utils.h"
#include "command.h"

Command::Command(const std::string &n, const std::string &o) : name(n), error_message(), opts() {
  // fgtodo: Tratar `write "foo  bar" /dir/file` corretamente, split nao vai ser suficiente
  opts = split(o, ' ');
}
Command::~Command() {}

Command Command::parse(const std::string& input) {
  std::string cmd_name = trim(input);
  std::string raw_opts;

  if (cmd_name.find(' ') != std::string::npos) {
    std::string cmd = cmd_name;
    cmd_name = cmd.substr(0, cmd.find(' '));
    raw_opts = cmd.substr(cmd_name.length()+1, cmd.length());
  }

  return Command(cmd_name, raw_opts);
}

bool Command::validate() {
  return true;
}

void Command::run() {
  if (name == "exit"){
    exit(0);
  } else if (this->name == "") {
    std::cout << " => No command provided!" << std::endl;
  } else {
    std::cout << " => Will execute: '" << name << "'" << std::endl;
    std::cout << "    -> Arguments provided: ";
    if (opts.size() > 0) {
      std::cout << "['" << join(opts, "', '") << "']";
    }
    std::cout << std::endl;
  }
}
