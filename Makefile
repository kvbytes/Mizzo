# Karla Barrios (Section 05 - 12:30pm)
#       RedID: 823557131
#       CSSC : cssc3502
#  Jordan Duong (Section 03 - 11:00am)
#       RedID: 822600994	
#       CSSC : cssc3419
CC = gcc
CCFLAGS= -Wall -lpthread -lrt

# factory_controller.cpp producers.cpp consumers.cpp

default: main.c producer.o consumer.o belt.o
	${CC} -o mizzo main.c producer.o consumer.o belt.o ${CCFLAGS}

producer.o: producer.c producer.h
	${CC} -c producer.c ${CCFLAGS}
	
consumer.o: consumer.c consumer.h
	${CC} -c consumer.c ${CCFLAGS}
	
belt.o: belt.c belt.h
	${CC} -c belt.c ${CCFLAGS}

# run command works on linux only
run:
	./mizzo -E 1000 -L 3000 -f 600 -e 400

clean:
	rm *.o
	rm *.stackdump