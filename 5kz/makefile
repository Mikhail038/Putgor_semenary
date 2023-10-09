
TSK = my_wc
CXX_COMPILER_AND_FLAGS = g++ -g -ggdb3 -Wall -Wextra -fsanitize=address

all:do

do: $(TSK).o
	$(CXX_COMPILER_AND_FLAGS) $(TSK).o -o $(TSK)

my_cp.o: $(TSK).cpp
	$(CXX_COMPILER_AND_FLAGS) -c -o $(TSK).o $(TSK).cpp

read_write_funcs.o: $(TSK).cpp
	$(CXX_COMPILER_AND_FLAGS) -c -o  $(TSK).cpp
