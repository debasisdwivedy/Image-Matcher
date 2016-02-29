
# Define executable name
BIN = a2

# Define source files
SRCS = ./SiftMatcher.cpp ./Matrix.cpp ./utils.cpp ./Transform.cpp ./siftpp/sift.cpp ./HomographyEstimator.cpp ./ImageMatchResult.cpp ./Config.cpp ./FastSiftMatcher.cpp ./a2.cpp

# Define header file paths
INCPATH = -I. -Isiftpp

# Define the -L library path(s)
LDFLAGS = 

# Define the -l library name(s)
LIBS = -lX11 -lpthread

# Define Compile Flags
COMPILEFLAGS = -Wall

# Define Link Flags
LINKFLAGS =

# Do not make any changes below!
OBJS      = $(SRCS:.cpp=.o)
CXX       = g++
DEP_FILE  = .depend

.PHONY = all clean distclean


# Main entry point
#
all: depend $(BIN)


# For linking object file(s) to produce the executable
#
$(BIN): $(OBJS)
	@echo Linking $@
	@$(CXX) $(LINKFLAGS) $^ $(LDFLAGS) $(LIBS) -o $@


# For compiling source file(s)
#
.cpp.o:
	@echo Compiling $<
	@$(CXX) -c $(COMPILEFLAGS) $(INCPATH) $< -o $(<:.cpp=.o)


# For cleaning up the project
#
clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(BIN)
	$(RM) $(DEP_FILE)


# For determining source file dependencies
#
depend: $(DEP_FILE)
	@echo "Dependencies generated."
	
$(DEP_FILE):
	@echo Generating dependencies in $@
	@-$(CXX) -E -MM $(COMPILEFLAGS) $(INCPATH) $(SRCS) >> $(DEP_FILE)

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring distclean,$(MAKECMDGOALS)))
-include $(DEP_FILE)
endif
endif

