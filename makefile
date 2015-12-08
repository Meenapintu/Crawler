all:	run

run:
	g++-5 -std=c++11 mem.cc  -o mem -lcurl -pthread -pedantic  -lm  -w

