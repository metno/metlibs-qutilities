/*
 * qtHelpers.cpp
 *
 *  Created on: Aug 20, 2010
 *      Author: aleksandarb
 */

#include "qtHelpers.h"

miutil::miString QtHelpers::qt2mi_string(const QString qString)
{
    return miutil::miString(qString.toLocal8Bit().constData());
}

QString QtHelpers::mi2qt_string(const miutil::miString miString)
{
    return QString(miString.cStr());
}

std::vector<miutil::miString> QtHelpers::qt2mi_vector_string(const std::vector<QString>& vQString)
{
    std::vector<miutil::miString> vMiString(vQString.size());
    for(int i = 0; i < vQString.size(); ++i)
	{
	    vMiString.push_back(qt2mi_string(vQString[i]));
    }
    return vMiString;
}

std::vector<QString > QtHelpers::mi2qt_vector_string(const std::vector<miutil::miString>& vMiString)
{
    std::vector<QString> vQString(vMiString.size());
    for(int i = 0; i < vMiString.size(); ++i)
    {
	    vQString.push_back(mi2qt_string(vMiString[i]));
    }
    return vQString;
}

