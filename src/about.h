#ifndef ABOUT_HPP
#define ABOUT_HPP

#include "ui_about.h"

class About: public QDialog, public Ui::AboutBox
{
  
public:
  About ( QWidget * parent = 0, Qt::WFlags f = 0 ) :
  QDialog (parent, f) 
  {
    setupUi (this);
  }

};


#endif
