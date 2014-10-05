#include <QtCore/QtDebug>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QMessageBox>
#include <QtGui/QStatusBar>

extern "C" {
#include "pci/pci.h"
}
#include "zlib.h"

#include "about.h"
#include "mainwindow.h"

MainWindow::MainWindow (QWidget * parent, Qt::WFlags f)
  : QMainWindow (parent, f),
    m_progress (this, 0), m_settings ("eComStation", "echo")
{
  m_downloader = 0;
  m_downloadWhat = None;
  m_nam = new QNetworkAccessManager (this);

  setupUi (this);

  connect (action_Quit, SIGNAL (triggered ()), qApp, SLOT (quit ()));
  connect (action_About, SIGNAL (triggered ()), this, SLOT (about ()));

  //m_pciData.setHeaderData (0, Qt::Horizontal, "VendorID", Qt::DisplayRole);
  tableView->setSortingEnabled (true);
  tableView->setModel (&m_pciData);

  m_progress.setMinimumDuration (500);
  m_progress.setAutoClose (false);


  // Start first download when we are shown; we can re-trigger this timer when we click on
  // "Retry" in the error handling
  m_actionTimer.setSingleShot (true);
  connect (&m_actionTimer, SIGNAL (timeout ()), this, SLOT (fetchSettings ()));
  m_actionTimer.start (50);
}

/**
 * \brief Attempt to download hardware database
 */
void MainWindow::fetchHardwareDB ()
{
  startDownload (HardwareDB, "latest hardware database", "http://www.ecomstation.com/support/devices.lst.gz");
}

/**
 * \brief Attempt to download configuration file
 */
void MainWindow::fetchSettings ()
{
  startDownload (Settings, "ECHO configuration", "http://www.ecomstation.com/support/echo.ini");
}

void MainWindow::downloading (qint64 received, qint64 total)
{
  //qDebug ("downloading: %lld, %lld", received, total);
  if (QNetworkReply::NoError == m_downloader->error())
  {
    m_progress.setMaximum (total);
    m_progress.setValue (received);
  }
  if (m_progress.wasCanceled ())
  {
    m_downloader->abort ();
  }
}

void MainWindow::download_error (QNetworkReply::NetworkError code)
{
  qDebug () << "download error" << code;

  m_progress.reset ();
  m_progress.hide ();

  QString msg;
  QMessageBox::StandardButtons actions = QMessageBox::Abort | QMessageBox::Retry | QMessageBox::Ignore; 
  // pop up error
  switch (code)
  {
    case QNetworkReply::NoError:
      actions = 0;
      break;
    case QNetworkReply::ConnectionRefusedError:
      msg = "the connection was refused by the server.";
      break;

    case QNetworkReply::RemoteHostClosedError:
      msg = "the remote host closed the connection (download was aborted).";
      break;

    case QNetworkReply::HostNotFoundError:
      msg = "the host not found; maybe your DNS settings are incorrect.";
      break;

    case QNetworkReply::TimeoutError:
      msg = "the connection timed out; check if your internet connection is working properly.";
      break;

    case QNetworkReply::OperationCanceledError:
      //msg = "Operation was canceled before it was finished.";
      actions = 0;
      afterDownload (Canceled);
      break;

    case QNetworkReply::SslHandshakeFailedError:
      msg = "it was not possible to set up a SSL (secure) connection.";
      break;

    case QNetworkReply::ProxyConnectionRefusedError:
    case QNetworkReply::ProxyConnectionClosedError:
    case QNetworkReply::ProxyNotFoundError:
    case QNetworkReply::ProxyTimeoutError:
    case QNetworkReply::ProxyAuthenticationRequiredError:
      msg = "there was a HTTP proxy error; check your internet & proxy settings.";
      break;

    case QNetworkReply::ContentAccessDenied:
    case QNetworkReply::ContentOperationNotPermittedError:
    case QNetworkReply::AuthenticationRequiredError:
      msg = "access to the content on the server was denied (may require authentication).";
      break;

    case QNetworkReply::ContentNotFoundError:
      msg = "the file was not found on the server.";
      //actions = 0;
      //afterDownload (Error);
      break;

    case QNetworkReply::ContentReSendError:
      msg = "the request was sent again, but this failed or was denied.";
      break;

    case QNetworkReply::ProtocolUnknownError:
    case QNetworkReply::ProtocolInvalidOperationError:
    case QNetworkReply::ProtocolFailure:
      msg = "there was an error with the underlying protocol.";
      break;

    case QNetworkReply::TemporaryNetworkFailureError:
      msg = "there was a temporary error with the network/internet.";
      break;

    case QNetworkReply::UnknownNetworkError:
    case QNetworkReply::UnknownProxyError:
    case QNetworkReply::UnknownContentError:
      msg = "of an unknown network, proxy or content error.";
      break;
  }

  if (0 != actions)
  {
    if (QMessageBox::Ignore & actions)
    {
      msg += "\nChoosing 'Ignore' will use the internal database, which may be outdated.";
    }

    msg = "There was an error downloading the " + m_downloadDescription + ",\nbecause " + msg;
    // Let the user decide what to do
    QMessageBox::StandardButton q = QMessageBox::question (this, "Error downloading information",
            msg, actions, QMessageBox::Retry);

    switch (q)
    {
      case QMessageBox::Abort:
        qApp->quit ();
        break;

      case QMessageBox::Retry:
        m_downloader->deleteLater ();
        m_actionTimer.start (50); // restart action
        break;

      case QMessageBox::Ignore:
      default:
        afterDownload (Ignored);
        break;
    }
  }
}

