#include <iostream>
#include <stdlib.h>
#include <string>

#include "utils.h"
#include "command.h"
#include "fsexcept.h"

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
      exit(0); // TODO: this may cause memory leaks. lets change it if we can help it
    }
};

class InitCommand : public Command {
  public:
    InitCommand(const std::string &n, const std::string& o) : Command(n, o) {}

    void run(FileSystem& fs) {
      std::cout << "=> Initializing file system...";
      int result = fs.init();
      if (result != RET_OK){
        print_exception("Error initializing file system", result);
      } else {
        std::cout << " DONE!" << std::endl;
      }
    }
};

class LoadCommand : public Command {
  public:
    LoadCommand(const std::string &n, const std::string& o) : Command(n, o) {}

    void run(FileSystem& fs) {
      std::cout << "=> Loading filesystem...";
      int result = fs.load();
      if (result != RET_OK){
        print_exception("Error loading file system", result);
      } else {
        std::cout << " DONE!" << std::endl;
      }
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
      int result = fs.makedir(opts[0]);
      if (result == RET_OK) {
        std::cout << "=> Directory created." << std::endl;
      } else if (result == RET_NOT_INITIALIZED) {
        std::cout << "[ERROR] Filesystem not loaded!" << std::endl;
      } else if (result == RET_DIR_ALREADY_EXISTS) {
        std::cout << "[ERROR] Directory already exists!" << std::endl;
      } else if (result == RET_FAT_FULL) {
        std::cout << "[ERROR] FAT is full!" << std::endl;
      } else {
        print_exception("Error creating directory", result);
      }
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
      try {
        int result = fs.createfile(opts[0]);
        if (result == RET_FAT_FULL) {
          std::cout << "[ERROR] FAT is full!" << std::endl;
        } else if (result == RET_DIR_ALREADY_EXISTS) {
          std::cout << "[ERROR] Directory already exists!" << std::endl;
        } else if (result == RET_NOT_INITIALIZED) {
          std::cout << "[ERROR] Filesystem not loaded!" << std::endl;
        } else if (result != RET_OK) {
          print_exception("Error creating file", result);
        }
      } catch(FSExcept &e) {
        print_exception("Error creating file: " + e.message, e.code);
      }
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
      std::vector<std::string> resp;
      try {
        int result = fs.listdir(opts[0], resp);
        if (result == RET_OK) {
          if (resp.size() == 0) {
            std::cout << "=> Directory is empty" << std::endl;
          } else {
            std::cout << opts[0] << std::endl;
            for (unsigned int i=0; i<resp.size(); i++){
              std::cout << resp[i] << std::endl;
            }
          }
        } else if (result == RET_NOT_INITIALIZED) {
          std::cout << "[ERROR] Filesystem not loaded!" << std::endl;
        } else {
          print_exception("Error listing directory", result);
        }
      } catch(FSExcept &e) {
        print_exception("Error listing directory: " + e.message, e.code);
      }
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
      try {
        int result = fs.unlink(opts[0]);
        if (result == RET_NOT_INITIALIZED) {
          std::cout << "[ERROR] Filesystem not loaded!" << std::endl;
        } else if (result != RET_OK) {
          print_exception("Error unlinking", result);
        }
      } catch(FSExcept &e) {
        print_exception("Error unlinking: " + e.message, e.code);
      }
    }
};

class WriteCommand : public Command {
  public:
    WriteCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = "USAGE: `write contents-to /path/to/file`";
    }

    bool validate() {
      return opts.size() == 2;
    }

    void run(FileSystem& fs) {
      try {
        int result = fs.write(opts[1], opts[0]);
        if (result == RET_NOT_INITIALIZED) {
          std::cout << "[ERROR] Filesystem not loaded!" << std::endl;
        } else if (result != RET_OK) {
          print_exception("Error writing", result);
        }
      } catch(FSExcept &e) {
        print_exception("Error writing: " + e.message, e.code);
      }
    }
};

class AppendCommand : public Command {
  public:
    AppendCommand(const std::string &n, const std::string& o) : Command(n, o) {
      help_text = "USAGE: `append contents-to /path/to/file`";
    }

    bool validate() {
      return opts.size() == 2;
    }

    void run(FileSystem& fs) {
      try {
        int result = fs.append(opts[1], opts[0]);
        if (result == RET_NOT_INITIALIZED) {
          std::cout << "[ERROR] Filesystem not loaded!" << std::endl;
        } else if (result != RET_OK) {
          print_exception("Error appending", result);
        }
      } catch(FSExcept &e) {
        print_exception("Error appending: " + e.message, e.code);
      }
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
      try {
        std::string contents;
        int result = fs.read(opts[0], contents);
        if (result == RET_OK) {
          std::cout << "File contents:" << std::endl;
          std::cout << contents << std::endl;
        } else if (result == RET_NOT_INITIALIZED) {
          std::cout << "[ERROR] Filesystem not loaded!" << std::endl;
        } else {
          print_exception("Error reading", result);
        }
      } catch(FSExcept &e) {
        print_exception("Error reading: " + e.message, e.code);
      }
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
