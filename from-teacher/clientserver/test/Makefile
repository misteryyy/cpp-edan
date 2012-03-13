#
# Makefile to make the test programs myserver and myclient,
# using the classes Connection and Server.
#
# The executables are created in the current directory.
#

# Define the compiler
CXX	 = g++

# Define the linker. This must be done since the implicit rule
# for linking .o-files uses CC as the linker.
CC	 = g++

# Define preprocessor and compiler flags
CPPFLAGS  = -I..
CXXFLAGS  = -ggdb  -Wall -W -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -Wold-style-cast

# Define linker flags
LDFLAGS	 = -ggdb -L..

# Libraries: -lclientserver is always necessary
LDLIBS	 = -lclientserver 
# -lsocket is necessary on Solaris
#LDLIBS	+= -lsocket
# -lnsl is necessary on Froste (Solaris)
#LDLIBS	+= -lnsl

# Define source files
SRC	= $(wildcard *.cc)

# Targets
PROGS	= myserver myclient

# Phony targets
.PHONY: all clean cleaner

all: $(PROGS)

# Targets rely on implicit rules for compiling and linking.
# The dependency on libclientserver.a is not defined.
myserver: myserver.o  
myclient: myclient.o

# Standard clean and cleaner
clean:
	$(RM) *.o

cleaner: clean
	$(RM) $(PROGS)

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
	 $(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

# Include dependencies in *.d files
include $(SRC:.cc=.d)
