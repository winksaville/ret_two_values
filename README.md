Investigate returning two parameters as a structure

Executive summary: The resulting executables are faster when
returning a struct with two integers compared to returning
one integer and returning the other integer via a parameter
which is a pointer to int.  For 32bit compiles I do need to
use the -freq-struct-return option, otherwise returning the
struct is about 1.3x slower.

Another interesting fact is gcc is faster than clang in this
test, on other tests I've done clang tends to be faster.


gcc 32bit is about 1.23x faster and 64bit is 1.77x faster.
```
$ make clean CC=gcc ; make run CC=gcc M=32 loops=1000000000
rm -f test test.o test.gcc.txt
gcc -x c -O2 -m32 -Wall -std=c11 -freg-struct-return -o test.o -c test.c
objdump -d test.o > test.gcc.txt
gcc -m32 -Wall -std=c11 -freg-struct-return test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 8713374435 = 3649692927135009 - 3649684213760574 status=0 v=1000000000 ok=0
result_ret ticks       = 7071175122 = 3649699998353364 - 3649692927178242 status=0 v=1000000000 ok=0
wink@wink-desktop:~/prgs/ret_two_values (master)

$ make clean CC=gcc ; make run CC=gcc M=64 loops=1000000000
rm -f test test.o test.gcc.txt
gcc -x c -O2 -m64 -Wall -std=c11 -o test.o -c test.c
objdump -d test.o > test.gcc.txt
gcc -m64 -Wall -std=c11 test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 8355602760 = 3649732781607612 - 3649724426004852 status=0 v=1000000000 ok=0
result_ret ticks       = 4694904246 = 3649737476542146 - 3649732781637900 status=0 v=1000000000 ok=0
```


Clang has about the same performance difference but is slower
than gcc. 32bit is about 1.18x faster and 64bit 1.55x faster.
```
$ make clean CC=clang ; make run CC=clang M=32 loops=1000000000
rm -f test test.o test.clang.txt
clang -x c -O2 -m32 -Wall -std=c11 -freg-struct-return -o test.o -c test.c
objdump -d test.o > test.clang.txt
clang -m32 -Wall -std=c11 -freg-struct-return test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 10030849530 = 3650596126110084 - 3650586095260554 status=0 v=1000000000 ok=0
result_ret ticks       = 8519094249 = 3650604645234738 - 3650596126140489 status=0 v=1000000000 ok=0

$ make clean CC=clang ; make run CC=clang M=64 loops=1000000000
rm -f test test.o test.clang.txt
clang -x c -O2 -m64 -Wall -std=c11 -o test.o -c test.c
objdump -d test.o > test.clang.txt
clang -m64 -Wall -std=c11 test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 8635790907 = 3650847973680516 - 3650839337889609 status=0 v=1000000000 ok=0
result_ret ticks       = 5549842284 = 3650853523554384 - 3650847973712100 status=0 v=1000000000 ok=0
```
