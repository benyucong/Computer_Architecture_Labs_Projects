CC=gcc
CFLAGS=-Wall -std=c99

all: linked_list vector bit_ops

linked_list: linked_list.o test_linked_list.o
	$(CC) -o linked_list linked_list.o test_linked_list.o

linked_list.o: linked_list.c linked_list.h
	$(CC) $(CFLAGS) -g -c linked_list.c

test_linked_list.o: test_linked_list.c linked_list.h
	$(CC) $(CFLAGS) -g -c test_linked_list.c

vector: vector.o test_vector.o
	$(CC) -o vector vector.o test_vector.o

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -g -c vector.c

test_vector.o: test_vector.c vector.h
	$(CC) $(CFLAGS) -g -c test_vector.c

bit_ops: bit_ops.o test_bit_ops.o
	$(CC) -o bit_ops bit_ops.o test_bit_ops.o

bit_ops.o: bit_ops.c bit_ops.h
	$(CC) $(CFLAGS) -g -c bit_ops.c

test_bit_ops.o: test_bit_ops.c bit_ops.h
	$(CC) $(CFLAGS) -g -c test_bit_ops.c

clean:
	rm linked_list linked_list.o test_linked_list.o vector vector.o test_vector.o bit_ops bit_ops.o test_bit_ops.o
