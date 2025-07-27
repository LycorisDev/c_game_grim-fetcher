CC = gcc
CFLAGS = -O2 -Iinclude -Wall -Wextra -pedantic -g
LDFLAGS = -lGL -Llib -lglfw3 -lm
DIR_BUILD = build
DIR_OBJ = $(DIR_BUILD)/unix/objects
BIN = $(DIR_BUILD)/unix/grim_fetcher
SRC = $(shell find lib -name '*.c') $(shell find src -name '*.c')
OBJ = $(patsubst %.c, $(DIR_OBJ)/%.o, $(SRC))

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(DIR_OBJ)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all win64 win32 clean clean-unix clean-win64 clean-win32 fclean 
.PHONY: fclean-unix fclean-win64 fclean-win32 re re-win64 re-win32

# Package: gcc-mingw-w64
win64:
	$(MAKE) all \
	CC=x86_64-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win64/objects \
	BIN=$(DIR_BUILD)/win64/GrimFetcher-64bit.exe \
	LDFLAGS="-mwindows -Llib/win64 -lglfw3 -lopengl32 -lgdi32 -lm"
win32:
	$(MAKE) all \
	CC=i686-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win32/objects \
	BIN=$(DIR_BUILD)/win32/GrimFetcher-32bit.exe \
	LDFLAGS="-mwindows -Llib/win32 -lglfw3 -lopengl32 -lgdi32 -lm"

clean:
	$(MAKE) clean-unix
	$(MAKE) clean-win64
	$(MAKE) clean-win32
clean-unix:
	rm -rf $(DIR_BUILD)/unix/objects
clean-win64:
	rm -rf $(DIR_BUILD)/win64/objects
clean-win32:
	rm -rf $(DIR_BUILD)/win32/objects

fclean:
	$(MAKE) fclean-unix
	$(MAKE) fclean-win64
	$(MAKE) fclean-win32
fclean-unix:
	rm -rf $(DIR_BUILD)/unix
fclean-win64:
	rm -rf $(DIR_BUILD)/win64
fclean-win32:
	rm -rf $(DIR_BUILD)/win32

re: fclean-unix all

re-win64: fclean-win64 win64

re-win32: fclean-win32 win32
