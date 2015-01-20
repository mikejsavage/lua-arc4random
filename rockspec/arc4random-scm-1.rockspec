package = "arc4random"
version = "scm-1"

source = {
	url = "git://github.com/mikejsavage/lua-arc4random.git",
}

description = {
	summary = "A Lua wrapper for arc4random",
	homepage = "http://github.com/mikejsavage/lua-arc4random",
	license = "ISC",
	maintainer = "Mike Savage",
}

dependencies = {
	"lua >= 5.1",
}

build = {
	type = "make",

	install_pass = false,

	build_variables = {
		LUA_INCDIR = "$(LUA_INCDIR)",
	},

	install = {
		lib = {
			[ "arc4random" ] = "arc4random.so",
		},
	},
}