void MainWindow::download_finished ()
{
  qDebug () << "download finished";
  if (QNetworkReply::NoError == m_downloader->error())
  {
    afterDownload (Success);
  }
}

void MainWindow::download_timeout ()
{
  qDebug () << "download time out";
  afterDownload (Error);
}

void MainWindow::about ()
{
  About dlg;

  dlg.exec ();
}


void MainWindow::startDownload (DownloadSection section, const QString &msg, const QString &url)
{
  qDebug () << "startDownload(" << msg << "," << url << ")";
  m_downloadWhat = section;

  QUrl req_url (url);
  QNetworkRequest request (req_url);
  m_downloader = m_nam->get (request);
  
  connect (m_downloader, SIGNAL (downloadProgress (qint64, qint64)), this, SLOT (downloading (qint64, qint64)));
  connect (m_downloader, SIGNAL (error (QNetworkReply::NetworkError)), this, SLOT (download_error (QNetworkReply::NetworkError)));
  connect (m_downloader, SIGNAL (finished ()), this, SLOT (download_finished ()));

  // Keep description for later
  m_downloadDescription = msg;
  QString t = "Downloading " + msg + "...";
  this->statusBar ()->showMessage (t);
  m_progress.setLabelText (t);
}

// Common entry point after download action, whether is has finished successfully, was canceled,
// had an error, etc.
void MainWindow::afterDownload (DownloadStatus status)
{
  qDebug() << "afterDownload(" << status << ")";
  
  int ret = 0;
  QString t;
  QFile f;

  this->statusBar ()->clearMessage ();
  m_progress.reset ();
  if (Success != status)
  {
    m_progress.hide ();
  }

  switch (m_downloadWhat)
  {
    case Settings:
      if (Success == status)
      {
        ret = parseSettings (*m_downloader);

        // Trigger next download
        disconnect (&m_actionTimer, SIGNAL (timeout ()), this, 0);
        connect (&m_actionTimer, SIGNAL (timeout ()), this, SLOT (fetchHardwareDB ()));
        m_actionTimer.start (50);
      }
      else
      {
        f.setFileName (":/data/echo.ini");
        ret = parseSettings (f);
      }

      // Update version string
      t = m_labelFound->text ();
      m_settings.beginGroup ("global");
      if (m_settings.contains ("version"))
      {
        m_labelFound->setText (t.replace ("@version@", m_settings.value ("version").toString()));
      }
      m_settings.endGroup ();

      if (Success == status)
      {
        break;
      }
      // else fall-through!

    case HardwareDB:
      // We don't need this anymore.
      m_progress.hide ();
      // Only if we had success downloading the database, parse it.
      if (Success == status)
      {
        ret = ParseDevicesList (*m_downloader);
      }
      else
      {
        ret = 1;
      }

      if (0 != ret)
      {
        this->statusBar ()->showMessage ("Using internal hardware database...");
        f.setFileName(":/data/devices.lst.gz");
        ret = ParseDevicesList (f);
      }

      if (0 != ret)
      {
        qDebug ("ParseDeviceList-int = %d", ret);
        QMessageBox::critical (this, "Hardware database error",
                "The internal hardware database could not be read. This indicates an internal failure or compile error.\n"
                "The application will now exit.",
                QMessageBox::Close);
        qApp->quit ();
        break;
      }

      FillPCITable ();
      tableView->resizeColumnsToContents ();
      break;

    default:
      break;
  }
  m_downloadWhat = None;
  m_downloader->deleteLater ();
}


