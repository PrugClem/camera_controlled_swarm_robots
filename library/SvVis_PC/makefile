IDIR =.
CC=g++
CFLAGS=-I$(IDIR) -Wall -Wextra

ODIR=obj
LDIR =.

LIBS=-lm

LIB_OUT = libSvVis_PC.a
CPPSOCK = ../cppsock/out/libcppsock.a
CPPSOCK_OBJ = 	../cppsock/obj/cppsock_addressinfo.cpp.o\
		 		../cppsock/obj/cppsock_socket.cpp.o\
				../cppsock/obj/cppsock_socketaddr.cpp.o\
				../cppsock/obj/cppsock_utility.cpp.o

_DEPS = SvVis_PC.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = SvVis_PC.cpp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

full:
	make lib
	make test

# create object file
$(ODIR)/%.o: % $(DEPS)
	mkdir obj || cd .
	$(CC) -c -o $@ $< $(CFLAGS)

# create library
$(LIB_OUT): $(OBJ)
# ar single line command
	#ar -r $(LIB_OUT) $(OBJ) $(CPPSOCK_OBJ)
# create temporary SvVis library to merge
	ar -r -s tmplib.a $(OBJ)
# merge SvVis library with cppsock library
	sh ../../merge_lib.sh $(LIB_OUT) tmplib.a $(CPPSOCK)
	rm tmplib.a

#build est programs
test: $(OBJ)
	$(CC) -pthread -o SvVis_PC_test \
		SvVis_PC_test.cpp \
		$(LIB_OUT)
	$(CC) -pthread -o SvVis_PC_test_server \
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