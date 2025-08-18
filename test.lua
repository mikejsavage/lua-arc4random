#! /usr/bin/env lua

local arc4 = require( "arc4random" )

for i = 1, 10000 do
	local r01 = arc4.random()
	assert( r01 >= 0 and r01 < 1 )
end

do
	local count13 = { }
	for i = 1, 10000 do
		local r13 = arc4.random( 3 )
		assert( r13 >= 1 and r13 <= 3 )
		count13[ r13 ] = ( count13[ r13 ] or 0 ) + 1
	end
	assert( count13[ 1 ] > 0 and count13[ 2 ] > 0 and count13[ 3 ] > 0 )
end

do
	local count55 = { }
	for i = 1, 10000 do
		local r55 = arc4.random( -5, 5 )
		assert( r55 >= -5 and r55 <= 5 )
		count55[ r55 ] = ( count55[ r55 ] or 0 ) + 1
	end
	for i = -5, 5, 1 do
		assert( count55[ i ] > 0 )
	end
end

local str = arc4.buf( 16 )
str = str:gsub( "(.)", function( c )
	return ( "%02x" ):format( string.byte( c ) )
end )
print( str )