/**
 \brief Parse INI file

 Unfortunately, Qt can't read QSettings from a stream, so we have to do a little parsing ourselves...
 */
int MainWindow::parseSettings (QIODevice &file)
{
  QString line, section, key, value;
  QRegExp section_reg("\\[(.*)\\]");
  QRegExp line_reg ("([A-Za-z0-9_-]+)=(.*)");

  if (!file.open (QIODevice::ReadOnly))
  {
    qWarning ("Failed to open settings file.");
    return 1;
  }

  QTextStream tstream (&file);
  while (!tstream.atEnd ())
  {
    line = tstream.readLine ().trimmed ();
    if (line.isEmpty ())
    {
      continue;
    }
    if (section_reg.indexIn (line) > -1)
    {
      if (!section.isEmpty())
      {
        m_settings.endGroup ();
      }
      section = section_reg.cap(1);
      m_settings.beginGroup (section);
    }
    else if (line_reg.indexIn (line) > -1)
    {
      key = line_reg.cap(1).toLower();
      value = line_reg.cap(2);
      if (!section.isEmpty ())
      {
        m_settings.setValue (key, value);
      }
    }
  }
  if (!section.isEmpty ())
  {
    m_settings.endGroup();
  }
  file.close ();
  return 0;
}

/**
 Parse text database with supported devices from a QIODevice
 */
int MainWindow::ParseDevicesList (QIODevice &devgz)
{
  // Allocate read buffer
  const qint64 buffer_size = 2048;
  char buffer[buffer_size];
  QByteArray in_buffer, out_buffer;
  int read_len;
  int ret = 0;

  // Read contents
  if (!devgz.open (QIODevice::ReadOnly))
  {
    return -1;
  }

  // Read until empty
  while (!devgz.atEnd ())
  {
    read_len = devgz.read (buffer, buffer_size);
    if (read_len < 0)
    {
      qWarning ("Error occurred while reading devgz");
      ret = -2;
    }
    else
    {
      in_buffer.append (buffer, read_len);
    }
  }
  devgz.close ();

  if (0 == ret)
  {
    // decompress; use 20-fold output buffer
    out_buffer.resize (20 * in_buffer.size ());

    // Now start decompressing...
    z_stream stream;
    int err;

    stream.next_in = (Bytef *) in_buffer.data ();
    stream.avail_in = in_buffer.size ();

    stream.next_out = (Bytef *) out_buffer.data ();
    stream.avail_out = out_buffer.size ();

    // Use default memory functions
    stream.zalloc = (alloc_func) 0;
    stream.zfree = (free_func) 0;

    // Use gzip header
    err = inflateInit2 (&stream, 32 + 15);
    if (err != Z_OK)
    {
      ret = -3;
    }
    else
    {
      err = inflate (&stream, Z_FINISH);
      if (err != Z_STREAM_END)
      {
        inflateEnd (&stream);
        if (err == Z_NEED_DICT || (err == Z_BUF_ERROR && stream.avail_in == 0))
        {
          ret = -3;
        }
      }
    }
    err = inflateEnd (&stream);
    if (err != Z_OK)
    {
      ret = -3;
    }
    if (err != 0)
    {
      qWarning ("Decompression error = %d", err);
    }
  }

  // We now have a decompressed stream; pull it through a textstream and start parsing lines
  if (0 == ret)
  {
    QTextStream db (out_buffer);
    QString line;
    QStringList fields;
    int sub_line = 0;
    int line_no = 0;
    PCIDevice new_dev;

    new_dev.clear ();
    while (!db.atEnd ())
    {
      line = db.readLine ();
      line_no++;

      if (line.isEmpty () || line.startsWith ("#"))
      {
        continue;
      }

      fields = line.split ("\t");
      if (fields.size () < 2 && db.atEnd ())
      {
        break;
      }

      switch (sub_line)
      {
        case 0:
          PushDevice (new_dev);
          new_dev.clear ();
          if (21 != fields.size ())
          {
            //            qWarning ("%d %s %s \"%s\"", fields.size (), line.isNull() ? "null" : "not-null", db.atEnd() ? "end" : "not-end", qPrintable (line));
            qWarning ("Parse error (0) on line %d", line_no);
          }
          else
          {
            int vid, pid;
            bool ok = true;

            vid = fields[0].toInt (&ok, 16);
            if (ok)
            {
              pid = fields[1].toInt (&ok, 16);
            }
            if (ok && vid > 0 && pid > 0)
            {
              new_dev.vendorID = vid;
              new_dev.productID = pid;
              new_dev.supported = 3;
            }
            sub_line++;
          }
          break;

        case 1:
          if (fields.size () < 2)
          {
            qWarning ("Parse error (1) on line %d", line_no);
            sub_line = 0;
          }
          else
          {
            new_dev.name = fields[1];
            sub_line++;
          }
          break;


        case 2:
          if (fields.size () < 3)
          {
            qWarning ("Parse error (2) on line %d", line_no);
          }
          else
          {
            new_dev.notes = fields[2];
          }
          sub_line = 0;
          break;
      } // ..switch sub_line
    } // ..while !end
    PushDevice (new_dev); // push last remaining device
  }
  return ret;
}

