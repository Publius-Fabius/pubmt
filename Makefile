
CFLAGS = -g -std=c11 -pedantic -Wconversion -Wall -I include
CC = gcc

clean:
	rm -r build/pubmt 
	rm -r build 
	rm -r bin

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

suite: \
	run_test_linked_list \
	run_test_dynamic_array