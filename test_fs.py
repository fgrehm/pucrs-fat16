#!/usr/bin/env python

if __name__ == "__main__":
    with open("./fat.part", "rb") as f:

        # testa integridade do boot block
        for i in xrange(1024):
            b = f.read(1)
            if ord(b) != 0xbb:
                print("Boot block invalido no offset %d" % i)
                exit(1)

        print("boot block valido")

