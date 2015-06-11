#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "console.h"
#include "command.h"
#include "utils.h"

void run_command(Command *command){
  if (command->validate())
    command->run();
  else
    std::cout << command->error_message << std::endl;
}

void run_console() {
  std::cout << "PUCRS FAT16 SHELL" << std::endl;
  while (true) {
    std::cout << "$ ";
    std::string cmd;

    if (!getline(std::cin, cmd)) break;

    cmd = trim(cmd);
    if (cmd != "")
      run_command(Command::parse(cmd));
  }
}
