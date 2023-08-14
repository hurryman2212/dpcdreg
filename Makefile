all:
	gcc -o dpcdreg-read dpcdreg-read.c -Wall -Wextra -Werror

clean:
	rm -f dpcdreg-read
