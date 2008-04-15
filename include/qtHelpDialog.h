#ifndef _helpdialog_h
#define _helpdialog_h


#include <qdialog.h>
#include <qfont.h>
#include <qpalette.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include <puTools/miString.h>

#include <vector>

using namespace std;

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QTextBrowser;
class QLabel;


class HelpDialog: public QDialog {
  Q_OBJECT
public:
  
  struct Info {
    struct Source {
      miString source;
      miString name;
      miString defaultlink; 
    };
    miString path;
    vector<Source> src;
  };
  
  HelpDialog( QWidget* parent, const Info& hdi );

  void setSource( const miString& source );
  miString helpPath() const {return info.path;}

public slots:
  void showdoc(const int doc, const miString tag ="");
  void showsource( const miString& source, const miString tag ="");

private slots:
  void hideHelp();
  void printHelp();
   
private:
  QTextBrowser* tb;
  Info info;

  QPushButton* pushbackward;
  QPushButton* pushforward;
  QPushButton* closebutton;
  QPushButton* printbutton;
  QLabel* plabel; 

  QVBoxLayout* vlayout;
  QHBoxLayout* hlayout;

  QFont m_font; 
};


#endif
