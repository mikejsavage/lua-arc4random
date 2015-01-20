#! /usr/bin/lua

local arc4 = require( "arc4random" )

for i = 1, 100 do
	print( ( "%.3f" ):format( arc4.random() ) )
end
print( arc4.random() )
print( arc4.random( 3 ) )
print( arc4.random( -5, 5 ) )

local str = arc4.buf( 16 )
str = str:gsub( "(.)", function( c )
	return ( "%02x" ):format( string.byte( c ) )
end )
print( str )
