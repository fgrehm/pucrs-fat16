#!/bin/bash

make
valgrind --tool=memcheck --leak-check=full ./pucrs-fat16

