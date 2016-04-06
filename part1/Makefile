CC = gcc
CFLAGS = -Wall -g -O0 -std=c11 -pthread
EXECUTABLE = df
SOURCES = df.c
HEADERS = df.h

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	\rm *~ df
