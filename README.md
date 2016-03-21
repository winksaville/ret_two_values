Investigating returning two parameters as a structure

Executive summary: Returning structs with two integers the
size of pointers is slower on 32bit and faster on 64bit.


1.3x slower on 32
1.5x faster on 64

32bit on x86_64:

$ make clean CC=gcc ;make run CC=gcc M=32 loops=1000000000
rm -f test test.o test.gcc.txt
gcc -x c -O2 -m32 -Wall -std=c11 -o test.o -c test.c
objdump -d test.o > test.gcc.txt
gcc -m32 -Wall -std=c11 test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 9683377218 = 3613666342315653 - 3613656658938435 status=0 v=1000000000 ok=0
result_ret ticks       = 12323369184 = 3613678665729285 - 3613666342360101 status=0 v=1000000000 ok=0
wink@wink-desktop:~/prgs/ret_two_values (master)


64bit on x86_64:

gcc -x c -O2 -m64 -Wall -std=c11 -o test.o -c test.c
objdump -d test.o > test.gcc.txt
gcc -m64 -Wall -std=c11 test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 8319044616 = 3613911036197145 - 3613902717152529 status=0 v=1000000000 ok=0
result_ret ticks       = 5621469786 = 3613916657700633 - 3613911036230847 status=0 v=1000000000 ok=0
wink@wink-desktop:~/prgs/ret_two_values (master)


Clang has about the same performance difference but is slower
on 32bit and about same speed on 64bit

1.5x slower on 32
1.5x faster on 64

$ make clean CC=clang ;make run CC=clang M=32 loops=1000000000
rm -f test test.o test.clang.txt
clang -x c -O2 -m32 -Wall -std=c11 -o test.o -c test.c
objdump -d test.o > test.clang.txt
clang -m32 -Wall -std=c11 test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 12091729050 = 3613730057214339 - 3613717965485289 status=0 v=1000000000 ok=0
result_ret ticks       = 18114682344 = 3613748171939160 - 3613730057256816 status=0 v=1000000000 ok=0


$ make clean CC=clang ;make run CC=clang M=64 loops=1000000000
rm -f test test.o test.clang.txt
clang -x c -O2 -m64 -Wall -std=c11 -o test.o -c test.c
objdump -d test.o > test.clang.txt
clang -m64 -Wall -std=c11 test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 8610961143 = 3613965911366010 - 3613957300404867 status=0 v=1000000000 ok=0
result_ret ticks       = 5524041423 = 3613971435432897 - 3613965911391474 status=0 v=1000000000 ok=0
