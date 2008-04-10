
#include <qtHelpDialog.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QTextBrowser>
#include <QProgressDialog>
#include <QPrintDialog>
#include <QDir>

//#include <pomSetup.h>

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <qmime.h> 
#include <qapplication.h> 
#include <qpainter.h>
#include <qprinter.h>


#include <puTools/miString.h>

#include <iostream>

#include <tb_close.xpm>
#include <tb_right_arrow.xpm>
#include <tb_left_arrow.xpm>
#include <tb_print.xpm>


HelpDialog::HelpDialog( QWidget* parent, const Info& hdi )
  : QDialog(parent), info(hdi), closebutton(0)
{
  setWindowTitle( tr("Help") );

  m_font= qApp->font();

  tb = new QTextBrowser( this ); 
  
  miString source = "";
  if (info.src.size()!=0) source= info.src[0].source;
  setSource( source );  
   
  pushbackward= new QPushButton( QPixmap(tb_left_arrow_xpm),
				 tr("Previous"), this );
  connect(pushbackward, SIGNAL( clicked()), tb, SLOT( backward()));

  pushforward= new QPushButton( QPixmap(tb_right_arrow_xpm),
				tr("Next"), this );
  connect(pushforward, SIGNAL( clicked()), tb, SLOT( forward())); 

  closebutton= new QPushButton( QPixmap(tb_close_xpm),
				tr("Close"), this );
  connect( closebutton, SIGNAL( clicked()), this, SLOT( hideHelp()) );

  printbutton= new QPushButton( QPixmap(tb_print_xpm),
				tr("Print.."), this );
  connect( printbutton, SIGNAL( clicked()), this, SLOT( printHelp()) );

  hlayout = new QHBoxLayout();    
  hlayout->addWidget( pushbackward );
  hlayout->addWidget( pushforward );
  hlayout->addWidget( closebutton );
  hlayout->addWidget( printbutton );
  hlayout->addStretch();
  
  vlayout = new QVBoxLayout( this );
  vlayout->addLayout( hlayout );
  vlayout->addWidget( tb );
  
  resize( 800, 600 );
}


void HelpDialog::hideHelp(){
  hide();
}


void HelpDialog::printHelp(){
  QPrinter printer;
#ifdef linux
  printer.setPrintProgram( QString("lpr") );
#else
  printer.setPrintProgram( QString("lp") );
#endif
  printer.setFullPage(TRUE);
  
  QPrintDialog *dialog = new QPrintDialog(&printer, this);
  dialog->setWindowTitle(tr("Print Document"));
  if (dialog->exec() != QDialog::Accepted)
    return;
  tb->document()->print(&printer);
}


// void HelpDialog::printHelp(){
//   QPrinter printer;
// #ifdef linux
//   printer.setPrintProgram( QString("lpr") );
// #else
//   printer.setPrintProgram( QString("lp") );
// #endif
//   printer.setFullPage(TRUE);

//   QString htmltext= tb->text();

//   pomSetup setup;
//   QFont font;
//   miString fontname= setup.gui.helpprintfont;
//   if (fontname.exists())
//     font.fromString( QString(fontname.cStr()) );


//   // Split source for proper page-breaks
//   QString sep("<!-- PAGEBREAK -->"); // the magic separator
//   QStringList vs = QStringList::split ( sep, htmltext, false);

//   bool usepagebreak = (vs.size() > 1);

//   int frompage= 1;
//   int topage = vs.size();

//   if (usepagebreak){
//     printer.setMinMax(frompage,topage);
//     printer.setFromTo(frompage,topage);
//   }

//   if ( printer.setup( this ) ) {
//     QPainter p( &printer );
//     Q3PaintDeviceMetrics metrics(p.device());
//     int dpix = metrics.logicalDpiX();
//     int dpiy = metrics.logicalDpiY();
//     const int margin = 40; // pt
//     QRect body(margin*dpix/72, margin*dpiy/72,
// 	       metrics.width()-margin*dpix/72*2,
// 	       metrics.height()-margin*dpiy/72*2 );
    
//     frompage= printer.fromPage();
//     topage=   printer.toPage();

//     bool firstpage= true;
//     int page = (usepagebreak ? 0 : 1);

//     QProgressDialog* progress = 0;

//     if (usepagebreak && vs.size() > 1){
//       // make a progress-dialog if more than one pages
//       progress= new QProgressDialog( tr("Printing document..."),
// 				     tr("Cancel printing "),
// 				     1, topage - frompage + 1,
// 				     this );
//       progress->setWindowModality(Qt::WindowModal);
//     }

//     for ( QStringList::Iterator it = vs.begin(); it != vs.end(); ++it ) {
//       // print entire document or one page
//       if (usepagebreak){
// 	page++;
// 	if (page < frompage)
// 	  continue;
// 	if (page > topage)
// 	  break;
	
// 	if (progress){ // show progress in dialog
// 	  progress->setValue( page - frompage + 1);
// 	  qApp->processEvents();
// 	  if ( progress->wasCanceled() )
// 	    break;
// 	}
// 	if (!firstpage) printer.newPage();
//       }

//       firstpage= false;

//       // reset all translations
//       p.resetXForm();

//       htmltext = *it;
//       if (usepagebreak){
// 	// Add proper html-tags if necessary
// 	if (!htmltext.contains(QString("<html>")))
// 	  htmltext = QString("<html><head></head><body>") + htmltext;
// 	if (!htmltext.contains(QString("</html>")))
// 	  htmltext += QString("</body></html>");
//       }

// //       Q3SimpleRichText richText( htmltext, font,//QFont(),
// // 				tb->context(), tb->styleSheet(),
// // 				tb->mimeSourceFactory(), body.height() );
// //       richText.setWidth( &p, body.width() );
// //       QRect view( body );
// //       do {
// // 	richText.draw( &p, body.left(), body.top(), view, colorGroup() );
// // 	view.moveBy( 0, body.height() );
// // 	p.translate( 0 , -body.height() );
// // 	p.drawText( view.right() -
// // 		    p.fontMetrics().width( QString::number(page) ),
// // 		    view.bottom() +
// // 		    p.fontMetrics().ascent() + 5, QString::number(page) );
// // 	if ( view.top()  >= richText.height() )
// // 	  break;
// // 	printer.newPage();
// // 	page++;
// //       } while (TRUE);

//     }
//     if (usepagebreak && progress){
//       progress->setValue( progress->maximum());
//     }
//   }
// }


void HelpDialog::setSource( const miString& source ){
  QDir dir( helpPath().c_str() );
  QStringList paths( dir.absolutePath() );
  tb->setSearchPaths( paths );
  //cerr << "Setting search path=" <<  dir.absolutePath().latin1() << endl;

  tb->setSource( QString(source.c_str()) );
  tb->update();
  return;
}


void HelpDialog::showdoc(const int doc, const miString tag ){
  if (info.src.size() <= doc) return;

  setWindowTitle( QString(info.src[doc].name.cStr()) );
  setSource( info.src[doc].source );
  
  if(tag.exists())
    tb->scrollToAnchor( QString(tag.cStr()) );
  else
    tb->scrollToAnchor( QString(info.src[doc].defaultlink.cStr()) );
  
  show();
}


