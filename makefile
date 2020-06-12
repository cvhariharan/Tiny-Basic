parser: src/parser.c src/lexer.c src/symboltable.c
	gcc src/parser.c src/lexer.c src/symboltable.c -g -o build/parser

