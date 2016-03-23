Investigate returning two parameters as a structure

Executive summary: The resulting executables are faster when
returning a struct with two integers compared to returning
one integer and returning the other integer via a parameter
which is a pointer to int.  For 32bit compiles I do need to
use the -freq-struct-return option, otherwise returning the
struct is about 1.3x slower.

Another interesting fact is gcc is faster than clang on the
the 32bit result_ret (1.2x) and 64bit status_ret (1.04x),
on most other tests I've done clang tends to be faster.


gcc 32bit is about 1.23x faster and 64bit is 1.5x faster.
```
$ make clean CC=gcc ; make run CC=gcc loops=1000000000 M=32
rm -f test test.o test.gcc-64.txt
gcc -x c -O2 -m32 -Wall -std=c11 -freg-struct-return -o test.o -c test.c
objdump -d test.o > test.gcc.txt
gcc -m32 -Wall -std=c11 -freg-struct-return test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 8642705184 = 4109228804021475 - 4109220161316291 status=0 v=1000000000 ok=0
result_ret ticks       = 7040149206 = 4109235844213311 - 4109228804064105 status=0 v=1000000000 ok=0

$ make clean CC=gcc ; make run CC=gcc loops=1000000000 M=64
rm -f test test.o test.gcc-64.txt
gcc -x c -O2 -m64 -Wall -std=c11 -o test.o -c test.c
objdump -d test.o > test.gcc.txt
gcc -m64 -Wall -std=c11 test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 8281407219 = 4109714613009933 - 4109706331602714 status=0 v=1000000000 ok=0
result_ret ticks       = 5521354368 = 4109720134405305 - 4109714613050937 status=0 v=1000000000 ok=0
```


Clang has about the same performance difference but is slower
than gcc. 32bit is same speed and 64bit 1.55x faster.
```
$ make clean CC=clang ; make run CC=clang loops=1000000000 M=32
rm -f test test.o test.clang-64.txt
clang -x c -O2 -m32 -Wall -std=c11 -freg-struct-return -o test.o -c test.c
objdump -d test.o > test.clang.txt
clang -m32 -Wall -std=c11 -freg-struct-return test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 8643692922 = 4110336666619998 - 4110328022927076 status=0 v=1000000000 ok=0
result_ret ticks       = 8476670163 = 4110345143331225 - 4110336666661062 status=0 v=1000000000 ok=0

$ make clean CC=clang ; make run CC=clang loops=1000000000 M=64
rm -f test test.o test.clang-64.txt
clang -x c -O2 -m64 -Wall -std=c11 -o test.o -c test.c
objdump -d test.o > test.clang.txt
clang -m64 -Wall -std=c11 test.o -o test
./test 1000000000
warming up loops=1000000000
warm up done
status_ret ticks       = 8596154424 = 4110551132595744 - 4110542536441320 status=0 v=1000000000 ok=0
result_ret ticks       = 5516450235 = 4110556649076690 - 4110551132626455 status=0 v=1000000000 ok=0
```
