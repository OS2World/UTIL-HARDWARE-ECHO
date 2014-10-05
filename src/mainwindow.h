/* 
 * File:   mainwindow.h
 * Author: jvoosten
 *
 * Created on March 30, 2011, 4:37 PM
 */

#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include <QtCore/QHash>
#include <QtCore/QSettings>
#include <QtGui/QProgressDialog>
#include <QtNetwork/QtNetwork>

#include "ui_mainwindow.h"

#include "PCITableModel.h"

class MainWindow: public QMainWindow, public Ui::MainWindow
{
   Q_OBJECT
public:
  MainWindow( QWidget * parent = 0, Qt::WFlags f = 0 );

private slots:
  void about ();

  void fetchSettings ();
  void fetchHardwareDB ();

  void downloading (qint64, qint64);// While downloading stuff
  void download_error (QNetworkReply::NetworkError); //
  void download_finished ();
  void download_timeout ();
  
private:
  enum DownloadSection { None, Settings, HardwareDB } m_downloadWhat; // What we are downloading
  enum DownloadStatus { NoDownload, InProgress, Success, Canceled, Error, Ignored }; // What the result of the download was
  
  QTimer m_actionTimer;
  QNetworkAccessManager *m_nam;
  QNetworkReply *m_downloader;
  QProgressDialog m_progress;
  QString m_downloadDescription;

  PCITableModel m_pciData;

  QHash<qint32, PCIDevice> m_supportedDevices;
  QSettings m_settings;

  void startDownload (DownloadSection section, const QString &msg, const QString &url);
  void afterDownload (DownloadStatus status);

  int parseSettings (QIODevice &file);

  int ParseDevicesList (QIODevice &file);
  void PushDevice (PCIDevice dev);
  bool FillPCITable ();
};

#endif	/* MAINWINDOW_H */

