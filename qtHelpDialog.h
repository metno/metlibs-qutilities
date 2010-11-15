#ifndef _helpdialog_h
#define _helpdialog_h

#include <QString>
#include <QDialog>
#include <QFont>
#include <QPalette>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

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
      QString source;
      QString name;
      QString defaultlink;
    };
    QString path;
    vector<Source> src;
  };

  HelpDialog( QWidget* parent, const Info& hdi );

  void setSource( const QString& source );
  QString helpPath() const {return info.path;}

public slots:
  void showdoc(const int doc, const QString& tag = QString());
  void showsource( const QString& source, const QString& tag = QString());

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
