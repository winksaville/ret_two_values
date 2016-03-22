# Copyright 2016 wink saville
#
# licensed under the apache license, version 2.0 (the "license");
# you may not use this file except in compliance with the license.
# you may obtain a copy of the license at
#
#     http://www.apache.org/licenses/license-2.0
#
# unless required by applicable law or agreed to in writing, software
# distributed under the license is distributed on an "as is" basis,
# without warranties or conditions of any kind, either express or implied.
# see the license for the specific language governing permissions and
# limitations under the license.

CC = gcc
O = 2
X = c
STD = c11
M = 64



CFLAGS = -m${M} -Wall -std=${STD}

# If 32bit compiler add req-sturct-return
# this isn't needed for 64 as its the default
# and clang complains.
ifeq (${M},32)
CFLAGS += -freg-struct-return
endif

all: test

test.o: test.c
	$(CC) -x $(X) -O$(O) $(CFLAGS) -o test.o -c test.c
	objdump -d test.o > test.${CC}.txt

test: test.o
	$(CC) $(CFLAGS) test.o -o test

run: test
	./test ${loops}

clean:
	rm -f test test.o test.${CC}.txt

cleanall:
	rm -f test test.o test.gcc.txt test.clang.txt
