all: pow sqrt

pow: pow.c
	gcc pow.c -lmpc -lmpfr -lgmp -o pow

sqrt: sqrt.c
	gcc sqrt.c -lmpc -lmpfr -lgmp -o sqrt

clean:
	rm -rf *.o pow sqrt
