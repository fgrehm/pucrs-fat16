#!/usr/bin/env python

import os
import sys

if __name__ == "__main__":

  if len(sys.argv) < 2:
      print("Uso: test_fs.py part_file_name")
      exit(1)

  # testa tamanho do FS virtual total
  statinfo = os.stat(sys.argv[1])
  if statinfo.st_size != 4194304:
    print("Tamanho invalido. Deve ter exatamente 4Mb (4194304).")
    exit(1)

  with open(sys.argv[1], "rb") as f:

    # testa integridade do boot block
    for i in xrange(1024):
      b = f.read(1)
      if ord(b) != 0xbb:
        print("Boot block invalido no offset %d." % i)
        exit(1)
    print("Boot block: OK...")

    # testa integridade do header da FAT16
    b = f.read(2)
    if not(ord(b[0]) == 0xff and ord(b[1]) == 0xfd):
      print("Header da FAT16 invalido: identificador do boot record invalido.")
      exit(1)

    for i in xrange(8):
      b = f.read(2)
      if not(ord(b[0]) == 0xff and ord(b[1]) == 0xfe):
        print("Header da FAT16 invalido: corpo do header FAT16 invalido.")
        exit(1)

    b = f.read(2)
    if not(ord(b[0]) == 0xff and ord(b[1]) == 0xff):
      print("Header da FAT16 invalido: end of FAT16 invalido.")
      exit(1)
    print("FAT header: OK...")


  print("Filesystem: OK!")

