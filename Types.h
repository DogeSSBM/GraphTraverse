#pragma once
typedef FILE			File;
typedef time_t			Time;

typedef long int			lint;
typedef unsigned char		uchar;
typedef unsigned int		uint;
typedef unsigned long		ul;
typedef unsigned long long	ull;

typedef uint8_t			u8;
typedef uint16_t			u16;
typedef uint32_t			u32;
typedef uint64_t			u64;

typedef int8_t			i8;
typedef int16_t			i16;
typedef int32_t			i32;
typedef int64_t			i64;

#define I8MIN			INT8_MIN
#define I8MAX			INT8_MAX
#define I16MIN			INT16_MIN
#define I16MAX			INT16_MAX
#define I32MIN			INT32_MIN
#define I32MAX			INT32_MAX
#define I64MIN			INT64_MIN
#define I64MAX			INT64_MAX

#define U8MAX			UINT8_MAX
#define U16MAX			UINT16_MAX
#define U32MAX			UINT32_MAX
#define U64MAX			UINT64_MAX

#define ABS(n)			((n)<0? -(n):(n))
#define ZFLOOR(n)			((n)<0?    0:(n))

typedef u32				Ticks;
typedef SDL_Event			Event;
typedef SDL_Rect 			Rect;
typedef SDL_Color 		Color;
typedef SDL_Surface 		Img;
typedef SDL_Keycode 		Key;
typedef SDL_BlendMode		BlendMode;
#define BLEND_NONE		SDL_BLENDMODE_NONE
#define BLEND_ALPHA		SDL_BLENDMODE_BLEND
#define BLEND_ADD			SDL_BLENDMODE_ADD
#define BLEND_MOD			SDL_BLENDMODE_MOD

typedef enum{
	FULLSCREEN	=		SDL_WINDOW_FULLSCREEN,
	BORDERLESS	=		SDL_WINDOW_FULLSCREEN_DESKTOP,
	WINDOWED	=		0
}WindowMode;

typedef union{
	int arr[2];
	struct{
		int x;
		int y;
	};
	struct{
		int min;
		int max;
	};
	struct{
		int neg;
		int pos;
	};
}Coord, Range, Length, Offset;

static inline
bool sameCoord(const Coord pos1, const Coord pos2)
{
	return pos1.x == pos2.x && pos1.y == pos2.y;
}

typedef enum{
	DIR_U = 0b00,
	DIR_R = 0b01,
	DIR_D = 0b10,
	DIR_L = 0b11
}Direction;

Direction keyToDir(const SDL_Keycode key)
{
	switch(key){
		case SDLK_w:
		case SDLK_UP:
			return DIR_U;
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			return DIR_R;
			break;
		case SDLK_s:
		case SDLK_DOWN:
			return DIR_D;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			return DIR_L;
			break;
	}
	printf("SDL_Keycode (0x%0X) is not WASD or URDL\n", key);
	return -1;
}

#define dirROR(d)			(((d)+1)&0b11)
#define dirROL(d)			(((d)-1)&0b11)

//#define dirROR(d)			(((d)&1)<<1^((d)^1))
//#define dirROL(d)			(((d)^1)<<1^((d)^1))
#define dirINV(d)			((d)^0b10)
#define dirLR(d)			((d)&1)
#define dirUD(d)			(!((d)&1))
#define dirPOS(d)			((d)==DIR_R||(d)==DIR_D)
#define dirNEG(d)			((d)==DIR_L||(d)==DIR_U)

// Bounds n between min (inclusive) and max (exclusive)
static inline
int clamp(const int n, const int min, const int max)
{
	if(n < min)
		return min;
	if(n >= max)
		return max-1;
	return n;
}

// Returns true if n is between min (inclusive) and max (exclusive)
static inline
bool inBound(const int n, const int min, const int max)
{
	return n >= min && n < max;
}

static inline
int wrap(const int n, const int min, const int max)
{
	if(n<min)
		return max-1;
	if(n>=max)
		return min;
	return n;
}

static inline
Coord coordWrap(const Coord coord, const Range x, const Range y)
{
	const Coord ret = {
		wrap(coord.x, x.min, x.max),
		wrap(coord.y, y.min, y.max)
	};
	return ret;
}

static inline
Coord coordClamp(const Coord coord, const Range x, const Range y)
{
	const Coord ret = {
		clamp(coord.x, x.min, x.max),
		clamp(coord.y, y.min, y.max)
	};
	return ret;
}

static inline
Coord coordShift(const Coord coord, const Direction dir, const int units)
{
	Coord ret = coord;
	if(dirUD(dir)){
		ret.y += dirPOS(dir)? units : -units;
	}else{
		ret.x += dirPOS(dir)? units : -units;
	}
	return ret;
}

static inline
Coord coordShift2(const Coord coord, const Direction dir1, const Direction dir2, const int units)
{
	const Coord ret = coordShift(coordShift(coord, dir1, units), dir2, units);
	return ret;
}
