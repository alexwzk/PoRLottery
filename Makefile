# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'SimplePoR'
# 'make clean'  removes all .o and executable files

# define the C++ compiler to use
CPPC = g++

# define any compile-time flags
CFLAGS = -O0 -g3 -Wall -fmessage-length=0

# define any directories containing header files other than /usr/include
# Include Openssl (current version is 1.0.1j)
# Include Boost library  
# Include Bitcoin source file
INCLUDES = -I/opt/local/include/  -I/Users/zkwen/Documents/bitcoin/src 

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
# include openssl and boost library (installed by macport)
LFLAGS = -L/opt/local/lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like:
LIBS = -lcrypto -lboost_system-mt -lboost_filesystem-mt -lboost_program_options-mt -lboost_thread-mt -lboost_chrono-mt

# define the C++ source files
SRCS = /Users/zkwen/Documents/bitcoin/src/utilstrencodings.cpp /Users/zkwen/Documents/bitcoin/src/uint256.cpp src/randengine.cpp src/pmc.cpp src/buffer.cpp src/path.cpp src/merkle.cpp src/fps.cpp src/ticket.cpp main.cpp
# src/dealer.cpp src/user.cpp src/verifier.cpp

# define the C++ object files 
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .cpp of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.cpp=.o)

# define the executable file 
MAIN = PoRLottery

# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:	$(MAIN)
	@echo  project PoRLottery has been compiled

$(MAIN): $(OBJS)
	$(CPPC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .cpp's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .cpp file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CPPC) $(CFLAGS) $(INCLUDES) $< -c -o $@

clean:
	$(RM) *.o src/*.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
