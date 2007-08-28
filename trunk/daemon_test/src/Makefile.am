## Process this file with automake to produce Makefile.in

AM_CFLAGS = \
-Wall -W -g -D_GNU_SOURCE @EXALT_DAEMON_TEST_CFLAGS@


bin_PROGRAMS = exalt-daemon-test

exalt_daemon_test_INCLUDES = \
exalt_daemon.h

exalt_daemon_test_SOURCES = \
exalt_daemon.c

exalt_daemon_test_LDFLAGS = @DLLDFLAGS@
exalt_daemon_test_LDADD = @EXALT_DAEMON_TEST_LIBS@

MAINTAINERCLEANFILES = Makefile.in
