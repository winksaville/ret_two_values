/*
 * Copyright 2016 Wink Saville
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uintptr_t uint_t;
typedef unsigned long long u64_t;
typedef unsigned int u32_t;

static inline u64_t rdtsc(void) {
  u32_t lo, hi;
  __asm__ volatile (
      "rdtsc\n\t"
      :"=a"(lo), "=d"(hi));
  return ((u64_t)hi << 32) | (u64_t)lo;
}

typedef struct {
  uint_t v;
  uint_t v2;
} pair_t;

typedef struct  {
  uint_t status;
  uint_t v;
} result_t;

volatile uint_t inc_v = 0;

__attribute__((__noinline__))
uint_t status_ret(uint_t* pv) {
  uint_t status;

  *pv += inc_v + 1;
  if ((*pv & 0x1) == 0) {
    status = 0;
  } else {
    status = 0x123;
  }
  return status;
}

__attribute__((__noinline__))
result_t result_ret(uint_t v) {
  result_t r;

  r.v = v + inc_v + 1;
  if ((r.v & 0x1) == 0) {
    r.status = 0;
  } else {
    r.status = 0x123;
  }

  return r;
}


int main(int argc, char* argv[]) {
  u64_t start;
  u64_t stop;  
  u32_t loops;
  u32_t ok;
  uint_t v;
  result_t rt;
  uint_t status;

  if (argc < 2) {
    printf("Error expecting loops parammeter\n");
    return 1;
  }
  loops = atol(argv[1]);

  printf("warming up loops=%d\n", loops);
  v = 0;
  ok = 0;
  status = 0;
  for (u64_t i = 0; i < loops; i++) {
    status = status_ret(&v);
    if (status == 0) {
      ok += 1;
    } else {
      ok -= 1;
    }
  }
  printf("warm up done\n");

  v = 0;
  ok = 0;
  status = 0;
  start = rdtsc();
  for (u64_t i = 0; i < loops; i++) {
    status = status_ret(&v);
    if (status == 0) {
      ok += 1;
    } else {
      ok -= 1;
    }
  }
  stop = rdtsc();
  printf("status_ret ticks       = %llu = %llu - %llu status=%d v=%d ok=%d\n",
      stop - start, stop, start, (int)status, (int)v, ok);

  rt.status = 0;
  rt.v = 0;
  ok = 0;
  start = rdtsc();
  for (u32_t i = 0; i < loops; i++) {
    rt = result_ret(rt.v);
    if (rt.status == 0) {
      ok += 1;
    } else {
      ok -= 1;
    }
  }
  stop = rdtsc();
  printf("result_ret ticks       = %llu = %llu - %llu status=%d v=%d ok=%d\n",
      stop - start, stop, start, (int)rt.status, (int)rt.v, ok);

  return 0;
}
