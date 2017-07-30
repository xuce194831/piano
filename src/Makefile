CROSS = arm-none-linux-gnueabi-

CC = $(CROSS)gcc
AR = $(CROSS)ar

CFLAGS = -shared -fPIC

CPPFLAGS := -I ../inc

SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SRC))


libcommon.so:$(OBJ)
	$(CC) $(CFLAGS) -o ../lib/$@ $^

$(OBJ):%.o:%.c
	$(CC) $< -o $@ $(CPPFLAGS) -c
	
clean:
	$(RM) $(OBJ)
