// -*- c++ -*-

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

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QTextBrowser;
class QLabel;
class QLineEdit;


class HelpDialog: public QDialog {
  Q_OBJECT
public:

  struct Info {
    struct Source {
      miutil::miString source;
      miutil::miString name;
      miutil::miString defaultlink;
    };
    miutil::miString path;
    std::vector<Source> src;
  };

  HelpDialog( QWidget* parent, const Info& hdi );

  void setSource( const miutil::miString& source );
  miutil::miString helpPath() const {return info.path;}

public slots:
  void showdoc(const int doc, const miutil::miString tag ="");
  void showsource( const miutil::miString& source, const miutil::miString tag ="");
  void showSearchBar();
  void searchDocument();

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
  QWidget* searchBar;
  QLineEdit* searchEdit;

  QVBoxLayout* vlayout;
  QHBoxLayout* hlayout;

  QFont m_font;
};


#endif
