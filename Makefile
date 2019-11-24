
CC=gcc
LIBS=-lpthread -lm 
CFLAGS=-g -Wall -std=gnu99
OBJECTS=bounded_queue.o thread_safe_bounded_queue.o

all: part1 part2 part3 part4 part5

#//Your TEST program names go here. Add the list of object dependencies and c file with a main function.
# See instructions below for creating objects from C files
# Add the command as below to replacing with your program name (bq_test|ts_bq_test) in the example below:
# Then add it to the list above, and it will be part of the make build process.

part1: bounded_queue.o thread_safe_bounded_queue.o topicqueue.o part1.c
	$(CC) -o part1 $(CFLAGS) part1.c topicqueue.o bounded_queue.o thread_safe_bounded_queue.o $(LIBS)

part2: threadfxn.o part2.c
	$(CC) -o part2 $(CFLAGS) part2.c threadfxn.o $(LIBS)

part3: bounded_queue.o thread_safe_bounded_queue.o topicqueue.o threadfxn.o p1fxns.o part3.c
	$(CC) -o part3 $(CFLAGS) part3.c bounded_queue.o thread_safe_bounded_queue.o topicqueue.o threadfxn.o p1fxns.o $(LIBS)

part4: bounded_queue.o thread_safe_bounded_queue.o topicqueue.o p1fxns.o threadfxn_p4.o part4.c
	$(CC) -o part4 $(CFLAGS) part4.c p1fxns.o bounded_queue.o thread_safe_bounded_queue.o topicqueue.o threadfxn_p4.o $(LIBS)

part5: bounded_queue.o thread_safe_bounded_queue.o topicqueue.o p1fxns.o threadfxn_p4.o part5.c
	$(CC) -o part5 $(CFLAGS) part5.c p1fxns.o bounded_queue.o thread_safe_bounded_queue.o topicqueue.o threadfxn_p4.o $(LIBS)
# EACH LINE BELOW COMPILES AS FOLLOWS:
# Compile each C file using default, example for bounded_queue.o below:
#
#       bounded_queue.o: bounded_queue.c bounded_queue.h
#
# Expands to:
#
#       $(CC) $(CFLAG) -o boundounded_queue.o bounded_queueue.c bounded_queue.h $(LIBS)
#
# Expands to:
#
#       gcc -g -W -Wall -pedantic -o boundounded_queue.o bounded_queueue.c bounded_queue.h -lpthread -lm
# So just add your object file name, c source file, header dependencies
# then add it to the list for your program above.



bounded_queue.o: bounded_queue.c bounded_queue.h
thread_safe_bounded_queue.o: thread_safe_bounded_queue.c thread_safe_bounded_queue.h
topicqueue.o: topicqueue.c topicqueue.h
threadfxn.o: threadfxn.c threadfxn.h
threadfxn_p4.o: threadfxn_p4.c threadfxn_p4.h
p1fxns.o: p1fxns.h p1fxns.c

# The general order of an instruction is:
#       <target>: <dependN...>
#               <cmd1>
#               <cmd2>
#               
#  If the <target> is not found (file on disk) then check for <dependN> (including other make command)
#       if all dependencies satisfied,
#               run cmd1
#               run cmd2...

clean:
	rm -f *.o
	rm -f part1
	rm -f part2
	rm -f part3
	rm -f part4
	rm -f part5
	
	
	
	
	
	
	
	
	
	
	
	
	
	

