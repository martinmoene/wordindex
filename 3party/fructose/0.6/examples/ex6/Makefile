all: ex6

ex6.cpp: ex6.h
	../../generator ex6.h >ex6.cpp

ex6: ex6.h ex6.cpp
	g++ -o ex6 -I../../include -I../../../tclap-1.1.0/include ex6.cpp
