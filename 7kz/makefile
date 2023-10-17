
TSK = trip
LIB = sys_funcs

CXX_COMPILER_AND_FLAGS = g++ -g -ggdb3 -Wall -Wextra -fsanitize=address

all:do

do: $(TSK).o $(LIB).o
	$(CXX_COMPILER_AND_FLAGS) $(TSK).o $(LIB).o -o $(TSK)

$(TSK).o: $(TSK).cpp
	$(CXX_COMPILER_AND_FLAGS) -c -o $(TSK).o $(TSK).cpp

$(LIB).o: $(LIB).cpp
	$(CXX_COMPILER_AND_FLAGS) -c -o $(LIB).o $(LIB).cpp
