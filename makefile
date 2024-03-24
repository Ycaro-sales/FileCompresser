build:
	gcc ./src/main.c ./src/utils.c ./src/core/compress.c ./src/core/decompress.c ./src/libs/file_functions.c ./src/libs/structs/huffmantree.c -o bin/huffman -g 

clean:
	rm -f bin/*

run:
	gdb ./bin/huffman

