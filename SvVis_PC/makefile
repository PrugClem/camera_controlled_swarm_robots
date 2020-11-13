IDIR =.
CC=g++
CFLAGS=-I$(IDIR) -Wall -Wextra

ODIR=obj
LDIR =.

LIBS=-lm

LIB_OUT = libSvVis_PC.a
CPPSOCK = ../cppsock/libcppsock.a

_DEPS = SvVis_PC.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = SvVis_PC.cpp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

full:
	make lib
	make test

# create object file
$(ODIR)/%.o: % $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# create library
$(LIB_OUT): $(OBJ)
# create temporary SvVis library to merge
	ar -r -s tmplib.a $(OBJ)
# merge SvVis library with cppsock library
	sh ../merge_lib.sh $(LIB_OUT) tmplib.a $(CPPSOCK)
	rm tmplib.a

#build est programs
test: $(OBJ)
	$(CC) -o SvVis_PC_test \
		SvVis_PC_test.cpp \
		$(LIB_OUT)
	$(CC) -o SvVis_PC_test_server \
		SvVis_PC_test_server.cpp \
		$(LIB_OUT)

lib: $(OBJ)
	make $(LIB_OUT)

clear:
	rm -f $(OBJ)
	rm -f SvVis_PC_test SvVis_PC_test.exe SvVis_PC_test_server SvVis_PC_test_server.exe $(LIB_OUT)

remake:
	make clear
	make full