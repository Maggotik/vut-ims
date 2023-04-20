CC=g++
-std=c++11 -lm

all: project

project: project.o
	$(CC) $(CFLAGS) -o project project.o

project.o: project.cpp
	$(CC) $(CFLAGS) -c project.cpp

run: project
	./project -d

vacc50: project
	./project -v50

vacc80: project
	./project -v80

vacc100: project
	./project -v100

clean:
	rm -f project *.o output.csv
