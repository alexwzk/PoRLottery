# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'SimplePoR'
# 'make clean'  removes all .o and executable files

# define the C++ compiler to use
CPPC = g++

# define any compile-time flags
CFLAGS = -O0 -g3 -Wall -fmessage-length=0 -std=c++11

# define any directories containing header files other than /usr/include
# Inlucde Openssl (current version is 1.0.1i)
INCLUDES = -I/usr/local/openssl/include 

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -L/usr/local/openssl/lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like:
LIBS = -lcrypto

# define the C++ source files
SRCS = src/randengine.cpp src/common.cpp src/path.cpp src/ticket.cpp src/merkle.cpp src/dealer.cpp src/user.cpp src/verifier.cpp src/fps.cpp main.cpp

# define the C++ object files 
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .cpp of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.cpp=.o)

# define the executable file 
MAIN = SimplePoR

# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:	$(MAIN)
	@echo  project SimplePoR has been compiled

$(MAIN): $(OBJS)
	$(CPPC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .cpp's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .cpp file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
# unused argument : -cpp
# 	$(CPPC) $(CFLAGS) $(INCLUDES) -cpp $< -c -o $@
.cpp.o:
	$(CPPC) $(CFLAGS) $(INCLUDES) $< -c -o $@

clean:
	$(RM) *.o src/*.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
