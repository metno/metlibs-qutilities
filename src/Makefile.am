# $Id$

pkgname = qUtilities

include $(top_srcdir)/metlibs.mk

# Should we flatten this as well?
SUBDIRS = lang

lib_LTLIBRARIES = libqUtilities.la

noqt_sources = \
	QLetterCommands.cc \
	miLogFile.cc \
	miMessage.cc

qt_sources = \
	ClientButton.cc \
	CoClient.cc \
	miSliderWidget.cc \
	miTimeSpinBox.cc \
	qtHelpDialog.cc \

nodist_libqUtilities_la_SOURCES = \
	$(qt_sources:.cc=.moc.cc)

libqUtilities_la_SOURCES = \
	$(noqt_sources) $(qt_sources)

libqUtilities_la_CPPFLAGS = \
	${MILOGGER_CPPFLAGS} \
	${QT4_CPPFLAGS}

libqUtilities_la_LDFLAGS = ${qUtilities_VERSION_INFO}
libqUtilities_la_LIBADD =
if ENABLE_SHARED_LIBRARY
libqUtilities_la_LDFLAGS += \
	${MILOGGER_LDFLAGS} \
	${QT4_LDFLAGS}
libqUtilities_la_LIBADD += \
	${MILOGGER_LIBS} \
	${QT4_LIBS}
endif


endif
endif

if WITH_FIMEX
libqUtilities_la_CPPFLAGS += \
	-DFIMEX \
	${FIMEX_CPPFLAGS}
libqUtilities_la_LDFLAGS += \
	${FIMEX_LDFLAGS}
libqUtilities_la_LIBADD += \
	${FIMEX_LIBS}
endif

# Do we really want to do this?  Wouldn't it be better to include the
# generated files in the dist tarball, like we do with configure etc.?
CLEANFILES = $(qt_sources:.cc=.moc.cc)

# I have not yet found a good way to do this:
LANGUAGES = $(srcdir)/lang/qUtilities_nb.ts $(srcdir)/lang/qUtilities_sv.ts

languages:
	${LUPDATE4} -verbose $(qt_sources) -ts $(LANGUAGES)

pkg_includedir = ${includedir}/qUtilities

pkg_include_HEADERS = \
	ClientButton.h \
	CoClient.h \
	QLetterCommands.h \
	miLogFile.h \
	miMessage.h \
	miSliderWidget.h \
	miTimeSpinBox.h \
	qtHelpDialog.h \
	qbLoginDialog.h \
	qbLoginInfo.h \
	circle_green.xpm \
	circle_red.xpm \
	circle_yellow.xpm \
	conn.xpm \
	disconn.xpm \
	tb_close.xpm \
	tb_left_arrow.xpm \
	tb_print.xpm \
	tb_right_arrow.xpm \
	unconn.xpm

# ???
%.xpm:
	@:
