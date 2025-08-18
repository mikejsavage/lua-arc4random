/*
 * Copyright (c) Michael Savage <mike@mikejsavage.co.uk>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "ggentropy.h"

#include <lua.h>
#include <lauxlib.h>

#if LUA_VERSION_NUM < 502
	#define luaL_newlib( L, l ) ( lua_newtable( L ), luaL_register( L, NULL, l ) )
#endif

#define LUAINT_MAX ( sizeof( lua_Integer ) == 4 ? INT32_MAX : INT64_MAX )

static uint32_t Random32() {
	uint32_t x;
	ggentropy( &x, sizeof( x ) );
	return x;
}

static uint64_t Random64() {
	uint64_t x;
	ggentropy( &x, sizeof( x ) );
	return x;
}

static double RandomDouble01() {
	uint64_t r = Random64();

	uint64_t u64 = UINT64_C( 0x3FF0000000000000 ) | ( r >> 12 );
	double d;
	memcpy( &d, &u64, sizeof( d ) );

	return d - 1.0;
}

// http://www.pcg-random.org/posts/bounded-rands.html
static int RandomUniformExact( int lo, int hi ) {
	uint32_t range = ( uint32_t )( hi ) - ( uint32_t )( lo );
	uint32_t x = Random32();

	uint64_t m = ( uint64_t )( x ) * ( uint64_t )( range );
	uint32_t l = ( uint32_t )( m );
	if( l < range ) {
		uint32_t t = -range;
		if( t >= range ) {
			t -= range;
			if( t >= range )
				t %= range;
		}
		while( l < t ) {
			x = Random32();
			m = ( uint64_t )( x ) * ( uint64_t )( range );
			l = ( uint32_t )( m );
		}
	}
	return lo + ( m >> 32 );
}

static int luaarc4_random( lua_State * L ) {
	if( lua_gettop( L ) == 0 ) {
		lua_pushnumber( L, RandomDouble01() );
		return 1;
	}

	lua_Integer min = 1;
	lua_Integer max;

	if( lua_gettop( L ) == 1 ) {
		max = lua_tointeger( L, 1 );
	}
	else {
		min = lua_tointeger( L, 1 );
		max = lua_tointeger( L, 2 );
	}

	if( max == LUAINT_MAX ) {
		luaL_error( L, "upper bound too large" );
	}

	lua_Integer max_plus_one = max + 1;

	if( max_plus_one <= min ) {
		luaL_error( L, "interval is empty" );
	}

	if( max_plus_one - min > INT32_MAX ) {
		luaL_error( L, "interval too large" );
	}

	lua_Integer r = RandomUniformExact( min, max_plus_one );
	lua_pushinteger( L, r );

	return 1;
}

static int luaarc4_buf( lua_State * L ) {
	lua_Integer bytes = luaL_checkinteger( L, 1 );
	char * buf = malloc( bytes );
	if( buf == NULL ) {
		luaL_error( L, "out of memory" );
	}

	if( !ggentropy( buf, bytes ) ) {
		luaL_error( L, "ggrandom failed" );
	}
	lua_pushlstring( L, buf, bytes );
	free( buf );

	return 1;
}

static const struct luaL_Reg luaarc4_lib[] = {
	{ "random", luaarc4_random },
	{ "buf", luaarc4_buf },
	{ NULL, NULL },
};

LUALIB_API int luaopen_arc4random( lua_State * L ) {
	luaL_newlib( L, luaarc4_lib );
	return 1;
}
