IDIR =.
CC=g++
CFLAGS=-I$(IDIR) -Wall -Wextra

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

$(LIB_OUT): $(OBJ)
	ar -r -s $(LIB_OUT) $(OBJ)

full:
	make lib
	make test

test: $(OBJ)
	$(CC) -o SvVis_PC_test \
		SvVis_PC_test.cpp \
		$(LIB_OUT) $(CPPSOCK)
	$(CC) -o SvVis_PC_test_server \
		SvVis_PC_test_server.cpp \
		$(LIB_OUT) $(CPPSOCK)

lib: $(OBJ)
	make $(LIB_OUT)

clear:
	rm -f $(OBJ)
	rm -f SvVis_PC_test SvVis_PC_test.exe SvVis_PC_test_server SvVis_PC_test_server.exe

remake:
	make clear
	make full