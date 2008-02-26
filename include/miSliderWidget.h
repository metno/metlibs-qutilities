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

#include <puTools/miString.h>

using namespace std; 

class miSliderWidget : public QWidget {
  Q_OBJECT
private:
  QLabel * desclabel;
  QSlider * slider;
  QLabel * vallabel;

  int nsteps;
  float minValue;
  float maxValue;
  float stepValue;
  float Value;
  bool tracking;

  float fValue(int v);

public:
  miSliderWidget(float minV,  float maxV, float stepV, float Val,
		 Qt::Orientation orientation, miString descript,
		 miString unit, bool usetracking, QWidget* p, const char * name = 0);

  float value();

public slots:
  void valueChanged(int v);
  void sliderMoved(int v);

signals:
  void valueChanged(float);
  void sliderReleased();
};

#endif
