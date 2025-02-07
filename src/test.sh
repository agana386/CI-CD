#!/bin/sh

cd src/cat/
gcc -Wall -Wextra -Werror s21_cat.c -o s21_cat
cd ../grep/
gcc -Wall -Wextra -Werror s21_grep.c -o s21_grep
cd ../

error_check() {
    if [[ $1 != 0 ]]; then
        echo "ERROR"
        exit 1   #покажет ошибку
    fi
}
error_flag=0
for i in n b s E e t
do
    echo "TEST: $i"
    cat/s21_cat -$i test.txt > s21_cat_result
    cat -$i test.txt > cat_result
    diff -s s21_cat_result cat_result
    if [[ $(diff -s s21_cat_result cat_result) != 'Files s21_cat_result and cat_result are identical' ]]; then
        error_flag=1
        break
    fi
done
rm -rf s21_cat_result cat_result
error_check $error_flag
for i in e i v c l n
do
    echo "TEST: $i"
    grep/s21_grep -$i them test.txt > s21_grep_result
    grep -$i them test.txt > grep_result
    diff -s s21_grep_result grep_result
    if [[ $(diff -s s21_grep_result grep_result) != 'Files s21_grep_result and grep_result are identical' ]]; then
        error_flag=1
        break
    fi
done
rm -rf s21_grep_result grep_result
error_check $error_flag