CC=g++-4.9
CFLAGS=-std=c++11
#INCLUDEBOOST=-I ../../../boost_1_61_0
INCLUDEBCDPP=-I ../../../cdboost/include

all: main.o ../data_structures/message.o 
	$(CC) -g -o ABP main.o ../data_structures/message.o
	$(CC) -g -o main fork.cpp pcb.cpp 

main.o: main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDEBOOST) $(INCLUDEBCDPP) main.cpp -o main.o

../data_structures/message.o: 
	$(CC) -g -c $(CFLAGS) $(INCLUDEBOOST) $(INCLUDEBCDPP) ../data_structures/message.cpp -o ../data_structures/message.o

	

clean:
	rm -f ABP *.o *~
	-for d in ../data_structures; do (cd $$d; rm -f *.o *~ ); done
