#ifndef METLIBS_QUTILITIES_VERSION_H
#define METLIBS_QUTILITIES_VERSION_H

#define METLIBS_QUTILITIES_VERSION_MAJOR 8
#define METLIBS_QUTILITIES_VERSION_MINOR 0
#define METLIBS_QUTILITIES_VERSION_PATCH 1

#define METLIBS_QUTILITIES_VERSION_INT(major,minor,patch) \
    (1000000*major + 1000*minor + patch)
#define METLIBS_QUTILITIES_VERSION_CURRENT_INT \
    METLIBS_QUTILITIES_VERSION_INT(METLIBS_QUTILITIES_VERSION_MAJOR, METLIBS_QUTILITIES_VERSION_MINOR, METLIBS_QUTILITIES_VERSION_PATCH)

#endif // METLIBS_QUTILITIES_VERSION_H
