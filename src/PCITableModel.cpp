#include <QtGui/QIcon>

#include "PCITableModel.h"



PCITableModel::PCITableModel (QObject *parent)
  : QAbstractTableModel (parent)
{
}


Qt::ItemFlags PCITableModel::flags (const QModelIndex & index) const
{
  return Qt::ItemIsEnabled;
}


int PCITableModel::rowCount (const QModelIndex & parent) const
{
  return m_devices.size ();
}

int PCITableModel::columnCount (const QModelIndex & parent) const
{
  return 5;
}



QVariant PCITableModel::headerData (int section, Qt::Orientation orientation, int role) const
{
   if (Qt::DisplayRole != role)
   {
     return QVariant ();
   }

   if (orientation == Qt::Horizontal)
   {
     switch (section)
     {
       case 0: return QString ("VendorID"); break;
       case 1: return QString ("ProductID"); break;
       case 2: return QString ("Class"); break;
       case 3: return QString ("Description"); break;
       case 4: return QString ("Notes"); break;
       default: return QString (""); break;
     }
   }
   else
   {
     return QString ("Row %1").arg (1 + section);
   }
}

QVariant PCITableModel::data (const QModelIndex &index, int role) const
{
  if (!index.isValid ())
  {
    return QVariant ();
  }
  if (index.row() >= m_devices.size())
  {
    return QVariant();
  }

  PCIDevice *dev = m_devices[index.row ()];
  if (Qt::DisplayRole == role)
  {
    switch (index.column())
    {
      case 0: return QString ("0x%1").arg (dev->vendorID,  4, 16, QLatin1Char ('0')); break;
      case 1: return QString ("0x%1").arg (dev->productID, 4, 16, QLatin1Char ('0')); break;
      case 2: return dev->className; break;
      case 3: return QString (dev->name); break;
      case 4: return QString (dev->notes); break;
      default: return QVariant ("?"); break;
    }
  }
  if (Qt::DecorationRole == role && 0 == index.column())
  {
    switch (dev->supported)
    {
      case 0: return QVariant (QIcon (":/images/error.png")); break;
      case 1: return QVariant (QIcon (":/images/info.png")); break;
      case 2: return QVariant (QIcon (":/images/warning.png")); break;
      case 3: return QVariant (QIcon (":/images/success.png")); break;
    }
  }
  return QVariant();
}


void PCITableModel::addDevice (const PCIDevice &dev)
{
  beginInsertRows (QModelIndex () , m_devices.size(), m_devices.size());
  m_devices.append (new PCIDevice (dev)); // make copy
//  m_sortedDevices.append (&dev);
  endInsertRows ();
}
