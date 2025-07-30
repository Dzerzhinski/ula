test: test.o ntp_unix.o
	g++ -std=c++20 test.o ntp_unix.o -lpthread -lgtest -lgtest_main -o test
	./test

test.o: 
	g++ -c test.cpp -o test.o

ntp_unix.o:
	gcc -c ../ntp_unix.c -o ./ntp_unix.o


clean:
	rm *.o



