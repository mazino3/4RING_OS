/*
 * 4RING_OS License: MIT
 *
 * stdarg.h
 *
 * The header defines a variable type va_list and three macros which can be
 * used to get the arguments in a function when the number of arguments are
 * not known i.e. variable number of arguments.
 * A function of variable arguments is defined with the ellipsis (,...) at the
 * end of the parameter list.
 * Sample: int foo(int a, int last_know_arg, ...) { }
 *
 * I needed, in order to better understand by myself, in explicitly way and in
 * detail document the functions and variables of this header.
 *
 * (C) Copyright 2021 Isa <isa@isoux.org>
 */

#ifndef _STDARG_H
#define _STDARG_H

/*
 * va_list This is a type suitable for holding information needed by the three
 * macros va_start(), va_arg() and va_end().
 */
typedef char * va_list;

/*
 * This macro calculates the size of each given type of argument.
 */
#define __argsize(type) \
  (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

/*
 * The macro va_start initializes arg_obj variable to be used with the
 * va_arg and va_end macros. The last_arg is the last known fixed argument
 * being passed to the function i.e. the argument before the ellipsis (x ,...).
 * This macro must be called before using va_arg and va_end.
 * Params:
 * arg_obj −  is the object of va_list and it will hold the information
 * needed to retrieve the additional arguments with va_arg.
 * last_arg − is the last known fixed argument being passed to the function.
 */
#define va_start(arg_obj, last_arg) \
  ((arg_obj) = ((va_list) &(last_arg) + __argsize(last_arg)))

/*
 * The macro type va_arg retrieves the next argument in the parameter list of
 * the function with type. This does not determine whether the retrieved
 * argument is the last argument passed to the function.
 * Params:
 * arg_obj is the object of type va_list with information about the additional
 * arguments and their retrieval state. This object should be initialized by an
 * initial call to va_start before the first call to va_arg.
 * type - is a type name. This type name is used as the type of the expression,
 * this macro expands to.
 * The macro returns the next additional argument as an expression of type type.
 */
#define va_arg(arg_obj, type) \
  (((arg_obj) += __argsize(type)), *(((type*) (void*) ((arg_obj) \
	  - __argsize(type)))))

/*
 * The macro void va_end allows a function with variable arguments which used
 * the va_start macro to return. If va_end is not called before returning from
 * the function, the result is undefined.
 * Params:
 * arg_obj is the va_list object previously initialized by va_start in the same
 * function.
 */
#define va_end(arg_obj) ((void) 0)

#endif /* _STDARG_H */
