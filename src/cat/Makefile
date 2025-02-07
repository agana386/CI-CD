all: clean s21_cat test

s21_cat:
	gcc -Wall -Werror -Wextra s21_cat.c -o s21_cat

test:
	chmod +x cat.sh
	./cat.sh

clean:
	rm -rf s21_cat *.o

linter:
	cppcheck *.h *.c
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c *.h
	rm -rf CPPLINT.cfg

