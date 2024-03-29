FILTER_OUT = $(foreach v,$(2),$(if $(findstring $(1),$(v)),,$(v)))
FILTER_IN  = $(foreach v,$(2),$(if $(findstring $(1),$(v)),$(v),))
#Compiler and Linker
CC          := g++

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
TARGETDIR   := bin
MAINDIR     := src/main
SRCEXT      := cpp
HEADEXT     := hpp
OBJEXT      := o

#The Target Binary Program
TARGET      := $(basename $(notdir $(shell find $(MAINDIR) -type f -name *.$(SRCEXT))))
TEST        := $(basename $(notdir $(wildcard ./src/tests/*.cpp)))
#TEST        := $(basename $(notdir $(wildcard ./src/tests/test_regen_path.cu)))
#Flags, Libraries and Includes
LDFLAGS  = -std=c++11 
CCFLAGS := -std=c++11 
INC         := -I$(INCDIR) -I/usr/local/include
INCDEP      := -I$(INCDIR)

SOURCES     := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
HEADERS     := $(shell find $(SRCDIR) -type f -name *.$(HEADEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))
MAIN_OBJ    := $(wildcard $(BUILDDIR)/main/*.o)
FILTER_OBJ  := $(call FILTER_OUT,test, $(OBJECTS))
FILTER_OBJ  := $(call FILTER_OUT,main,$(FILTER_OBJ))


#main
.PHONY: main
main: resources
main: LDFLAGS += -O3
main: CCFLAGS += -O3
main: $(TARGET)

.PHONY: parallel
parallel: LDFLAGS += -fopenmp
parallel: CCFLAGS += -fopenmp
parallel: fast


.PHONY: fast
fast: LDFLAGS += -DFASTEXP
fast: CCFLAGS += -DFASTEXP
fast: main


.PHONY: test
test: LDFLAGS += -g -W -Wall -ggdb3 -DDEBUG
test: CCFLAGS += -g -W -Wall -ggdb3 -DDEBUG 
test: $(TEST)
#Remake

.PHONY: debug
debug: clean
debug: resources
debug: LDFLAGS += -g -W -Wall -ggdb3 -DDEBUG
debug: CCFLAGS += -g -W -Wall -ggdb3 -DDEBUG 
debug: $(TARGET)

.PHONY: remake
remake: cleaner all
remake: main

.PHONY: print
print:
	echo $(HEADERS)

#Copy Resources from Resources Directory to Target Directory
resources: directories
    # @cp $(RESDIR)/* $(TARGETDIR)/

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Clean only Objecst
clean:
	@$(RM) -r $(BUILDDIR)
	@$(RM) -r $(TARGETDIR)

#Full Clean, Objects and Binaries
.PHONY: cleaner
cleaner: clean
	@$(RM) -r $(TARGETDIR)


#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(LDFLAGS) -o $(TARGETDIR)/$@ $(FILTER_OBJ) $(call FILTER_IN,$(basename $@),$(OBJECTS)) 

$(TEST): $(OBJECTS) $(HEADERS)
	@$(CC) $(LDFLAGS) -o $(TARGETDIR)/$@ $(FILTER_OBJ) $(call FILTER_IN,$(basename $@),$(OBJECTS))
	@./$(TARGETDIR)/$@ 
#@printf "\n" 


#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c -o $@ $<

#Non-File Targets
.PHONY: all remake clean cleaner resources


