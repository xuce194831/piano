vpath %.c = src/
vpath %.h = inc/

SRCPATH = ./src

HEADER := $(wildcard inc/*.h)
SRC := $(wildcard src/*.c)
OBJ := $(SRC:%.c=%.o)

CROSS = arm-none-linux-gnueabi-

CC = $(CROSS)gcc

CPPFLAGS += -I ./inc

LDFLAGS += -L ./lib
LDFLAGS += -lcommon
LDFLAGS += -lpthread

LDFLAGS += -Wl,-rpath=./lib



piano.out:piano.c lib/libcommon.so $(SRC) $(HEADER)
	$(CC) $(SRC) $< -o $@ $(CPPFLAGS) $(LDFLAGS) -DNDEBUG

debug.out:piano.c lib/libcommon.so $(SRC) $(HEADER)
	$(CC) $(SRC) $< -o $@ $(CPPFLAGS) $(LDFLAGS) -DDEBUG



lib/libcommon.so:
	$(MAKE) -C $(SRCPATH)

clean:
	$(RM) $(OBJ) *.out ./src/*.o

distclean:clean
	$(RM) .*.sw? lib/libcommon.so
