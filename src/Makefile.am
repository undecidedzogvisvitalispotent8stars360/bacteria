AM_CFLAGS = 
bin_PROGRAMS = bacteria
bacteria_SOURCES = async_serv.c main.c signal_handler.c
bacteria_DEPENDENCIES = $(top_builddir)/libbacteria/libbacteria.a
bacteria_LDADD = $(top_builddir)/libbacteria/libbacteria.a -lluajit-5.1
bacteria_CFLAGS= -g

AM_CPPFLAGS = -I$(top_srcdir)/libbacteria/ -I/usr/include/lua5.1/ -I$(top_builddir)/libbacteria -I/usr/include/lua5.1/ -I /usr/include/luajit-2.1/


target_CPPFLAGS = -I$(top_srcdir)/libbacteria/




