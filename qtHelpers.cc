/*
 * qtHelpers.cpp
 *
 *  Created on: Aug 20, 2010
 *      Author: aleksandarb
 */

#include "qtHelpers.h"

std::vector<miutil::miString> QtHelpers::qt2mi_vector_string(const std::vector<QString>& vQString)
{
    std::vector<miutil::miString> vMiString(vQString.size());
    for(int i = 0; i < vQString.size(); ++i)
	{
            vMiString.push_back(vQString[i].toStdString());
    }
    return vMiString;
}

std::vector<QString > QtHelpers::mi2qt_vector_string(const std::vector<miutil::miString>& vMiString)
{
    std::vector<QString> vQString(vMiString.size());
    for(int i = 0; i < vMiString.size(); ++i)
    {
        vQString.push_back(QString::fromStdString(vMiString[i]));
    }
    return vQString;
}

