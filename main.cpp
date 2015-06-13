
#include "console.h"
#include "filesystem.h"
#include "utils.h"

int main() {

  std::string file = get_env("PARTITION_FILE");
  if (file == "")
    file = "./fat.part";
  FileSystem fs(file);
  fs.debug();

  //run_console(fs); // mvtodo: temporarily disabling console because its causing a memory leak somewhere. debug it later.
  return 0;
}

