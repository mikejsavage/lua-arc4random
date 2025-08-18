package = "arc4random"
version = "1.5-1"

source = {
	url = "git://github.com/mikejsavage/lua-arc4random.git",
	tag = "v1.5-1",
}

description = {
	summary = "A CSPRNG for Lua",
	homepage = "http://github.com/mikejsavage/lua-arc4random",
	license = "MIT",
	maintainer = "Mike Savage",
}

dependencies = {
	"lua >= 5.1",
}

build = {
    type = "builtin",
    modules = { arc4random = { "src/main.c", "src/ggentropy.c" } },
}
