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

class MakeDirCommand : public Command {
  public:
    MakeDirCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = "USAGE: `mkdir /path/to/dir`";
    }

    bool validate() {
      return opts.size() == 1;
    }

    void run(FileSystem& fs) {
      (void)fs;
      debug("Will create dir `" + opts[0] + "`");
      // TODO: fs.mkdir(opts[0]);
    }
};

class CreateCommand : public Command {
  public:
    CreateCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = "USAGE: `create /path/to/file`";
    }

    bool validate() {
      return opts.size() == 1;
    }

    void run(FileSystem& fs) {
      (void)fs;
      debug("Will create file `" + opts[0] + "`");
      // TODO: fs.createfile(opts[0], filesout);
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

class UnlinkCommand : public Command {
  public:
    UnlinkCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = "USAGE: `unlink /path/to/dir` (or file)";
    }

    bool validate() {
      return opts.size() == 1;
    }

    void run(FileSystem& fs) {
      (void)fs;
      debug("Will unlink `" + opts[0] + "`");
      // TODO: fs.listdir(opts[0], filesout);
    }
};

class WriteCommand : public Command {
  public:
    WriteCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = "USAGE: `write \"contents-to\" /path/to/file`";
    }

    bool validate() {
      return opts.size() == 2;
    }

    void run(FileSystem& fs) {
      (void)fs;
      debug("Will write \"" + opts[0] + "\" to `" + opts[1] + "`");
      // TODO: fs.listdir(opts[0], filesout);
    }
};

class AppendCommand : public Command {
  public:
    AppendCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = "USAGE: `append \"contents-to\" /path/to/file`";
    }

    bool validate() {
      return opts.size() == 2;
    }

    void run(FileSystem& fs) {
      (void)fs;
      debug("Will append \"" + opts[0] + "\" to `" + opts[1] + "`");
      // TODO: fs.append(opts[0], filesout);
    }
};

class ReadCommand : public Command {
  public:
    ReadCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = "USAGE: `read /path/to/file`";
    }

    bool validate() {
      return opts.size() == 1;
    }

    void run(FileSystem& fs) {
      (void)fs;
      debug("Will read `" + opts[0] + "`");
      // TODO: fs.read(opts[0], fileout);
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

  if (cmd_name == "mkdir")
    return new MakeDirCommand(cmd_name, raw_opts);

  if (cmd_name == "create")
    return new CreateCommand(cmd_name, raw_opts);

  if (cmd_name == "unlink")
    return new UnlinkCommand(cmd_name, raw_opts);

  if (cmd_name == "write")
    return new WriteCommand(cmd_name, raw_opts);

  if (cmd_name == "append")
    return new AppendCommand(cmd_name, raw_opts);

  if (cmd_name == "read")
    return new ReadCommand(cmd_name, raw_opts);

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
