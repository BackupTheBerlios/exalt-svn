## Process this file with automake to produce Makefile.in

AM_CFLAGS = \
-Wall -W -g -D_GNU_SOURCE @EXALT_DAEMON_CFLAGS@


bin_PROGRAMS = exalt-daemon

exalt_daemon_INCLUDES = \
daemon.h \
cb_functions.h \
define.h

exalt_daemon_SOURCES = \
daemon.c \
cb_functions.c

exalt_daemon_LDFLAGS = @DLLDFLAGS@
exalt_daemon_LDADD = @EXALT_DAEMON_LIBS@

MAINTAINERCLEANFILES = Makefile.in
