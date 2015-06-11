
#include "console.h"
#include "filesystem.h"

int main() {
  FileSystem fs("./fat.part");
  fs.debug();

  run_console();
  return 0;
}
