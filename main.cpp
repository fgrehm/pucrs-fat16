#include <iostream>
#include <string>

std::string parse_command(const std::string& input) {
  return input.substr(0, input.find(' '));
}

int main() {
  std::cout << "PUCRS FAT16 SHELL\n";
  while (true) {
    std::cout << "$ ";
    std::string cmd;
    getline (std::cin, cmd);
    std::cout << "-> Will execute: " << parse_command(cmd) << "\n";
  }
}
