all: arc4random.so

include Makefile.mess

SRCS += src/main.c

CFLAGS += -Wall -Wno-pointer-sign
CFLAGS += -O2 -fPIC -DNDEBUG

LDFLAGS += -lm

OBJS := $(patsubst %.c,%.o,$(SRCS))

debug: CFLAGS += -ggdb3 -UNDEBUG
debug: all

arc4random.so: $(OBJS)
	$(CC) -o arc4random.so $(OBJS) $(LDFLAGS)

clean:
	rm -f arc4random.so
	rm -f $(OBJS)
