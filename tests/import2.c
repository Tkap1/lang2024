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
typedef struct s_v2
{
	float x;
	float y;
} s_v2;
typedef struct s_player
{
	s_v2 pos;
} s_player;
void foo(void)
{
	s_player player = {0};
	player.pos.x = 1;
}
