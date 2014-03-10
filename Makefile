CC = g++
CFLAGS = -Wall -ggdb3 # -lm
OBJECTS = main.o rw.o pushRelabel.o queue.o circuits_cancel.o circuits.o negativeCommunity.o circle_cancel.o

.PHONY:all clean testmain
all:pushRelabel

pushRelabel:$(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^  -lm

testmain:testmain.o circle_cancel.o queue.o pushRelabel.o rw.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

circuits_cancel.o: circuits_cancel.cpp variable.hpp circuits_cancel.hpp
circuits.o: circuits.cpp circuits.hpp
main.o: main.cpp variable.hpp pushRelabel.hpp rw.hpp circuits.hpp circuits_cancel.hpp negativeCommunity.hpp
pushRelabel.o: pushRelabel.cpp queue.hpp pushRelabel.hpp variable.hpp
queue.o: queue.cpp queue.hpp
rw.o: rw.cpp rw.hpp variable.hpp
negativeCommunity.o: negativeCommunity.cpp circuits.hpp circle_cancel.hpp variable.hpp negativeCommunity.hpp
circle_cancel.o: circle_cancel.cpp variable.hpp circle_cancel.hpp

%.o:%.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) pushRelabel testmain $(OBJECTS) testmain.o