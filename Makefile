run:
	gcc-13 -o ringmaster src/parser.c src/tokenizer.c ringmaster.c -I include; ./ringmaster

default:
	gcc-13 -o ringmaster src/parser.c src/tokenizer.c ringmaster.c -I include