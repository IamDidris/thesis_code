mainmake: parser.c driver.c
	gcc -Wall -c driver.c
	gcc -Wall -c parser.c
	gcc -Wall -c Dkeytoktab.c
	gcc -Wall -c keytoktab.c
	gcc -Wall -c Dlexer.c
	gcc -Wall -c lexer.c
	gcc -Wall -c Dsymtab.c
	gcc -Wall -c symtab.c
	#gcc -Wall -o tkeytoktab Dkeytoktab.c keytoktab.c
	#gcc -Wall -o parser driver.c parser.c keytoktab.c lexer.c
	#gcc -Wall -o tlexer Dlexer.c lexer.c keytoktab.c
	#gcc -Wall -o tsymtab Dsymtab.c symtab.c keytoktab.c
	gcc -Wall -o parser driver.c parser.c keytoktab.c lexer.c symtab.c