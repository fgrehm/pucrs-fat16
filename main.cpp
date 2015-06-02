#include <iostream>
#include <stdlib.h>
#include <string>
#include "utils.h"

std::string parse_command(const std::string& input) {
  std::string cmd = trim(input);
  return cmd.substr(0, cmd.find(' '));
}

void run_command(const std::string &cmd){
  if (cmd == "exit"){
    exit(0);
  } else if (cmd == "") {
    std::cout << " => No command provided!" << std::endl;
  } else {
    std::cout << " => Will execute: " << parse_command(cmd) << std::endl;
  }
}

int main() {
  std::cout << "PUCRS FAT16 SHELL" << std::endl;
  while (true) {
    std::cout << "$ ";
    std::string cmd;
    getline(std::cin, cmd);
    run_command(parse_command(cmd));
  }
  return 0;
}
