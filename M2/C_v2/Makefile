MPI_INC = /usr/include/openmpi-x86_64
MPI_LIB = /usr/lib64/openmpi/lib

CFLAGS = -lm -pg -O0 -fopenmp -mavx -std=c99 -g -L$(MPI_LIB) -lmpi_cxx -lmpi -I$(MPI_INC)

output: setup_final.o flightpath_final.o
	gcc setup_final.o flightpath_final.o ${CFLAGS} -o flightpath_final

flightpath_final.o: flightpath_final.c setup_final.h
	gcc -c ${CFLAGS} flightpath_final.c

setup_final.o: setup_final.c setup_final.h
	gcc -c ${CFLAGS} setup_final.c

clean:
	rm flightpath_final.o setup_final.o flightpath_final
