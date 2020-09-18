IDIR =.
CC=g++
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =.

LIBS=-lm

LIB_OUT = libSvVis_PC.a
CPPSOCK = ../cppsock/out/libcppsock.a

_DEPS = SvVis_PC.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = SvVis_PC.cpp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: % $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

full:
	make lib
	make test

test: $(CPPSOCK) $(OBJ)
	$(CC) -o SvVis_PC_test \
		SvVis_PC_test.cpp \
		$(LIB_OUT) $(CPPSOCK)

lib: $(OBJ)
	ar -r -s $(LIB_OUT) $(OBJ)

clear:
	rm -f $(OBJ)
	rm -f SvVis_PC_test SvVis_PC_test.exe

remake:
	make clear
	make full