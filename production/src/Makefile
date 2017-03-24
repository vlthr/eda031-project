#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
LDFLAGS =   -g -L.
LDLIBS = -lclientserver -lsqlite3

#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++

all: libclientserver.a clientmain servermain



# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

libclientserver.a: connection.o server.o news.o protocol.o
	ar rv libclientserver.a news.o connection.o server.o protocol.o
	ranlib libclientserver.a

clientmain: clientmain.o libclientserver.a

servermain: servermain.o libclientserver.a inmemdb.o sqldb.o

install: all
	cp ./clientmain ../bin/ 
	cp ./servermain ../bin/ 
	cp ./Database.db ../bin/ 
	
# Phony targets
.PHONY: all clean 

# Standard clean
clean:
	rm -f *.o *.d libclientserver.a

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
SRC = $(wildcard *.cc)
include $(SRC:.cc=.d)
