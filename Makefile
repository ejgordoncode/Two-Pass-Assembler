# NOTE TO DEVS: I am including comments for us that will be
#    removed once development is complete. 
#    These comments serve as reminders and explanations for
#    decisions that I make in code, and they help solidify
#    my memory of code I've written.
#    - @ejgordoncode

# MAKEFILE STATEMENT STRUCTURE
#
# The following is the general structure of makefile statements:
# 
#    target: dependencies
#        [TAB] command
#  
# Where...
#    target = c file you'd build (usually exe)
#    dependencies = files the target (exe) depends on (.cpp or .h)
#    command = the way to build your executable, must begin with
#      a [TAB]
#
# Our target (exe) is asx (the assembler program executable name). 
# The asx depends on obj files (.o)
# Obj files depend on .cpp and .h files
# the "clean" target is included to delete compiled files
#

# MAKEFILE COMMANDS STRUCTURE
#  
# [TAB] $(VAR_NAME) $(VAR_NAME2)... -flag $symbol
# 
# Where...
#    $(VAR_NAME) = retrieve the value of a variable called VAR_NAME
#    -flag = optional parameters in the command
#    $symbol = syntax for an automatic variable (a rule that determines
#      how make will automatically assign the value)


# VARIABLES FOR MAKE COMMANDS

CC = g++ # CC "C Compiler" holds the compile program 
CFLAGS = -Wall -g # -Wall to include compile warnings, 
                  #   -g to include debug info
SOURCEFILES = $(wildcard *.cpp) # this automates the source files list
                              #   to look for any files with extension
                              #   ".cpp", preventing make from failing
                              #   due to a file name change
OBJFILES = $(SOURCEFILES:.cpp=.o) # the list of obj files to combine in executable 
# NOTE: $(SOURCEFILES:.cpp=.o) allows make to auto-generate the object file name
#   based on the source (.cpp) file name


# MAKE COMMANDS

# asx command links the obj files and creates asx executable
asx: $(OBJFILES)
  $(CC) $(CFLAGS) -o asx $(OBJFILES) # "-o asx" means output file will
                                     #    be named "asx"
# .o command takes single souce file ($<) and compiles into .o file without linking
%.o: %.cpp
  $(CC) $(CFLAGS) -c $< # -c means compile only (don't link, no executable)
                        # $< is an auto variable that gets value of the first
                        #    prerequisite (dependency) of the rule that triggered
                        #    this command

clean:
  rm -f *.o asx 
# rm = the Unix/Linux cmd to del files
# -f = "force", forces removal without approval and ignores nonexistant files
