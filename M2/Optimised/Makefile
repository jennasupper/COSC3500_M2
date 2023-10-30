CFLAGS = -std=gnu99 -lm -pg -O0 

output: setup_final.o flightpath_final.o
	gcc setup_final.o flightpath_final.o ${CFLAGS} -o flightpath_final

flightpath_final.o: flightpath_final.c setup_final.h
	gcc -c ${CFLAGS} flightpath_final.c

setup_final.o: setup_final.c setup_final.h
	gcc -c ${CFLAGS} setup_final.c

clean:
	rm flightpath_final.o setup_final.o flightpath_final