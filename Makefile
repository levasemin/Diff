CC = g++

CFLAGS= -g -std=c++14 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat -Wc++11-compat -Wc++14-compat -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlarger-than=8192 -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs
BuildF = obj/

all: clean main.exe

main.exe :	 $(BuildF)main.o $(BuildF)differentiate.o $(BuildF)simplifier.o $(BuildF)make_graph.o $(BuildF)read_graph.o $(BuildF)dump_graph.o $(BuildF)stack.o $(BuildF)struct.o $(BuildF)open.o 
	$(CC) -g $(BuildF)main.o $(BuildF)differentiate.o $(BuildF)simplifier.o $(BuildF)make_graph.o $(BuildF)read_graph.o $(BuildF)dump_graph.o $(BuildF)stack.o $(BuildF)struct.o $(BuildF)open.o -o main.exe

$(BuildF)main.o : main.cpp | obj
	$(CC) -c $(CFLAGS) main.cpp -o $(BuildF)main.o

$(BuildF)make_graph.o : make_graph.cpp | obj
	$(CC) -c $(CFLAGS) make_graph.cpp -o $(BuildF)make_graph.o

$(BuildF)read_graph.o : read_graph.cpp | obj
	$(CC) -c $(CFLAGS) read_graph.cpp -o $(BuildF)read_graph.o

$(BuildF)differentiate.o : differentiate.cpp | obj
	$(CC) -c $(CFLAGS) differentiate.cpp -o $(BuildF)differentiate.o

$(BuildF)simplifier.o : main.cpp | obj
	$(CC) -c $(CFLAGS) simplifier.cpp -o $(BuildF)simplifier.o

$(BuildF)dump_graph.o : dump_graph.cpp | obj
	$(CC) -c $(CFLAGS) dump_graph.cpp -o $(BuildF)dump_graph.o

$(BuildF)open.o : open.cpp | obj
	$(CC) -c $(CFLAGS) open.cpp -o $(BuildF)open.o

$(BuildF)struct.o : struct.cpp | obj
	$(CC) -c $(CFLAGS) struct.cpp -o $(BuildF)struct.o

$(BuildF)stack.o : stack.cpp | obj
	$(CC) -c $(CFLAGS) stack.cpp -o $(BuildF)stack.o

obj:
	mkdir $@

clean:
	del /q /s obj