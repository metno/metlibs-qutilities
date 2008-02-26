#include <miSliderWidget.h>

#include <QLayout>
#include <QHBoxLayout>
#include <QLabel>

miSliderWidget::miSliderWidget(float minV, float maxV,
			       float stepV,float Val, 
			       Qt::Orientation orientation,
			       miString descript,miString unit,
			       bool usetracking,
			       QWidget* p, const char * name)
  : QWidget(p,name),
    minValue(minV), maxValue(maxV), stepValue(stepV), Value(Val), tracking(usetracking)
{
  if ( stepValue <= 0.0 )
    stepValue = 0.1;

  QHBoxLayout * hl =  new QHBoxLayout(this, 2,2, "hl"); 
  orientation= Qt::Horizontal;
  
  desclabel= new QLabel(descript.c_str(),this);
  hl->addWidget(desclabel);
  
  nsteps = static_cast<int>((maxValue - minValue)/stepValue);

  int iminValue =  0;
  int imaxValue = nsteps;
  int istepValue = 1;
  int iValue = static_cast<int>((Value - minValue)/stepValue);

  int tickinterval = istepValue;
  while ( ((imaxValue-iminValue)/tickinterval) > 40 ){
    tickinterval*= 10;
  }

  slider = new QSlider(iminValue, imaxValue, istepValue*5,
		       iValue, orientation,this);
  slider->setTracking(false);
  slider->setLineStep(istepValue);
  slider->setTickmarks(QSlider::TicksBelow);
  slider->setTickInterval(tickinterval);

  connect(slider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
  connect(slider,SIGNAL(sliderReleased()),this,SIGNAL(sliderReleased()));
  connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(sliderMoved(int)));
  hl->addWidget(slider,1);
  desclabel->setBuddy(slider);

  vallabel= new QLabel("",this);
  vallabel->setNum(Value);
  vallabel->setFixedWidth(50);
  
  hl->addWidget(vallabel);
}

float miSliderWidget::fValue(int v)
{
  return (minValue + roundf(v*stepValue*100)/100);
}

void miSliderWidget::valueChanged(int v)
{
//   cerr << "valueChanged" << endl;
  Value = fValue(v);
  vallabel->setNum(Value);

  emit valueChanged(Value);
}

void miSliderWidget::sliderMoved(int v)
{
//   cerr << "sliderMoved" << endl;
  Value = fValue(v);
  vallabel->setNum(Value);
  if (tracking) emit valueChanged(Value);
}

float miSliderWidget::value()
{
  return Value;
}
