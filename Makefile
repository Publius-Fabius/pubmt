
CFLAGS = -g -std=c11 -pedantic -Wconversion -Wall -I include
CC = gcc

clean:
	rm -r build/pubmt || true
	rm -r build || true
	rm -r bin || true

build:
	mkdir build
	mkdir build/pubmt

bin:
	mkdir bin 

scaffold: build bin

build/pubmt/linked_list.o : source/pubmt/linked_list.c \
	include/pubmt/linked_list.h \
	scaffold 
	$(CC) $(CFLAGS) -c -o $@ $<
bin/test_linked_list: tests/pubmt/linked_list.c \
	build/pubmt/linked_list.o 
	$(CC) $(CFLAGS) -o $@ $^ 
run_test_linked_list : bin/test_linked_list
	valgrind -q --error-exitcode=1 --leak-check=full $^ 1>/dev/null

build/pubmt/dynamic_array.o : source/pubmt/dynamic_array.c \
	include/pubmt/dynamic_array.h \
	scaffold 
	$(CC) $(CFLAGS) -c -o $@ $<
bin/test_dynamic_array: tests/pubmt/dynamic_array.c \
	build/pubmt/dynamic_array.o 
	$(CC) $(CFLAGS) -o $@ $^ 
run_test_dynamic_array : bin/test_dynamic_array
	valgrind -q --error-exitcode=1 --leak-check=full $^ 1>/dev/null

build/pubmt/binary_heap.o : source/pubmt/binary_heap.c \
	include/pubmt/binary_heap.h \
	scaffold 
	$(CC) $(CFLAGS) -c -o $@ $<
bin/test_binary_heap: tests/pubmt/binary_heap.c \
	build/pubmt/binary_heap.o \
	build/pubmt/dynamic_array.o
	$(CC) $(CFLAGS) -o $@ $^ 
run_test_binary_heap : bin/test_binary_heap
	valgrind -q --error-exitcode=1 --leak-check=full $^ 1>/dev/null

build/pubmt/byte_stack.o : source/pubmt/byte_stack.c \
	include/pubmt/byte_stack.h \
	scaffold 
	$(CC) $(CFLAGS) -c -o $@ $<
bin/test_byte_stack: tests/pubmt/byte_stack.c \
	build/pubmt/byte_stack.o \
	build/pubmt/dynamic_array.o
	$(CC) $(CFLAGS) -o $@ $^ 
run_test_byte_stack : bin/test_byte_stack
	valgrind -q --error-exitcode=1 --leak-check=full $^ 1>/dev/null

build/pubmt/hash_map.o : source/pubmt/hash_map.c \
	include/pubmt/hash_map.h \
	scaffold 
	$(CC) $(CFLAGS) -c -o $@ $<
bin/test_hash_map: tests/pubmt/hash_map.c \
	build/pubmt/hash_map.o \
	build/pubmt/dynamic_array.o \
	build/pubmt/linked_list.o 
	$(CC) $(CFLAGS) -o $@ $^ 
run_test_hash_map : bin/test_hash_map
	valgrind -q --error-exitcode=1 --leak-check=full $^ 1>/dev/null

build/pubmt/avl_tree.o : source/pubmt/avl_tree.c \
	include/pubmt/avl_tree.h \
	scaffold 
	$(CC) $(CFLAGS) -c -o $@ $<
bin/test_avl_tree: tests/pubmt/avl_tree.c \
	build/pubmt/avl_tree.o
	$(CC) $(CFLAGS) -o $@ $^ 
run_test_avl_tree : bin/test_avl_tree
	valgrind -q --error-exitcode=1 --leak-check=full $^ 1>/dev/null

suite: \
	run_test_linked_list \
	run_test_dynamic_array \
	run_test_binary_heap \
	run_test_byte_stack \
	run_test_hash_map \
	run_test_avl_tree
