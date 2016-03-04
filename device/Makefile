main:main.o device.o
	gcc main.o device.o -o main
main.o:main.c device.h
	gcc -c main.c -o main.o
gps.o:device.c  device.h
	gcc -c device.c -o device.o
clean:
	rm *.o main -rf
