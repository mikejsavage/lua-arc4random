/*
 * Public domain
 * string.h compatibility shim
 */

#include_next <string.h>

#ifndef LIBCRYPTOCOMPAT_STRING_H
#define LIBCRYPTOCOMPAT_STRING_H

void explicit_bzero(void *, size_t);

#endif
