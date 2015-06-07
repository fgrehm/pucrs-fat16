#!/usr/bin/env python

import os

if __name__ == "__main__":

  # testa tamanho do FS virtual total
  statinfo = os.stat('fat.part')
  if statinfo.st_size != 4194304:
    print("fat.part tamanho invalido. Deve ter exatamente 4Mb (4194304).")
    exit(1)

  with open("./fat.part", "rb") as f:

    # testa integridade do boot block
    for i in xrange(1024):
      b = f.read(1)
      if ord(b) != 0xbb:
        print("Boot block invalido no offset %d." % i)
        exit(1)

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

    print("boot block valido")

  print("FS virtual valido!")

