SANITIZE = -fsanitize=address -fsanitize=undefined -fsanitize=shift -fsanitize=shift-exponent -fsanitize=shift-base -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -fsanitize=bounds -fsanitize=bounds-strict -fsanitize=bool -fsanitize=enum

aes:
	make aes128
	make aes192
	make aes256
	rm -rf *.o

aes128: main128.o utile128.o functions128.o tests128.o files128.o p_bmp128.o entropie128.o
	gcc -Wall -g -o $@ $^ -O3 -lm
	
aes192: main192.o utile192.o functions192.o tests192.o files192.o p_bmp192.o entropie192.o
	gcc -Wall -g -o $@ $^ -O3 -lm 
	
aes256: main256.o utile256.o functions256.o tests256.o files256.o p_bmp256.o entropie256.o
	gcc -Wall -g -o $@ $^ -O3 -lm 
	
%128.o: %.c
	gcc -Wall -g -c $< -DAES128 -o $@

%192.o: %.c
	gcc -Wall -g -c $< -DAES192 -o $@

%256.o: %.c
	gcc -Wall -g -c $< -DAES256 -o $@

entropie: entropie.c
	gcc -Wall -g -o $@ $^ -O3 -lm

clear:
	rm -rf aes128
	rm -rf aes192
	rm -rf aes256
	rm -rf entropie
	rm -rf *.o
	find .. -name ".DS_Store" -delete

clean:
	rm -rf *.o
	find .. -name ".DS_Store" -delete