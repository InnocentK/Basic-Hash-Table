CXX = g++
FLAGS = -gstabs+

Hash = HashTable
Probe = ProbeHashTable
Chain = ChainHashTable
TARGET = $(Chain).o $(Probe).o

all: proj5.out test1.out test2.out test3.out

test1.out: $(TARGET) getCurrentCPU.cpp test1.cpp
	$(CXX) $(FLAGS) $(TARGET) test1.cpp getCurrentCPU.cpp -o test1.out

test2.out: $(TARGET) getCurrentCPU.cpp words.h test2.cpp
	$(CXX) $(FLAGS) $(TARGET) test2.cpp getCurrentCPU.cpp words.h -o test2.out

test3.out: $(TARGET) getCurrentCPU.cpp test3.cpp
	$(CXX) $(FLAGS) $(TARGET) test3.cpp getCurrentCPU.cpp -o test3.out

proj5.out: $(TARGET) Driver.cpp
	$(CXX) $(FLAGS) $(TARGET) Driver.cpp -o proj5.out

$(Probe).o: $(Hash).h $(Probe).h $(Probe).cpp
	$(CXX) $(FLAGS) -c $(Probe).cpp

$(Chain).o: $(Hash).h $(Chain).h $(Chain).cpp
	$(CXX) $(FLAGS) -c $(Chain).cpp

run:
	./proj5.out
val:
	valgrind ./proj5.out
leak:
	valgrind --leak-check=full ./proj5.out

clean:
	$(RM) *~
	$(RM) *.o

test1:
	./test1.out
val1:
	valgrind ./test1.out

test2:
	./test2.out
val2:
	valgrind ./test2.out

test3:
	./test3.out
val3:
	valgrind ./test3.out
