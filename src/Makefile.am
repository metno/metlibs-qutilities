lib_LTLIBRARIES = libqUtilities.la

qt_sources = \
    ClientButton.cc \
    CoClient.cc \
	miLogFile.cc \
	miMessage.cc \
	miSliderWidget.cc \
	miTimeSpinBox.cc \
	qbLoginDialog.cc \
	qbLoginInfo.cc

nodist_libqUtilities_la_SOURCES = \
	$(qt_sources:.cc=_moc.cc)

libqUtilities_la_SOURCES = \
	$(qt_sources)

libqUtilities_la_CXXFLAGS = -g ${puSQL_CFLAGS} ${puTools_CFLAGS}  ${miLogger_CFLAGS} \
    ${QT_CFLAGS} ${LOG4CXX_CPPFLAGS} 

libqUtilities_la_LDFLAGS = -g ${puSQL_LIBS} ${puTools_LIBS}  ${miLogger_LIBS} \
    ${QT_LIBS} 	${LOG4CXX_LIBS} ${VERSION_INFO}

CLEANFILES = $(qt_sources:.cc=_moc.cc)

# This rule lets GNU make create any moc_*.cpp from the equivalent *.h
%_moc.cc: $(top_srcdir)/include/%.h
	${MOC} $< -o $@

# I have not yet found a good way to do this:
LANGUAGES = $(top_srcdir)/lang/qUtilities_no.ts

languages:
	${QT_LUPDATE} -verbose $(qt_sources) -ts $(LANGUAGES)

INSTALL=install -p # tells install to preserve timestamp
DEFAULT_INCLUDES=-I${srcdir}/../include
