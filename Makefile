CC = gcc
CFLAGS = -Wall -g -O0 -std=c11 -pthread -lm
EXECUTABLE = df
SOURCES = df.c
HEADERS = df.h

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(CFLAGS) -o $@

clean:
	\rm *~ df
