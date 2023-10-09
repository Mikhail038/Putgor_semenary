
CXX_COMPILER_AND_FLAGS = g++ -Wall -Wextra -fsanitize=address

all:do

do: my_cp.o read_write_funcs.o
	$(CXX_COMPILER_AND_FLAGS) my_cp.o read_write_funcs.o -o my_cp

my_cp.o: my_cp.cpp
	$(CXX_COMPILER_AND_FLAGS) -c -o my_cp.o my_cp.cpp

read_write_funcs.o: read_write_funcs.cpp
	$(CXX_COMPILER_AND_FLAGS) -c -o read_write_funcs.o read_write_funcs.cpp
