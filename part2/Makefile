CC = g++
CFLAGS = -Wall -g -O0 -std=c++11 -pthread
EXECUTABLE = df
SOURCES = drinking.cpp
HEADERS = drinking.hpp

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	\rm *~ df
