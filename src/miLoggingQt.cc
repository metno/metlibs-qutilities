
#define MILOGGER_CATEGORY "milogger"
#include "miLoggingQt.h"

#include <QString>
#include <QStringList>

#include <ostream>

std::ostream& operator<<(std::ostream& out, const QString& qs)
{
    out << qs.toStdString();
    return out;
}

std::ostream& operator<<(std::ostream& out, const QStringList& qsl)
{
    out << '[';
    for (int i=0; i<qsl.count(); ++i) {
        if (i>0)
            out << ' ';
        out << '\'' << qsl.at(i) << '\'';
    }
    out << ']';
    return out;
}
