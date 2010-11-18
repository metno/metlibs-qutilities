/*
 * qtHelpers.h
 *
 *  Created on: Aug 20, 2010
 *      Author: aleksandarb
 */

#ifndef QTHELPERS_H_
#define QTHELPERS_H_

#include <QString>
#include <vector>
#include "puTools/miString.h"

class QtHelpers {
public:
	static miutil::miString qt2mi_string(const QString qString);
	static QString mi2qt_string(const miutil::miString miString);

	static std::vector<miutil::miString> qt2mi_vector_string(const std::vector<QString>& vQString);
	static std::vector<QString> mi2qt_vector_string(const std::vector<miutil::miString>& vMiString);
};

#endif /* QTHELPERS_H_ */
