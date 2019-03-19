OUT = project3
CFLAGS = -std=gnu99
SOURCES = lab3.c link.c my_malloc.c file_read.c helpers.c
LIBS = 

p=0
f=0

default:
	gcc -Wall -g $(CFLAGS) $(SOURCES) $(LIBS) -o $(OUT)


clean:
	rm $(OUT)

run:
	./$(OUT) $p testfiles/input/$f.txt

