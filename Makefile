CC=gcc
CCOPTS=--std=gnu99 -g -Wall -D_LIST_DEBUG_
AR=ar

OBJS=bit_map.o\
     buddy_allocator.o

HEADERS= bit_map.h buddy_allocator.h

LIBS=libbuddy.a

BINS=buddy_test buddy_allocator_test

.phony: clean all


all:	$(BINS)

%.o:	%.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

buddy_test: buddy_test.o $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^ -lm

buddy_allocator_test: buddy_allocator_test.o $(LIBS)
	$(CC) $(CCOPTS) -o $@ $^ -lm

clean:
	rm -rf *.o *~ $(BINS)
