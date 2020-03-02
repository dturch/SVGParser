CFLAGS = -Wall -g -std=c11
BIN = bin/
INC = include/
SRC = src/
XML_PATH = /usr/include/libxml2

# Standard common makefile
parser: $(BIN)libsvgparse.so

$(BIN)libsvgparse.so: $(BIN)SVGParser.o $(BIN)LinkedListAPI.o
	gcc -shared -o $(BIN)libsvgparse.so $(BIN)SVGParser.o $(BIN)LinkedListAPI.o -lxml2 -lm

$(BIN)SVGParser.o: $(SRC)SVGParser.c $(INC)LinkedListAPI.h $(INC)SVGParser.h
	gcc -c -fpic $(CFLAGS) -I$(XML_PATH) -I$(INC) $(SRC)SVGParser.c -o $(BIN)SVGParser.o

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	gcc -c -fpic $(CFLAGS) -I$(INC) $(SRC)LinkedListAPI.c -o $(BIN)LinkedListAPI.o

test: $(INC)SVGParser.h $(SRC)SVGParser.c $(INC)helper.h $(INC) $(INC)LinkedListAPI.h $(SRC)LinkedListAPI.c $(SRC)main.c
	gcc $(CFLAGS) -I$(INC) `xml2-config --cflags --libs` -I$(XML_PATH) $(SRC)SVGParser.c $(SRC)LinkedListAPI.c $(SRC)main.c -o main -lxml2 -lm

clean:
	rm $(BIN)*.o $(BIN)*.so
