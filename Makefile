.PHONY : all clean compile link

TESTAPPNAME=pucrs-fat16

SRC= main.cpp utils.cpp console.cpp filesystem.cpp fat16.cpp
OBJ= main.o   utils.o   console.o   filesystem.o   fat16.o

ARCH := $(shell getconf LONG_BIT)

ifeq ($(MODE),)
	MODE=debug
endif

ifeq ($(MODE),debug)
INTFLAGS = \
	-g \
	-Wall \
	-Wextra \
	-pedantic \
	-Weffc++ \
	-fPIC \
	-pthread
POSTBUILD=
#DEFINES= -D _DEBUG -D _LOG
endif

ifeq ($(MODE),release)
INTFLAGS = \
   	-O2 \
	-Wall \
	-Wextra \
	-pedantic \
	-Weffc++ \
	-fPIC \
	-pthread
POSTBUILD= strip $(TESTAPPNAME)
#DEFINES= -D _LOG
endif

ifeq ($(ARCH),64)
CPPFLAGS = $(INTFLAGS) -m64
else
CPPFLAGSS = $(INTFLAGS)
endif

INCLUDES=-I.
LDFLAGS=

all: compile link

compile:
	g++ -c $(INCLUDES) $(SRC) $(CPPFLAGS)

link:
	g++ -o $(TESTAPPNAME) $(OBJ) $(LDFLAGS)
	$(POSTBUILD)

clean:
	rm -rf $(OBJ)
	rm -rf $(TESTAPPNAME)

