/* 
 * File:   PCITableModel.h
 * Author: jvoosten
 *
 * Created on March 30, 2011, 5:27 PM
 */

#ifndef PCITABLEMODEL_H
#define	PCITABLEMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QVector>

// Copied from pci_dev
struct PCIDevice
{
  int vendorID;
  int productID;
  int classID;

  QString className;

  QString name;
  QString notes;
  int supported; // 0 = no, 1 = yes, with info, 2 = yes, with quirks, 3 = yes, fully

  PCIDevice ()
  {
    clear();
  }

  void clear ()
  {
    vendorID = 0;
    productID = 0;
    classID = 0;

    className = "";
    name = "";
    notes = "";
    supported = 0;
  }

  PCIDevice &merge (const PCIDevice &src)
  {
    name = src.name;
    notes = src.notes;
    supported = src.supported;
    
    return *this;
  }

};


class PCITableModel : public QAbstractTableModel
{
public:
  PCITableModel (QObject *parent = 0);

  Qt::ItemFlags flags (const QModelIndex & index) const;

  int rowCount (const QModelIndex & parent) const;
  int columnCount (const QModelIndex & parent) const;

  QVariant headerData (int section, Qt::Orientation orientation, int role) const;
  QVariant data (const QModelIndex &index, int role) const;
  
  void addDevice (const PCIDevice &dev);

private:
  QList<PCIDevice *> m_devices;
};

#endif	/* PCITABLEMODEL_H */
