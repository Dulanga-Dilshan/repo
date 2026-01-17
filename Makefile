cc = gcc
target = bin/repo
flags = -Wall -static -Iinclude -Wextra

all : obj bin final

obj : 
	mkdir -p obj

bin : 
	mkdir -p bin

final : obj/fname.o obj/linkedlist.o obj/os.o obj/token.o obj/main.o
	$(cc) $(flags) -o $(target) obj/fname.o obj/linkedlist.o obj/os.o obj/token.o obj/main.o

obj/fname.o :  src/fname.c include/fname.h
	$(cc) $(flags) -c src/fname.c -o obj/fname.o

obj/linkedlist.o :  src/linkedlist.c include/linkedlist.h
	$(cc) $(flags) -c src/linkedlist.c -o obj/linkedlist.o

obj/os.o :  src/os.c include/os.h
	$(cc) $(flags) -c os.c -o obj/os.o

obj/token.o :  src/token.c include/token.h
	$(cc) $(flags) -c src/token.c -o obj/token.o

obj/main.o :  src/main.c include/token.h include/os.h include/linkedlist.h include/fname.h
	$(cc) $(flags) -c src/main.c -o obj/main.o

clean:
	rm -rf obj/*.o

	