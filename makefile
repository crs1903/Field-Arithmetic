all : out

out : 
	gcc -o test field.c stdarith.c test.c -I.
	
clean:
	rm test

