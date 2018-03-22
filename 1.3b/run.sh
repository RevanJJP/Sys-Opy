#!/bin/bash

echo "Optimalization 0" > raport.txt
echo "STATIC " >> raport.txt
make staticO0
make static_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n"  >> raport.txt
echo "DYNAMIC " >> raport.txt
make dynamicO0
make dynamic_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n" >> raport.txt
echo "SHARED " >> raport.txt
make sharedO0
make shared_tests >> raport.txt
make clean

echo "Optimalization 1" >> raport.txt
echo "STATIC " >> raport.txt
make staticO1
make static_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n"  >> raport.txt
echo "DYNAMIC " >> raport.txt
make dynamicO1
make dynamic_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n" >> raport.txt
echo "SHARED " >> raport.txt
make sharedO1
make shared_tests >> raport.txt
make clean

echo "Optimalization 2" >> raport.txt
echo "STATIC " >> raport.txt
make staticO2
make static_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n"  >> raport.txt
echo "DYNAMIC " >> raport.txt
make dynamicO2
make dynamic_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n" >> raport.txt
echo "SHARED " >> raport.txt
make sharedO2
make shared_tests >> raport.txt
make clean

echo "Optimalization 3" >> raport.txt
echo "STATIC " >> raport.txt
make staticO3
make static_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n"  >> raport.txt
echo "DYNAMIC " >> raport.txt
make dynamicO3
make dynamic_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n" >> raport.txt
echo "SHARED " >> raport.txt
make sharedO3
make shared_tests >> raport.txt
make clean

echo "Optimalization s" >> raport.txt
echo "STATIC " >> raport.txt
make staticOs
make static_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n"  >> raport.txt
echo "DYNAMIC " >> raport.txt
make dynamicOs
make dynamic_tests >> raport.txt
printf "\n" >> raport.txt
make clean

printf "\n" >> raport.txt
echo "SHARED " >> raport.txt
make sharedOs
make shared_tests >> raport.txt
make clean
