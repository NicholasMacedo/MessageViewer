all: post addauthor a3
	chmod +x view.py
	chmod +x getNewMessage.py
	chmod +x getNewSortMessage.py
	chmod +x getstreams.py
	chmod +x getNumRead.py
	chmod +x markAsRead.py
	mkdir messages
	chmod -R 777 messages

stream.o: stream.c stream.h
	gcc -ansi -Wall -g -c stream.c -o stream.o

libstream.a: stream.o
	ar cr libstream.a stream.o

a1: a1.c a1.h
	gcc -Wall -ansi -g a1.c -o a1

post.c: post.cc a1
	./a1 post.cc

post: post.c libstream.a stream.h
	gcc -Wall -ansi -g post.c -o post -lstream -L.

addauthor: addauthor.c libstream.a stream.h
	gcc -Wall -ansi -g addauthor.c -o addauthor -lstream -L.
a3: a3.c
	gcc -Wall -ansi -g a3.c -o a3

clean: 
	rm post addauthor a1 post.c libstream.a stream.o a3

rmmessages:
	rm -r messages
