// -*- c++ -*-

#ifndef _helpdialog_h
#define _helpdialog_h

#include <qdialog.h>
#include <qfont.h>
#include <qpalette.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include <string>
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
      std::string source;
      std::string name;
      std::string defaultlink;
    };
    std::string path;
    std::vector<Source> src;
  };

  HelpDialog(QWidget* parent, const Info& hdi);

  void setSource( const std::string& source );
  std::string helpPath() const {return info.path;}

public Q_SLOTS:
  void showdoc(const int doc, const std::string& tag = "");
  void showsource(const std::string& source, const std::string& tag = "");
  void showSearchBar();
  void searchDocument();

private Q_SLOTS:
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
