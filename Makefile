CC = clang

PROJECT_NAME = graphics

CFLAGS = -std=c99 -Wall -Wextra -Wno-unused-parameter -Isrc -Ilib/include $(cflags)
LDFLAGS = lib/lib-arm64/libglfw3.a

LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
CFLAGS += -fsanitize=address -g

EXE = $(PROJECT_NAME)
OBJ = src/main.o \
	lib/glad.o
DEP = $(OBJ:%.o=%.d)

CLEAN_FILES = $(OBJ) $(EXE)

# store rm command in a variable so you can update it in the future if you want to target more operating systems
RM = rm -f

# .PHONY tells make that these targets won't output a file with the name of the target 
.PHONY: default clean compile_flags

default: $(EXE)

$(EXE): $(OBJ)
		# $@ is the target name
		$(CC) -o $@ $(LDFLAGS) $(OBJ) $(CFLAGS)

%.o: %.c
		# $< are the target's dependencies
		$(CC) -c -o $@ $< $(CFLAGS) -MMD

clean:
		$(RM) $(CLEAN_FILES)

# generates a file to tell the LSP what compile flags you got
compile_flags:
		$(foreach flag,$(CFLAGS),echo $(flag) >> compile_flags.txt;)

-include $(DEP)