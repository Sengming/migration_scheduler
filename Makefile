#declerations
CC = g++
CFLAGS = -c -std=c++11 -g -O0
DIRECTORIES = Schedulers/ Monitor/ SimulatorCore/
SRC_FILES = $(filter-out SchedulerSimulator.cpp, $(wildcard *.cpp))


all: Simulator

Simulator: directories extra
	g++ -std=c++11 SchedulerSimulator.cpp $(wildcard objects/*) -o Simulator

directories:
	mkdir -p objects/
	for dir in $(DIRECTORIES); do\
		cd $$dir;\
		g++ -w -c -g -O0 -std=c++11 *.cpp; \
		mv *.o ../objects; \
		cd -;\
	done
extra: 
	g++ -c -std=c++11 $(SRC_FILES);
	mv *.o objects/;	

clean: 
	rm -rf objects/ Simulator_EXE *.tex
