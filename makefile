build:
	gcc -Wall ./src/main.c ./src/utils.c ./src/core/compress.h ./src/core/decompress.h -o main -g

clean:
	rm -

run:
	gdb ./bin/huffman

