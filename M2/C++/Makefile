CFLAGS = -lm -pg -O0 -fopenmp -mavx

output: setup_final.o flightpath_final.o
	g++ setup_final.o flightpath_final.o ${CFLAGS} -o flightpath_final

flightpath_final.o: flightpath_final.cpp setup_final.h
	g++ -c ${CFLAGS} flightpath_final.cpp

setup_final.o: setup_final.cpp setup_final.h
	g++ -c ${CFLAGS} setup_final.cpp

clean:
	rm flightpath_final.o setup_final.o flightpath_final
