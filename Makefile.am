ACLOCAL_AMFLAGS = -I m4 --install
AM_CPPFLAGS = -I$(top_srcdir)/include 
AM_CFLAGS = -I$(top_srcdir)/include 

lib_LTLIBRARIES = libobj/libthp.la
libobj_libthp_la_SOURCES =      src/log.c \
                                src/punch.c 
							
libobj_libthp_la_LIBADD = $(libevent_LIBS) \
			  -lssl -lcrypto \
			  $(jansson_LIBS) 
							
libobj_libthp_la_LDFLAGS = -version-info 0:0
include_HEADERS = include/thp.h 


bin_PROGRAMS = punchctl
punchctl_SOURCES = punchctl.c
punchctl_CPPFLAGS = -DDEBUG
punchctl_LDADD = $(lib_LTLIBRARIES) \
                 $(libevent_LIBS)

noinst_PROGRAMS = tests/unittests
tests_unittests_SOURCES = tests/test_init.c
tests_unittests_CFLAGS = -DDEBUG 
tests_unittests_LDADD = $(lib_LTLIBRARIES) \
	     $(libevent_LIBS) 

EXTRA_DIST = m4/NOTES 
