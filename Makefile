LIBS = -lzvbi

z2r_objs = zvbi2raw.o

zvbi2raw: ${z2r_objs}
	${CC} -o $@ ${z2r_objs} ${LIBS}

clean:
	rm zvbi2raw zvbi2raw.o
