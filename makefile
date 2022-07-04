all: lab2.out

lab2.out: lab2.c funciones.h funciones.c 
	gcc lab2.c funciones.h funciones.c -pthread -Wall -lm -o lab2.out

run1:
	./lab2.out -i test100.csv -o propiedades.txt -d 100 -n 4 -h 4 -c 100
run2:
	./lab2.out -i test10000.csv -o propiedades.txt -d 100 -n 4 -h 4 -c 100
clean:
	rm -f *.o lab2.out propiedades.txt