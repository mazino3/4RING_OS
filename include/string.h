/*
 * 4RING_OS License: MIT
 *
 * string.h
 *
 * Defines for the static inline string functions.
 * Due to the simplicity of implementation, I prefer that these functions in
 * most cases are always statically inline. If they need to be called
 * dynamically, I can easily integrate them into the code for that purpose,
 * even from libraries ...
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _STRING_H
#define _STRING_H

#include <typedef.h>
#include <io.h>

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

/*
 * I need here __inl_unusd_: the attributes always_inline and unused.
 * The need for unused is to avoid warning: [-Wunused-function] because when
 * C files include this header and do not use all of them, static declarations
 * of functions reasonably produce these warnings, but static decl. preserves
 * many binary copies of functions in each compiled file where will be included.
 */

/// Copying functions

// String Copy
/*
 * The function stcpy  copies the string pointed to, by src to dest.
 * It returns a pointer to the destination (dest).
 */
static __inl_unusd_ char * strcpy(char *dest, const char *src)
{
  _esi(src);
  _edi(dest);
  __asm{
	cld
	loop: lodsb
	stosb
	test al,al
	jne loop
  }
  return dest;
}

// Bounded String Copy
/*
 * The function strncpy  copies up to n characters from the string pointed to,
 * by src to dest. In a case where the length of src is less than that of n,
 * the remainder of dest will be padded with null bytes. It returns a pointer
 * to the destination (dest).
 */
static __inl_unusd_ char * strncpy(char *dest, const char *src, size_t n)
{
  _esi(src);
  _edi(dest);
  _ecx(n);
 __asm{
	cld
	loop: dec ecx
	js exit
	lodsb
	stosb
	test al,al
	jne loop
	rep
	stosb
	exit:
  }
return dest;
}

// Copy Memory Block
/*
 * The  function memcpy copies n characters from memory area src to memory
 * area dest. It returns a pointer to destination, which is str1.
 */
static __inl_unusd_ void * memcpy(void *dest, const void *src, size_t n)
{
  _esi(src);
  _edi(dest);
  _ecx(n);
  __asm{
	cld
	rep movsb
	mov eax, dest
  }
}

// Copy for overlapping memory blocks
/*
 * The function memmove copies n characters from src to dest, but for
 * overlapping memory blocks, memmove() is a safer approach than memcpy().
 * It returns a pointer to the destination, which is dest.
 */
static __inl_unusd_ void * memmove(void * dest, const void * src, int n)
{
	if (dest < src) {
		_esi(src);
		_edi(dest);
		_ecx(n);
		__asm{
		  cld
		  rep movsb
		  mov eax, dest
		}
	} else {
		_esi(n-1+(const char *)src);
		_edi(n-1+(char *)dest);
		_ecx(n);
		__asm{
		  std
		  rep movsb
		  mov eax, dest
		}
	}
}

/// Concatenation functions

// String Concatenation
/*
 * The function char strcat appends the string pointed to by src
 * to the end of the string pointed to by dest. It returns a pointer
 * to the destination (dest).
 */
static __inl_unusd_ char * strcat(char *dest, const char *src)
{
  _esi(src);
  _edi(dest);
__asm__{
	mov eax,0
	mov ecx, -1
	cld
	repne
	scasb
	dec edi
	loop: lodsb
	stosb
	test al,al
	jne loop
  }
return dest;
}

// Bounded String Concatenation
/*
 * The function char strncat appends the string pointed to by src
 * to the end of the string pointed to by dest up to n characters long.
 * In a case where the length of src is less than that of n, the remainder
 * of dest will be padded with null bytes.
 * It returns a pointer to the destination (dest).
 */
static __inl_unusd_ char * strncat(char *dest, const char *src, size_t n)
{
  _esi(src);
  _edi(dest);
  __asm__{
	mov eax,0
	mov ecx, -1
	cld
	repne
	scasb
	dec edi
	mov ecx, n
	loop: dec ecx
	js l2
	lodsb
	stosb
	test al,al
	jne loop
	l2: xor eax,eax
	stosb
  }
return dest;
}

/// Comparison functions

// String Compare
/*
 * The function strcmp compares the string pointed to, by str1 to the
 * string pointed to by str2. It returns a value:
 * if Return value < 0 then it indicates str1 is less than str2.
 * if Return value > 0 then it indicates str2 is less than str1.
 * if Return value = 0 then it indicates str1 is equal to str2.
 */
