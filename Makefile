default:
	gcc-13 -o ringmaster src/parser.c src/tokenizer.c src/subjects.c ringmaster.c -I include

run:
	gcc-13 -o ringmaster src/parser.c src/tokenizer.c src/subjects.c ringmaster.c -I include; ./ringmaster

grade:
	python3 test/grader.py ./ringmaster test-cases