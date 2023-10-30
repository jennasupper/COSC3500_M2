CFLAGS = -std=gnu99 -lm -lhdf5 -lhdf5_hl -g -O0


output: setup_draft.o flightpath_draft.o
	gcc setup_draft.o flightpath_draft.o ${CFLAGS} -o flightpath_draft

flightpath_draft.o: flightpath_draft.c setup_draft.h
	gcc -c ${CFLAGS} flightpath_draft.c

setup_draft.o: setup_draft.c setup_draft.h
	gcc -c ${CFLAGS} setup_draft.c

clean:
	rm flightpath_draft.o setup_draft.o flightpath_draft