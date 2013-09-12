hw5: main.o solve.o count.o gen.o
	gcc -o hw5 -g main.o count.o solve.o gen.o

main.o: main.c solve.h count.h gen.h
	gcc -g -Wall -c main.c

count.o: count.c count.h gen.h solve.h
	gcc -g -Wall -c count.c

solve.o: solve.c count.h gen.h solve.h
	gcc -g -Wall -c solve.c

gen.o: gen.c count.h gen.h solve.h
	gcc -g -Wall -c gen.c

	
clean: 
		rm *.o
		rm hw5
		