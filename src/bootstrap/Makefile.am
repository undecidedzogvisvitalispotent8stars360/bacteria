AM_CFLAGS = 
bin_PROGRAMS = bbootstrap-server
bbootstrap_server_SOURCES = main.cpp utils.cpp opcodes.cpp serv.cpp
#$(top_builddir)/libbacteria/lua/luaserv.c
ibootstrap_server_DEPENDENCIES = $(top_builddir)/libbacteria/libbacteria.a
bbootstrap_server_LDADD = $(top_builddir)/libbacteria/libbacteria.a -lluajit-5.1
ibootstrap_server_CFLAGS= -g

AM_CPPFLAGS = -I$(top_srcdir)/libbacteria/ -I/usr/include/lua5.1/ -I$(top_builddir)/libbacteria -I/usr/include/lua5.1/ -I /usr/include/luajit-2.1/ 


target_CPPFLAGS = -I$(top_srcdir)/libbacteria/




