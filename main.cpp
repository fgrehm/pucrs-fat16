#include <iostream>
#include <string>

int main() {
  std::cout << "PUCRS FAT16 SHELL\n";
  std::cout << "$ ";

  std::string name;
  getline (std::cin, name);
  std::cout << "-> Will execute: " << name << "\n";
}
