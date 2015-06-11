#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "console.h"
#include "command.h"
#include "utils.h"

void run_command(const Command *command){
  if (command->name == "exit"){
    exit(0);
  } else if (command->name == "") {
    std::cout << " => No command provided!" << std::endl;
  } else {
    std::cout << " => Will execute: '" << command->name << "'" << std::endl;
    std::cout << "    -> Arguments provided: ";
    if (command->opts.size() > 0) {
      std::cout << "['" << join(command->opts, "', '") << "']";
    }
    std::cout << std::endl;
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
