#SUBDIRS=lua
AUTOMAKE_OPTIONS = subdir-objects
lib_LIBRARIES = libbacteria.a
libbacteria_a_SOURCES = cryptocoins.c  cryptocoins.h  json_rpc.c  macros.h \
		       	lua/afirst.c  lua/libcrypto.c  lua/libjson.c lua/libencdec.c lua/libimages.c lua/libgmp.c lua/libed25519rsa.c \
			encdec/async/x25519.c encdec/sync/AES.c encdec/verify/rsa_ed25519.c encdec/verify/base64.c \
			images/images.c \
			lua/luaserv.c
libbacteria_a_CFLAGS = -g -I /usr/include/luajit-2.1/ -I$(topdir)/libbacteria/ -I$(topdir)/luasql/src -L$(topdir)/luasql/src -I/usr/include/lua5.1/

