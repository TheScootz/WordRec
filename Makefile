CC=g++
debug=-g

p2: p2.o WordRec.o  
	$(CC) -o p2 p2.o WordRec.o $(debug)

p2.o: p2.cpp WordRec.h 
	$(CC) -c p2.cpp  $(debug)

WordRec.o: WordRec.cpp WordRec.h
	$(CC) -c WordRec.cpp $(debug)	

clean:
	\rm -f *.o p2
