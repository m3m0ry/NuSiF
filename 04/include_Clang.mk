CC  = clang
CXX = clang++
FC  = gfortran
LINKER = $(CXX)

ANSI_CFLAGS  = -ansi
ANSI_CFLAGS += -std=c++0x
ANSI_CFLAGS += -pedantic
ANSI_CFLAGS += -Wextra

CFLAGS   = -O3 -Wno-format  -Wall $(ANSI_CFLAGS) 
# More warning pls
CFLAGS   += -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align  -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return
# Maybe too much warnings
CFLAGS   += -Wcast-qual -Wswitch-default -Wconversion -Wunreachable-code
CXXFLAGS := $(CFLAGS) 
# Specific C flags
CFLAGS   := $(CFLAGS) -Wstrict-prototypes
FCFLAGS  = 
CPPFLAGS = -std=c++0x 
LFLAGS   = #-fopenmp
DEFINES  = -D_GNU_SOURCE -DNDEBUG
INCLUDES = #-I/usr/lib/gcc/x86_64-linux-gnu/4.8/include/
LIBS     = #-L/usr/lib/x86_64-linux-gnu/libomp.so.5
