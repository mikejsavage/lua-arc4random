A Lua wrapper for OpenBSD's arc4random.


Requirements
------------

lua >= 5.1


Copying
-------

Many of the files in this repository have been taken from OpenBSD's
tree. You should consult individual file headers for specific licensing
information. More broadly, everything here is compatible with the [ISC
license][ISC].

[ISC]: http://en.wikipedia.org/wiki/ISC_license


Installation
------------

	$ luarocks install arc4random


Usage
-----

The library provides two methods, `random` and `buf`. `random` is
intended to be a drop in replacement for `math.random`, so it handles
three cases.

`arc4.random()` returns a random floating point number in the range
[0,1). `arc4.random( n )` and `arc4.random( m, n )` return a random
integer in the range [1,n] and [m,n] respectively. All three cases match
the behavior of `math.random`.

`arc4.buf( n )` returns a string of `n` random characters. It is
suitable for generating private keys and IVs.

Some example code:

	local arc4 = require( "arc4random" )
	
	print( arc4.random() )
	print( arc4.random( 3 ) )
	print( arc4.random( -5, 5 ) )
	
	local str = arc4.buf( 16 )
	str = str:gsub( "(.)", function( c )
		return ( "%02x" ):format( string.byte( c ) )
	end )
	print( str )
