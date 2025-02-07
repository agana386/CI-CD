CC	=	gcc
FLAGS	=	-Wall -Werror -Wextra -std=c11
LFLAGS	=	-lcheck	-lm	-lpthread
RFLAGS	=	-fprofile-arcs	-ftest-coverage

all: clean s21_grep linter test_grep

s21_grep:
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

clean:
	rm -rf s21_grep *.cfg log.txt 

test_grep:
	chmod +x script.sh
	./script.sh

linter:
	cp ../../materials/linters/.clang-format
	clang-format -i ./s21_cat.c 
	clang-format -i ./s21_cat.h
	rm -rf .clang-format

docker:
	sh ../../materials/build/run.sh

# valgrind:
# 	valgrind -q --leak-check=yes ./s21_grep -e "abc" patterns.txt 

# sanitize: clean sanitizer test_grep

# sanitizer:
# 	$(CC) $(FLAGS) -fsanitize=address	-g s21_grep.c -o s21_grep