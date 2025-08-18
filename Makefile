all:
	luarocks make CFLAGS="-O2 -fPIC -Wall -Wextra" --no-install

debug:
	luarocks make CFLAGS="-fPIC -g -Wall -Wextra" --no-install

clean:
	rm -f arc4random.so src/*.o
