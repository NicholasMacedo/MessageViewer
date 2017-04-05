all: post addauthor a3 db dbFunctions
	chmod +x view.py
	chmod +x getNewMessage.py
	chmod +x getNewSortMessage.py
	chmod +x getstreams.py
	chmod +x getNumRead.py
	chmod +x markAsRead.py

stream.o: stream.c stream.h
	gcc -ansi -Wall -g -c stream.c -o stream.o -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

libstream.a: stream.o
	ar cr libstream.a stream.o

a1: a1.c a1.h
	gcc -Wall -ansi -g a1.c -o a1 -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

post.c: post.cc a1
	./a1 post.cc

post: post.c libstream.a stream.h
	gcc -Wall -ansi -g post.c -o post -lstream -L. -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

addauthor: addauthor.c libstream.a stream.h
	gcc -Wall -ansi -g addauthor.c -o addauthor -lstream -L. -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

a3: a3.c
	gcc -Wall -ansi -g a3.c -o a3

db: db.c
	gcc -Wall -ansi -g db.c -o db -lm -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

dbFunctions: dbFunctions.c
	gcc -Wall -ansi -g dbFunctions.c -o dbFunctions -lm -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

server:
	scp -r . nmacedov@portkey.socs.uoguelph.ca:/home/undergrad/1/nmacedov/CIS2750/A4

web:
	scp -r . nmacedov@2750web.socs.uoguelph.ca:/srv/www/nmacedov

clean: 
	rm post addauthor a1 post.c libstream.a stream.o a3 db dbFunctions
