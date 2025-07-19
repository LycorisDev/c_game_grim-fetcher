CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
LDFLAGS = -lGL -Llib -lglfw3 -lm
BIN = grim_fetcher
BIN_W64 = GrimFetcher-64bit.exe
BIN_W32 = GrimFetcher-32bit.exe

SRC = $(wildcard lib/*.c) $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

# Package: gcc-mingw-w64
win64:
	$(MAKE) fclean
	$(MAKE) CC=x86_64-w64-mingw32-gcc BIN=$(BIN_W64) CFLAGS="$(CFLAGS)" \
		LDFLAGS="-mwindows -Llib/win64 -lglfw3 -lopengl32 -lgdi32 -lm"

# Package: gcc-mingw-w64
win32:
	$(MAKE) fclean
	$(MAKE) CC=i686-w64-mingw32-gcc BIN=$(BIN_W32) CFLAGS="$(CFLAGS)" \
		LDFLAGS="-mwindows -Llib/win32 -lglfw3 -lopengl32 -lgdi32 -lm"

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(BIN) $(BIN_W64) $(BIN_W32)

re: fclean all

.PHONY: all win64 win32 clean fclean re
