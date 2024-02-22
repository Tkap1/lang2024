#include <stdint.h>
#include <math.h>
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t b8;
typedef uint32_t b32;
void* memcpy(void* restrict dest, const void* restrict src, size_t n);
typedef struct s_a
{
	int a_member;
} s_a;
typedef struct s_b
{
	s_a b_member;
} s_b;
typedef struct s_c
{
	s_b c_member;
} s_c;
typedef struct s_d
{
	s_c d_member;
} s_d;
void foo(void)
{
	s_d d = {0};
	d.d_member.c_member.b_member.a_member;
}
