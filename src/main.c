/*
 * Copyright (c) 2015, Michael Savage <mike@mikejsavage.co.uk>
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
#include <sys/types.h>
#include <math.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#if LUA_VERSION_NUM < 502
	#define luaL_newlib( L, l ) ( lua_newtable( L ), luaL_register( L, NULL, l ) )
#endif

#define LUAINT_MAX ( sizeof( lua_Integer ) == 32 ? INT32_MAX : INT64_MAX )

static int luaarc4_random( lua_State * const L ) {
	if( lua_gettop( L ) == 0 ) {
		// generate a random 53 bit int, then divide by 2^53
		const uint64_t r64 = ( ( uint64_t ) arc4random() << 32 ) | arc4random();
		const uint64_t r53 = r64 & ( ( UINT64_C( 1 ) << 53 ) - 1 );
		const double d = ( double ) r53 / ( UINT64_C( 1 ) << 53 );

		lua_pushnumber( L, d );

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
		lua_pushliteral( L, "upper bound too large" );
		return lua_error( L );
	}

	lua_Integer max_plus_one = max + 1;

	if( max_plus_one <= min ) {
		lua_pushliteral( L, "interval is empty" );
		return lua_error( L );
	}

	if( max_plus_one - min > UINT32_MAX ) {
		lua_pushliteral( L, "interval too large" );
		return lua_error( L );
	}

	const lua_Integer r = min + arc4random_uniform( max_plus_one - min );
	lua_pushinteger( L, r );

	return 1;
}

static int luaarc4_buf( lua_State * const L ) {
	const long bytes = luaL_checklong( L, 1 );
	char * const buf = malloc( bytes );

	if( buf == NULL ) {
		lua_pushliteral( L, "out of memory" );
		return lua_error( L );
	}

	arc4random_buf( buf, bytes );
	lua_pushlstring( L, buf, bytes );
	free( buf );

	return 1;
}

static const struct luaL_Reg luaarc4_lib[] = {
	{ "random", luaarc4_random },
	{ "buf", luaarc4_buf },
	{ NULL, NULL },
};

LUALIB_API int luaopen_arc4random( lua_State * const L ) {
	luaL_newlib( L, luaarc4_lib );

	return 1;
}
