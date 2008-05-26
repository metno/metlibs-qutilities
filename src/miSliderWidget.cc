#include <miSliderWidget.h>

#include <QLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleValidator>
#include <QShortcut>

miSliderWidget::miSliderWidget(float minV, float maxV,
			       float stepV,float Val, 
			       Qt::Orientation orientation,
			       miString descript,miString unit,
			       bool usetracking,
			       QWidget* p, const char * name,
			       bool editf, bool btns)
  : QWidget(p),
    minValue(minV), maxValue(maxV), stepValue(stepV), Value(Val), tracking(usetracking),
    editfield(editf), buttons(btns), valedit(0), vallabel(0)
{
  
  editfield=true;

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
  //hl->setSpacing(1);

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

  if ( editfield ){
    valedit = new QLineEdit(this);
    valedit->setMaximumWidth(50);
    float steplog = logf(stepValue );
    int decimals = ( steplog < 0.0 ? 1 : 0 );
    valedit->setValidator ( new QDoubleValidator(minValue,maxValue,decimals,this) );
    connect(valedit,SIGNAL(editingFinished()),this,SLOT(editingFinished())); 
    QShortcut *scut = new QShortcut(QKeySequence(tr("Ctrl+A")),valedit,
				    0,0,Qt::WidgetShortcut);
    connect(scut,SIGNAL(activated()),this,SLOT(editingFinished()) );

    hl->addWidget(valedit,2);

  } else {
    vallabel= new QLabel("",this);
    hl->addWidget(vallabel,2);
  }
  writeValue(Value);
  
//  if(orientation==Qt::Horizontal)
//    vallabel->setFixedWidth(30);  
}

void miSliderWidget::editingFinished()
{
  cerr << "void miSliderWidget::editingFinished():" <<  valedit->text().toStdString() << endl;
  if ( valedit && valedit->hasAcceptableInput()){
    float v = valedit->text().toFloat();
    setValue(v);
    emit valueChanged(Value);
    emit valueChangedForPar(Value,parname);
  }
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

  writeValue(Value);
  slider->setSliderPosition(ivalue);
} 


void miSliderWidget::writeValue(float V)
{
  if ( editfield ){
    ostringstream ost;
    ost << V;
    if (valedit) valedit->setText(ost.str().c_str());
  } else {
    if (vallabel) vallabel->setNum(V);
  }
}

void miSliderWidget::valueChanged(int v)
{
//   cerr << "valueChanged" << endl;
  Value = fValue(v);
  
  writeValue(Value);

  emit valueChanged(Value);
  emit valueChangedForPar(Value,parname);
}

void miSliderWidget::sliderMoved(int v)
{
//   cerr << "sliderMoved" << endl;
  Value = fValue(v);
  writeValue(Value);
  if (tracking){
    emit valueChanged(Value);
    emit valueChangedForPar(Value,parname);
  }
}

float miSliderWidget::value()
{
  return Value;
}
