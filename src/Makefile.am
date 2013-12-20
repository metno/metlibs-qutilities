# $Id$
# NB! coserver stuff has been moved into the coserver library
#
#

pkgname = qUtilities

lib_LTLIBRARIES = libqUtilities.la

cc_sources= \
	miLogFile.cc 


qt_sources = \
	miSliderWidget.cc \
	qtHelpDialog.cc 

img_sources=\
	circle_green.xpm \
	circle_red.xpm \
	circle_yellow.xpm \
	tb_close.xpm \
	tb_left_arrow.xpm \
	tb_print.xpm \
	tb_right_arrow.xpm 



# Generated Qt source code 
moc_sources = $(qt_sources:.cc=.moc.cc) 
%.moc.cc: %.h
	$(MOC4) -o $@ $<

nodist_libqUtilities_la_SOURCES = $(moc_sources)

libqUtilities_la_SOURCES = $(cc_sources) $(qt_sources)

pkg_includedir = ${includedir}/qUtilities

pkg_include_HEADERS = \
	$(cc_sources:.cc=.h)\
	$(qt_sources:.cc=.h)\
	$(img_sources)


libqUtilities_la_CPPFLAGS = ${QT4_CPPFLAGS}
libqUtilities_la_LDFLAGS  = ${VERSION_INFO}


if ENABLE_SHARED_LIBRARY

libqUtilities_la_LDFLAGS +=  ${QT4_LDFLAGS}
libqUtilities_la_LIBADD   =  ${QT4_LIBS}

endif




# Languages -------------------------

language_sources= \
	$(top_srcdir)/lang/qUtilities_nb.ts \
	$(top_srcdir)/lang/qUtilities_sv.ts 

languagedir= $(datarootdir)/tseries/@PVERSION@/lang
nodist_language_DATA=$(language_sources:.ts=.qm)


%.qm: %.ts  $(qt_sources)
	$(LUPDATE4)  $(qt_sources) -ts $< ; \
	$(LRELEASE4)  $<



CLEANFILES = $(moc_sources)

INSTALL = install -p