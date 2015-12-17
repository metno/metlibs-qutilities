
#ifndef METLIBS_MILOGGER_MILOGGINGQT_H
#define METLIBS_MILOGGER_MILOGGINGQT_H 1

#ifdef __WIN32__
#define METLIBS_LOG_SCOPE(x) /* emtpy */
#define METLIBS_LOG_ERROR(x) /* emtpy */
#define METLIBS_LOG_INFO(x)  /* emtpy */
#define METLIBS_LOG_DEBUG(x) /* emtpy */
#else
#include <miLogger/miLogging.h>
#endif /* __WIN32__ */

#include <iosfwd>
class QString;
class QStringList;

std::ostream& operator<<(std::ostream& out, const QString& qs);
std::ostream& operator<<(std::ostream& out, const QStringList& qsl);

#endif // METLIBS_MILOGGER_MILOGGINGQT_H