static __inl_unusd_ int strcmp(const char *str1, const char *str2)
{
  _edi(str1);
  _esi(str2);
  __asm{
	cld
	l1: lodsb
	scasb
	jne l2
	test al,al
	jne l1
	xor eax,eax
	jmp l3
	l2: mov eax, 1
	jl l3
	neg eax
	l3: /* By default __asm{... returns eax! */
  }
  /* No need for explicit return... By default function returns eax! */
}

// Bounded String Compare
/*
 * The function strncmp compares at most the first n bytes of str1 and str2.
 * It returns a value:
 * if Return value < 0 then it indicates str1 is less than str2.
 * if Return value > 0 then it indicates str2 is less than str1.
 * if Return value = 0 then it indicates str1 is equal to str2.
 */
static __inl_unusd_ int strncmp(const char *str1, const char *str2, size_t n)
{
  _edi(str1);
  _esi(str2);
  __asm__{
	mov ecx, n
	cld
	l1: dec ecx
	js l2
	lodsb
	scasb
	jne l3
	test al,al
	jne l1
	l2: xor eax,eax
	jmp l4
	l3: mov eax, 1
	jl l4
	neg eax
	l4:
	}
}

// Compare Memory Blocks
/*
 * The function memcmp compares the first n bytes of memory area str1 and
 * memory area str2. It returns a value:
 * if Return value < 0 then it indicates str1 is less than str2.
 * if Return value > 0 then it indicates str2 is less than str1.
 * if Return value = 0 then it indicates str1 is equal to str2.
 */
static __inl_unusd_ size_t memcmp(const void *str1, const void *str2, size_t n)
{
  _edi(str1);
  _esi(str2);
  __asm{
	mov ecx, n
	cld
	xor eax, eax
	repne
	cmpsb
	je l1
	mov eax, 1
	jl l1
	neg eax
  	l1:
  }
}

// TODO:
//strcoll	String Compare Using Locale-Specific Collating Sequence
//strxfrm	Transform Locale-Specific String

/// Search functions

// Search String for Character
/*
 * The function char strchr  searches for the first occurrence of the
 * character c in the string pointed to by the argument str.
 * It returns a pointer to the first occurrence of the character c
 * in the string str, or NULL if the character is not found.
 */
static __inl_unusd_ char * strchr(const char *str, int c)
{
  _eax(c);
  _esi(str);
  __asm{
	mov ah, al
	l1: lodsb
	cmp al,ah
	je l2
	test al, al
	jne l1
	mov esi ,1
	l2: mov eax, esi
	dec eax
  }
}

// Search String in Reverse for Character
/*
 * The function strrchr  searches for the last occurrence of the
 * character c in the string pointed to by the argument str.
 * It returns a pointer to the last occurrence of the character
 * in the string, or NULL if the character is not found.
 */
static __inl_unusd_ char * strrchr(const char *str, int c)
{
  _eax(c);
  _esi(str);
  __asm{
	cld
  	mov ah, al
  	l1: lodsb
  	cmp al,ah
  	jne l2
	lea edx,[esi-1]
	l2: test al, al
  	jne l1
  	mov eax, edx
  }
}

// Search String for Initial Span of Characters in Set
/*
 * The function strspn  calculates the length of the initial segment
 * of str1 which consists entirely of characters in str2.
 * It returns the number of characters in the initial segment
 * of str1 which consist only of characters from str2.
 */
static __inl_unusd_ size_t strspn(const char *str1, const char *str2)
{
  _esi(str1);
  __asm__{
	mov eax,0
	mov ecx, -1
	cld
	mov edi, str2
	repne
	scasb
	not ecx
	dec ecx
	mov edx, ecx
	l1: lodsb
	test al, al
	je l2
	mov edi, str2
	mov ecx, edx
	repne
	scasb
	je l1
	l2: dec esi
	mov eax, esi
	sub eax, str1
  }
}

// Search String for Intial Span of Characters Not in Set
/*
 * The function strcspn calculates the length of the initial segment
 * of str1, which consists entirely of characters NOT in str2.
 * It returns the number of characters in the initial segment
 * of str1 which are not in the string str2.
 */
static __inl_unusd_ size_t strcspn(const char *str1, const char *str2)
{
  _esi(str1);
  __asm__{
	mov eax,0
	mov ecx, -1
	cld
	mov edi, str2
	repne
	scasb
	not ecx
	dec ecx
	mov edx, ecx
	l1: lodsb
	test al, al
	je l2
	mov edi, str2
	mov ecx, edx
	repne
	scasb
	jne l1
	l2: dec esi
	mov eax, esi
	sub eax, str1
  }
}

