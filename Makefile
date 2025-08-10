CC = gcc
CFLAGS = -O2 -Iinclude -Wall -Wextra -pedantic
LDFLAGS = -lGL -Llib -lglfw3 -lm
DIR_BUILD = build
DIR_OBJ = $(DIR_BUILD)/linux/objects
BIN_NAME = grim_fetcher
BIN_NAME_WIN = GrimFetcher.exe
BIN = $(DIR_BUILD)/linux/$(BIN_NAME)
SRC = $(shell find lib -name '*.c') $(shell find src -name '*.c')
OBJ = $(patsubst %.c, $(DIR_OBJ)/%.o, $(SRC))

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(DIR_OBJ)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all win64 win32 clean clean-linux clean-win64 clean-win32 fclean 
.PHONY: fclean-linux fclean-win64 fclean-win32 re re-win64 re-win32

# Package: gcc-mingw-w64
win64:
	@$(MAKE) --no-print-directory all \
	CC=x86_64-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win64/objects \
	BIN=$(DIR_BUILD)/win64/$(BIN_NAME_WIN) \
	LDFLAGS="-mwindows -Llib/win64 -lglfw3 -lopengl32 -lgdi32 -lm"
win32:
	@$(MAKE) --no-print-directory all \
	CC=i686-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win32/objects \
	BIN=$(DIR_BUILD)/win32/$(BIN_NAME_WIN) \
	LDFLAGS="-mwindows -Llib/win32 -lglfw3 -lopengl32 -lgdi32 -lm"

clean:
	@$(MAKE) --no-print-directory clean-linux
	@$(MAKE) --no-print-directory clean-win64
	@$(MAKE) --no-print-directory clean-win32
clean-linux:
	rm -rf $(DIR_BUILD)/linux/objects
clean-win64:
	rm -rf $(DIR_BUILD)/win64/objects
clean-win32:
	rm -rf $(DIR_BUILD)/win32/objects

fclean:
	@$(MAKE) --no-print-directory fclean-linux
	@$(MAKE) --no-print-directory fclean-win64
	@$(MAKE) --no-print-directory fclean-win32
fclean-linux: clean-linux
	rm -rf $(DIR_BUILD)/linux/$(BIN_NAME)
fclean-win64: clean-win64
	rm -rf $(DIR_BUILD)/win64/$(BIN_NAME_WIN)
fclean-win32: clean-win32
	rm -rf $(DIR_BUILD)/win32/$(BIN_NAME_WIN)

re: fclean-linux all

re-win64: fclean-win64 win64

re-win32: fclean-win32 win32
