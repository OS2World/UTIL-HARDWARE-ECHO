/*
 * File:   main.cpp
 * Author: jvoosten
 *
 * Created on March 29, 2011, 4:24 PM
 */

#include <QtGui/QApplication>

#include "mainwindow.h"

int main (int argc, char *argv[])
{
  // initialize resources, if needed
  // Q_INIT_RESOURCE(resfile);

  QApplication app (argc, argv);
  MainWindow mw;
  mw.show ();
  return app.exec ();
}
