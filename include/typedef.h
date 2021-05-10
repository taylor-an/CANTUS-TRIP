

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_


#ifdef TRUE	
#undef TRUE 
#endif

#ifdef FALSE 
#undef FALSE
#endif

#ifdef true
#undef true 
#endif

#ifdef false 
#undef false
#endif

typedef enum {
	FALSE=0,
	TRUE,
}BOOL;

typedef enum {
	false=0,
	true=1,
}bool;

typedef signed char		S8;
typedef signed short	S16;
typedef signed int		S32;

typedef signed char		s8;
typedef signed short	s16;
typedef signed int		s32;

typedef unsigned char	U8;
typedef unsigned short	U16;
typedef unsigned int	U32;
typedef unsigned long long	U64;

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;
typedef unsigned long long	u64;

typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;


#define ALIGN4  __attribute__((__aligned__(4)))

#endif

