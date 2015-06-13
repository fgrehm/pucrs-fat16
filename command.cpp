#include <iostream>
#include <stdlib.h>
#include <string>

#include "utils.h"
#include "command.h"

/***************************************************
 * Custom commands logic
 ***************************************************/

class InvalidCommand : public Command {
  public:
    InvalidCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = name + ": command not found";
    }

    bool validate() {
      return false;
    }
};

class ExitCommand : public Command {
  public:
    ExitCommand(const std::string &n, const std::string& o) : Command(n, o) {}

    void run(FileSystem&) {
      exit(0);
    }
};

class InitCommand : public Command {
  public:
    InitCommand(const std::string &n, const std::string& o) : Command(n, o) {}

    void run(FileSystem& fs) {
      std::cout << "=> Formating partition..." << std::endl;
      fs.init();
      std::cout << "=> DONE" << std::endl;
    }
};

class LoadCommand : public Command {
  public:
    LoadCommand(const std::string &n, const std::string& o) : Command(n, o) {}

    void run(FileSystem& fs) {
      std::cout << "=> Loading partition table..." << std::endl;
      fs.load();
      std::cout << "=> DONE" << std::endl;
    }
};

class ListCommand : public Command {
  public:
    ListCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = "USAGE: `ls /path/to/dir`";
    }

    bool validate() {
      return opts.size() == 1;
    }

    void run(FileSystem& fs) {
      (void)fs;
      debug("Will list `" + opts[0] + "`");
      // TODO: fs.listdir(opts[0], filesout);
    }
};

/***************************************************
 * Command "factory"
 ***************************************************/

Command *Command::parse(const std::string& input) {
  std::string cmd_name = input;
  std::string raw_opts;

  if (cmd_name.find(' ') != std::string::npos) {
    std::string cmd = cmd_name;
    cmd_name = cmd.substr(0, cmd.find(' '));
    raw_opts = cmd.substr(cmd_name.length()+1, cmd.length());
  }

  if (cmd_name == "exit")
    return new ExitCommand(cmd_name, raw_opts);

  if (cmd_name == "init")
    return new InitCommand(cmd_name, raw_opts);

  if (cmd_name == "load")
    return new LoadCommand(cmd_name, raw_opts);

  if (cmd_name == "ls")
    return new ListCommand(cmd_name, raw_opts);

  // fgtodo: Criar as classes apropriadas para cada comando
  bool valid_command = cmd_name == "init" \
                       || cmd_name == "mkdir" \
                       || cmd_name == "create" \
                       || cmd_name == "unlink" \
                       || cmd_name == "write" \
                       || cmd_name == "append" \
                       || cmd_name == "read";

  if (valid_command) {
    std::cout << "[DEBUG] Returning dummy command..." << std::endl;
    return new Command(cmd_name, raw_opts);
  }

  return new InvalidCommand(cmd_name, raw_opts);
}

/***************************************************
 * Logic common to all commands
 ***************************************************/

Command::Command(const std::string &n, const std::string &o) : name(n), help_text(), opts() {
  // fgtodo: Tratar `write "foo  bar" /dir/file` corretamente, split nao vai ser suficiente
  opts = split(o, ' ');
}
Command::~Command() {}

bool Command::validate() {
  return true;
}

void Command::run(FileSystem&) {
  std::cout << " => Will execute: '" << name << "'" << std::endl;
  std::cout << "    -> Arguments provided: ";
  if (opts.size() > 0) {
    std::cout << "['" << join(opts, "', '") << "']";
  }
  std::cout << std::endl;
}
