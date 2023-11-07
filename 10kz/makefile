
TSK = father_stretch_my_hands
CXX_COMPILER_AND_FLAGS = g++ -g -ggdb3 -Wall -Wextra -fsanitize=address

all:do

do: $(TSK).o monitor.o
	$(CXX_COMPILER_AND_FLAGS) $(TSK).o monitor.o -o $(TSK)

TSK.o: $(TSK).cpp
	$(CXX_COMPILER_AND_FLAGS) -c -o $(TSK).o $(TSK).cpp

monitor.o: monitor.cpp
	$(CXX_COMPILER_AND_FLAGS) -c -o monitor.o monitor.cpp
