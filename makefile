proj3: driver.o
	g++ -o proj3 driver.o
main.o: driver.cpp
	g++ -c driver.cpp
clean:
	rm *.o proj3