#include <iostream>
#include <stdlib.h>
#include <string>

void run_command(const std::string &cmd){

  if (cmd == "exit"){
    exit(0);
  } 

}

std::string parse_command(const std::string& input) {
  return input.substr(0, input.find(' '));
}

int main() {
  std::cout << "PUCRS FAT16 SHELL" << std::endl;
  while (true) {
    std::cout << "$ ";
    std::string cmd;
    getline (std::cin, cmd);
    run_command(parse_command(cmd));
  }
  return 0;
}
