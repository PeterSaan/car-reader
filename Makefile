COMPILER := g++
FLAGS := -std=c++2b -Wall -Wextra -O2
SRC := src/main.cpp src/person.cpp
OUT := car-reader.out

all:
	$(COMPILER) $(FLAGS) $(SRC) -o $(OUT)