// Search String for One of a Set of Characters
/*
 * The function strpbrk finds the first character in the string str1 that
 * matches any character specified in str2. This does not include the
 * terminating null-characters. It returns a pointer to the character in str1
 * that matches one of the chars in str2, or NULL if no such charsr is found.
 */
static __inl_unusd_ char * strpbrk(const char *str1,const char *str2)
{
  _esi(str1);
  __asm{
	mov eax,0
	mov ecx, -1
	cld
	mov edi, str2
	repne
	scasb
	not ecx
	dec ecx
	mov edx, ecx
	l1: lodsb
	test al, al
	je l2
	mov edi, str2
	mov ecx, edx
	repne
	scasb
	jne l1
	l2:
  }
}

// Search String for Substring
/*
 * The function strstr finds the first occurrence of the substring str2
 * in the string str1. The terminating '\0' characters are not compared.
 * It returns a pointer to the first occurrence in str1 of any of the
 * entire sequence of characters specified in str2, or a null pointer if
 * the sequence is not present in str1.
 */
static __inl_unusd_ char * strstr(const char *str1,const char *str2)
{
  _esi(str1);
  __asm{
	mov eax,0
	mov ecx, -1
	cld
	mov edi, str2
	repne
	scasb
	not ecx
	dec ecx
	mov edx, ecx
	l1: mov edi, str2
	mov eax, esi
	mov ecx, edx
	repe
	cmpsb
	je l2
	xchg esi, eax
	inc esi
	cmp byte ptr [eax-1], 0
	jne l1
	xor eax,eax
	l2:
  }
}

extern char * __strtok;

// Search String for Token
/*
 * The function strtok breaks string str into a series of tokens using
 * the delimiter delim. It returns a pointer to the first token found in the
 * string. A null pointer is returned if there are no tokens left to retrieve.
 */
static __inl_unusd_ char * strtok(char *str, const char *delim)
{
  _edx(delim);
  _esi(str);
  _ebx(__strtok);
  __asm{
  test esi, esi
  jne l1
  test ebx, ebx
  je l8
  mov esi,ebx
  l1: xor ebx, ebx
  mov ecx, -1
  xor eax, eax
  cld
  mov edi, delim
  repne
  scasb
  not ecx
  dec ecx
  je l7
  mov edx, ecx
  l2: lodsb
  test al, al
  je l7
  mov edi, delim
  mov ecx, edx
  repne
  scasb
  je l2
  dec esi
  cmp byte ptr [esi], 0
  je l7
  mov ebx, esi
  l3: lodsb
  test al, al
  je l5
  mov edi, delim
  mov ecx, edx
  repne
  scasb
  jne l3
  dec esi
  cmp byte ptr [esi], 0
  je l5
  mov byte ptr [esi], 0
  inc esi
  jmp l6
  l5: xor esi, esi
  l6: cmp byte ptr [ebx], 0
  jne l7
  xor ebx, ebx
  l7: test ebx, ebx
  jne l8
  mov __strtok,esi
  l8:
  mov __strtok,esi
  mov eax, ebx
  }
}

// Search Memory Block for Character
/*
 * The function memchr searches for the first occurrence of the character c
 * (an unsigned char) in the first n bytes of the string pointed to, by the
 * argument str. It returns a pointer to the matching byte or NULL if the
 * character does not occur in the given memory area.
 */
static __inl_unusd_ void * memchr(const void *str, int c, size_t count)
{
  if (!count)
	return NULL;
  _edi(str);
  _ecx(count);
  _eax(c);
  __asm{
	cld
	repne
	scasb
	je l1
	mov edi, 1
	l1: dec edi
	mov eax, edi
  }
}

/// Miscellaneous functions

// String Length
/*
 * The function strlen computes the length of the string str up to,
 * but not including the terminating null character.
 * It returns the length of string.
 */
static __inl_unusd_ size_t strlen(const char *str)
{
  _edi(str);
  __asm{
	mov eax, 0
	mov ecx, -1
	cld
	repne
	scasb
	not ecx
	dec ecx
	mov eax, ecx
  }
}

// memset	Initialize Memory Block
/*
 * The function memset copies the character c (an unsigned char) to the
 * first count characters of the string pointed to, by the argument str.
 * It returns a pointer to the memory area str.
 */
static __inl_unusd_ void * memset(void *str, int c, size_t count)
{
  _edi(str);
  _ecx(count);
  _eax(c);
  __asm{
	cld
  	rep stosb
  	mov eax, str
  }
}

// TODO:
// strerror	Convert Error Number to String

#endif /* _STRING_H */


