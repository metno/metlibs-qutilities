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
  : QWidget(p),
    minValue(minV), maxValue(maxV), stepValue(stepV), Value(Val), tracking(usetracking)
{
	setObjectName(name);
	
  if ( stepValue <= 0.0 )
    stepValue = 0.1;

  QBoxLayout * hl;
  if(orientation==Qt::Horizontal) 
    hl =  new QBoxLayout(QBoxLayout::LeftToRight,this);
  else {
    hl =  new QBoxLayout(QBoxLayout::BottomToTop,this);
    hl->setAlignment(Qt::AlignHCenter);
  }
  hl->setObjectName("hl");

  parname=descript;
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
  
  slider = new QSlider(orientation, this);
  slider->setTracking(false);
  slider->setSingleStep(istepValue);
  slider->setPageStep(istepValue*5);
  slider->setTickPosition(QSlider::TicksBelow);
  slider->setTickInterval(tickinterval);
  slider->setMinimum(iminValue);
  slider->setMaximum(imaxValue);
  slider->setSliderPosition(iValue);

  connect(slider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
  connect(slider,SIGNAL(sliderReleased()),this,SIGNAL(sliderReleased()));
  connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(sliderMoved(int)));
  hl->addWidget(slider,1);
  desclabel->setBuddy(slider);

  vallabel= new QLabel("",this);
  vallabel->setNum(Value);
  
//  if(orientation==Qt::Horizontal)
//    vallabel->setFixedWidth(30);  
  
  hl->addWidget(vallabel);
}

float miSliderWidget::fValue(int v)
{
  return (minValue + roundf(v*stepValue*100)/100);
}

void miSliderWidget::setValue(float v) 
{
  if(v<minValue || v > maxValue)
    return;
  Value=v;
  
  int ivalue = static_cast<int>((Value - minValue)/stepValue);

  vallabel->setNum(Value);
  slider->setSliderPosition(ivalue);
} 



void miSliderWidget::valueChanged(int v)
{
//   cerr << "valueChanged" << endl;
  Value = fValue(v);
  vallabel->setNum(Value);

  emit valueChanged(Value);
  emit valueChangedForPar(Value,parname);
}

void miSliderWidget::sliderMoved(int v)
{
//   cerr << "sliderMoved" << endl;
  Value = fValue(v);
  vallabel->setNum(Value);
  if (tracking){
    emit valueChanged(Value);
    emit valueChangedForPar(Value,parname);
  }
}

float miSliderWidget::value()
{
  return Value;
}
