LIBS  = $(shell llvm-config --libs)
LIBS += -lclang-cpp

CXXFLAGS  = -Wall -Wextra -Werror
CXXFLAGS += -O0
CXXFLAGS += -g
CXXFLAGS += -std=c++17

SAN ?= 1
ifeq ($(SAN),1)
FLAGS = -fsanitize=address -fsanitize=leak -fsanitize=undefined
endif

run: main
	./$^

main: main.o
	$(CXX) -o $@ $^ $(LIBS) $(FLAGS)

%.o: %.cc
	$(CXX) -o $@ -c $^ $(CXXFLAGS) $(FLAGS)

fmt:
	clang-format -i *.cc *.h

clean:
	$(RM) main *.o
