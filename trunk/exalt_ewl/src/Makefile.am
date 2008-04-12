## Process this file with automake to produce Makefile.in

AM_CFLAGS = \
-Wall -W -g -D_GNU_SOURCE @EXALT_CFLAGS@


bin_PROGRAMS = exalt_ewl

exalt_ewl_INCLUDES = \
exalt.h            \
main_window.h        \
eth_panel.h     \
general_panel.h     \
wireless_panel.h \
about_panel.h \
boot_panel.h \
dns_panel.h

exalt_ewl_SOURCES = \
exalt.c \
main_window.c        \
eth_panel.c     \
general_panel.c     \
wireless_panel.c \
about_panel.c \
boot_panel.c \
dns_panel.c

exalt_ewl_LDFLAGS = @DLLDFLAGS@
exalt_ewl_LDADD = @EXALT_LIBS@

MAINTAINERCLEANFILES = Makefile.in
