#
# Makefile for Binary Search Tree
#
# Homework 4 (fall 2019)
#

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11


Bst: bst.o BinarySearchTree.o
	${CXX} -o $@ $^

test: test.o BinarySearchTree.o
	${CXX} -o $@ $^

%.o: %.cpp $(shell echo *.h)
	${CXX} ${CXXFLAGS} -c $<

clean:
	rm -rf bst *.o *.dSYM


