output: demo.o simple_crypto.o
	gcc demo.o simple_crypto.o -o demo

demo.o: demo.c
	gcc -c demo.c

simple_crypto.o: simple_crypto.c simple_crypto.h
	gcc -c simple_crypto.c

clean:
	rm *.o demo