#!/bin/bash
#zad3
echo "STATIC" > raport.txt
make zad3a_static
make static_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n"  >> raport.txt
echo "DYNAMIC" >> raport.txt
make zad3a_dynamic
make dynamic_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n" >> raport.txt
echo "SHARED" >> raport.txt
make zad3a_shared
make shared_tests >> raport.txt
make clean
