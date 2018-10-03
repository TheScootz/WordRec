debugFlag=-g

p2: WordRec.o p2.o 
	g++ -o p2 p2.o WordRec.o $(debugFlag)

p2.o: p2.cpp WordRec.h
	g++ -c p2.cpp $(debugFlag)

WordRec.o: WordRec.cpp WordRec.h 
	g++ -c WordRec.cpp $(debugFlag)

clean: 
	\rm -f *.o p2