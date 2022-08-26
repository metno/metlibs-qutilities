PROJECT(metlibs-qutilities)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(MetnoUtils)
INCLUDE(MetnoQtUtils)

METNO_PVERSION_DEFINES(QUTILITIES "src/qUtilitiesVersion.h")

SET(PKGS
  metlibs-milogger>=6.0.0
)

PKG_CHECK_MODULES(PC_METLIBS REQUIRED
  ${PKGS}
)

FIND_PACKAGE(Boost REQUIRED)

METNO_FIND_QT(Widgets BUILDONLY PrintSupport)

SET(lib_name "metlibs-qutilities${METNO_QT_SUFFIX}")

METNO_GEN_PKGCONFIG(metlibs-qutilities.pc.in "${lib_name}.pc"
  "${PKGS};${METNO_PC_DEPS_QT}" "" "" "")

CONFIGURE_FILE(qUtilities.pc.in qUtilities.pc @ONLY)
INSTALL(FILES ${CMAKE_BINARY_DIR}/qUtilities.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig")


ADD_SUBDIRECTORY(src)
