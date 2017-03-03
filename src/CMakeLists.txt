
SET(CMAKE_AUTOMOC TRUE)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(${PC_METLIBS_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})
ADD_DEFINITIONS(-DQT_NO_KEYWORDS -W -Wall ${PC_METLIBS_CFLAGS_OTHER})
LINK_DIRECTORIES(${PC_METLIBS_LIBRARY_DIRS} ${BOOST_LIBRARY_DIRS})

########################################################################

SET(qutilities_SOURCES
  miLoggingQt.cc
  miLogFile.cc
  miSliderWidget.cc
  qtHelpDialog.cc
)

METNO_HEADERS (qutilities_HEADERS qutilities_SOURCES ".cc" ".h")
LIST(APPEND qutilities_HEADERS
  qUtilitiesVersion.h
)

########################################################################

FILE(GLOB QUTILITIES_TS ../lang/qUtilities_*.ts)
METNO_QT_ADD_TRANSLATION(QM ${QUTILITIES_TS})

ADD_LIBRARY(qutilities SHARED
  ${qutilities_SOURCES}
  ${QM}
)

SET_TARGET_PROPERTIES(qutilities PROPERTIES
  VERSION ${QUTILITIES_PVERSION_FULL}
  SOVERSION "${QUTILITIES_VERSION_MAJOR}"
  OUTPUT_NAME "${lib_name}"
)

TARGET_LINK_LIBRARIES(qutilities
  ${QT_LIBRARIES}
  ${PC_METLIBS_LIBRARIES}
  ${BOOST_LIBRARIES}
)

INSTALL(TARGETS qutilities
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

INSTALL(FILES ${QM}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/metlibs/translations
)

INSTALL(FILES ${qutilities_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/metlibs/qUtilities
)
