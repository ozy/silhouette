SRCDIR	= src
HDRDIR  = include
OBJDIR	= build
BINDIR	= bin
DEPDIR	= dep

CC	= gcc
TARGET	= main
CFLAGS	= -flto -O2 -I$(HDRDIR)
LFLAGS	= -lm -pthread

SOURCES	:= $(wildcard $(SRCDIR)/*.c)
HEADERS := $(wildcard $(HDRDIR)/*.h)
OBJECTS	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS	:= $(OBJECTS:$(OBJDIR)/%.o=$(DEPDIR)/%.d)

REMOVE	:= rm -rf

# Linking
$(BINDIR)/$(TARGET): $(OBJECTS) $(CMN_OBJ)
	mkdir -p $(BINDIR)
	$(CC) $(LFLAGS) -o $@ $(OBJECTS) $(CMN_OBJ)
	@echo "Linking complete"

-include $(DEPS)

# Compilation
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	mkdir -p $(DEPDIR)
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -I$(HDRDIR) -MM -MT '$(OBJDIR)/$*.o' $(SRCDIR)/$*.c > $(DEPDIR)/$*.d
	@echo "Compiled $<"

# Generate file list for cscope
cscope.files: $(SOURCES) $(HEADERS)
	echo $(SOURCES) $(HEADERS) > cscope.files

# Generate cscope database
cscope.out: cscope.files
	cscope -q -R -b -i cscope.files

.PHONY: clean
clean:
	$(REMOVE) $(OBJECTS) $(OBJDIR) $(BINDIR) $(DEPDIR)
	@echo "Deleted $<"

.PHONY: remove
remove:
	$(REMOVE) $(BINDIR)/$(TARGET)
	$(REMOVE) $(OBJECTS)
	$(REMOVE) $(DEPS)
	$(REMOVE) cscope.*
	@echo "Deleted $<"

.PHONY: cscope
cscope: cscope.out

.PHONY: all
all: $(BINDIR)/$(TARGET)
