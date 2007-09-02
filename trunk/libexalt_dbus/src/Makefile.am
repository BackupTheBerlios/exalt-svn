MAINTAINERCLEANFILES = Makefile.in

AUTOMAKE_OPTIONS = 1.4 foreign

# Gettext
datadir = @datadir@
localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

INCLUDES	= \
 -Wall -g @LIBEXALT_DBUS_CFLAGS@

lib_LTLIBRARIES = libexalt_dbus.la

EXTRA_DIST =

EXALT_DBUSHEADERS = \
libexalt_dbus.h \
define.h \
exalt_dbus_ethernet.h \
exalt_dbus_wireless.h\
exalt_dbus_wireless_info.h

libexalt_dbus_la_SOURCES = \
libexalt_dbus.c \
exalt_dbus_ethernet.c \
exalt_dbus_wireless.c \
exalt_dbus_wireless_info.c \
$(EXALTHEADERS)

installed_headersdir = $(prefix)/include/exalt_dbus
installed_headers_DATA = $(EXALT_DBUSHEADERS)

libexalt_dbus_la_LIBADD  = @LIBEXALT_DBUS_LIBS@

libexalt_dbus_la_DEPENDENCIES =

libexalt_dbus_la_LDFLAGS = -version-info 1:0:0
