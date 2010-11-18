#ifndef _miSliderWidget_h
#define _miSliderWidget_h


/*!
    ------------------------------------
    Created by:
    met.no/FoU/pu/
    at Tue Apr  3 09:42:51 2007
    $Revision$
    at $Date$
    ------------------------------------
*/

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>

#include <puTools/miString.h>

using namespace std;

class miSliderWidget : public QWidget {
  Q_OBJECT
private:
  QLabel  * desclabel;
  QSlider * slider;
  QLabel  * vallabel;
  QLineEdit * valedit;

  int nsteps;
  float minValue;
  float maxValue;
  float stepValue;
  float Value;
  bool tracking;
  QString parname;
  bool editfield;
  bool buttons;

  float fValue(int v);
  void writeValue(float V);

public:
  miSliderWidget(float minV,  float maxV, float stepV, float Val,
                 Qt::Orientation orientation, const QString& descript,
                 const QString& unit, bool usetracking, QWidget* p, const char * name = 0,
                 bool editf=false, bool btns=false);

  float value();
  void  setValue(float v);

public slots:
  void valueChanged(int v);
  void sliderMoved(int v);
  void editingFinished();

signals:
  void valueChanged(float);
  void valueChangedForPar(float, QString);
  void valueChangedForPar(float,miutil::miString);
  void sliderReleased();
};

#endif
