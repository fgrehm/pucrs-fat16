#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "console.h"
#include "command.h"

void run_command(const Command *command){
  if (command->cmd == "exit"){
    exit(0);
  } else if (command->cmd == "") {
    std::cout << " => No command provided!" << std::endl;
  } else {
    std::cout << " => Will execute: " << command->cmd << std::endl;
  }
}

void run_console() {
  std::cout << "PUCRS FAT16 SHELL" << std::endl;
  while (true) {
    std::cout << "$ ";
    std::string cmd;
    getline(std::cin, cmd);
    run_command(Command::parse(cmd));
  }
}
