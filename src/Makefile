CROSS = arm-none-linux-gnueabi-

CC = $(CROSS)gcc
AR = $(CROSS)ar

CFLAGS = -shared -fPIC

CPPFLAGS := -I ../inc
LDFLAGS  += -lpthread

SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SRC))


libcommon.so:$(OBJ)
	$(CC) $(CFLAGS) -o ../lib/$@ $^

$(OBJ):%.o:%.c
	$(CC) $< -o $@ $(CPPFLAGS) $(LDFLAGS) -c -fPIC 
	
clean:
	$(RM) $(OBJ)