/**
 Push device onto the list the list of supported devices. Also sets compatibility level
 */
void MainWindow::PushDevice (PCIDevice dev)
{
  if (0 != dev.vendorID && 0 != dev.productID)
  {
    // Push to list of supported devices
    qint32 key = dev.vendorID << 16 | dev.productID;

    // TODO: differentiate with quirks and info
    dev.supported = 3;

    // Remove useless comments
    if (dev.notes.startsWith ("_"))
    {
      dev.notes = "";
    }
    m_supportedDevices[key] = dev;
  }

}

/**
 Use libpci to gather information about the installed devices
 */
bool MainWindow::FillPCITable ()
{
  bool ret = true;
  struct pci_access *pAccess = 0;

  qDebug ("FillPCITable");
  pAccess = pci_alloc ();
  if (0 == pAccess)
  {
    qWarning ("pci_alloc failed");
    return false;
  }
  pAccess->method = PCI_ACCESS_AUTO;
  pAccess->writeable = 0;
  pAccess->buscentric = 0;
  pAccess->debugging = 1;
  pAccess->id_lookup_mode |= (PCI_LOOKUP_NETWORK | PCI_LOOKUP_CACHE);
  //pAccess->error = die;

  pci_init (pAccess);

  pci_scan_bus (pAccess);
  struct pci_dev *pDev = pAccess->devices;
  PCIDevice MyDev;

  while (0 != pDev)
  {
    char classbuf[128], devbuf[128];
    qint32 key;
    //qDebug("0x%04x, 0x%04x", pDev->vendor_id, pDev->device_id);

    MyDev.clear ();
    MyDev.vendorID = pDev->vendor_id;
    MyDev.productID = pDev->device_id;
    MyDev.classID = pDev->device_class;

    if (pDev->known_fields & PCI_FILL_CLASS)
    {
      MyDev.className = pci_lookup_name (pAccess, classbuf, sizeof (classbuf),
              PCI_LOOKUP_CLASS,
              pDev->device_class);
    }
    else
    {
      MyDev.className = "-";
    }
    MyDev.name = pci_lookup_name (pAccess, devbuf, sizeof (devbuf),
            PCI_LOOKUP_VENDOR | PCI_LOOKUP_DEVICE,
            pDev->vendor_id, pDev->device_id);

    key = pDev->vendor_id << 16 | pDev->device_id;
    if (m_supportedDevices.contains (key))
    {
      // Copy relevant information from supported devices list
      MyDev.merge (m_supportedDevices[key]);
    }

    // Check settings for hidden/special classes
    QString class_x = QString ("%1").arg (MyDev.classID,  4, 16, QLatin1Char ('0'));
    m_settings.beginGroup ("hide-classes");
    if (!m_settings.contains (class_x))
    {
      m_settings.endGroup ();
      m_settings.beginGroup ("warn-classes");
      if (m_settings.contains (class_x))
      {
        MyDev.supported = 2;
        MyDev.notes = m_settings.value (class_x).toString ();
      }
      m_settings.endGroup ();
      m_settings.beginGroup ("info-classes");
      if (m_settings.contains (class_x))
      {
        MyDev.supported = 1;
        MyDev.notes = m_settings.value (class_x).toString ();
      }

      m_pciData.addDevice (MyDev);
    }
    m_settings.endGroup ();

    pDev = pDev->next;
  }

  pci_cleanup (pAccess);
  return ret;
}


