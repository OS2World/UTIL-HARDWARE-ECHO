
#include <windows.h>
#include <initguid.h>
#include <propkeydef.h>

#include <ctype.h>
#include <devguid.h>
#include <regstr.h>
#include <stdio.h>
#include <setupapi.h>
#include <tchar.h>


#include "internal.h" 


static int parse_hex (_TCHAR *str)
{
  int ret = 0, n = 0, p = 0;
  int j = _tcslen (str);

  while (j > 0)
  {
    n = _tcsnextc (str);
    p = 0;

    if (n >= '0' && n <= '9')
    {
      p = (n - '0');
    }
    else if (n >= 'A' && n <= 'F')
    {
      p = (n - 'A' + 10);
    }
    else if (n >= 'a' && n <= 'f')
    {
      p = (n - 'a' + 10);
    }
    else
    {
      break;
    }

    ret = (ret << 4) + p;
    str++;
    j--;
  }
  return ret;
}



/*
  Helper function to fetch an integer (DWORD) property. Returns -1 on error
*/
static DWORD getDevDword (HDEVINFO DevInfo, PSP_DEVINFO_DATA DeviceInfoData, DWORD PropertyID)
{
  DWORD buffersize;
  DWORD buffer = 0;
  DWORD DataT = 0;

  buffer = -1;
  buffersize = sizeof (buffer);
  SetupDiGetDeviceRegistryProperty(
		DevInfo,
		DeviceInfoData,
		PropertyID,
		&DataT,
		(PBYTE)&buffer,
		buffersize,
		&buffersize);
  return buffer;
}

/*
  Helper function to fetch a string property. The string is allocated on the heap and must be free()d with LocalFree!
*/

static LPTSTR getDevString (HDEVINFO DevInfo, PSP_DEVINFO_DATA DeviceInfoData, DWORD PropertyID)
{
  DWORD buffersize;
  PBYTE buffer = 0;
  DWORD DataT = 0;

  buffersize = 0;
  while (!SetupDiGetDeviceRegistryProperty(
		DevInfo,
		DeviceInfoData,
		PropertyID,
		&DataT,
		(PBYTE)buffer,
		buffersize,
		&buffersize))
  {
    if (ERROR_INSUFFICIENT_BUFFER == GetLastError ())
    {
      // Change the buffer size.
      if (buffer) 
      {
        LocalFree(buffer);
        buffer = 0;
      }
      // Double the size to avoid problems on 
      // W2k MBCS systems per KB 888609. 
      buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
    }
    else
    {
      // Insert error handling here.
      if (buffer)
      {
        LocalFree (buffer);
        buffer = 0;
      }
      break;
    }
  }
  return buffer;
}

static int
win_api_detect(struct pci_access *a)
{
  return 1;
}

static void
win_api_init (struct pci_access *a)
{
  // NOP
}
static void 
win_api_cleanup (struct pci_access *a)
{
  // NOP
}

static void 
win_api_scan (struct pci_access *a)
{
  HDEVINFO hDevInfo;
  SP_DEVINFO_DATA DeviceInfoData;
  DWORD i;
  struct pci_dev *t = 0;
  int di = 0;


  // Get PCI IDs, classes, etc. by parsing strings from the device manager.

  // Create a HDEVINFO with all present devices.
  hDevInfo = SetupDiGetClassDevs(NULL,
	  0, // Enumerator
	  0,
	  DIGCF_PRESENT | DIGCF_ALLCLASSES );

  if (hDevInfo == INVALID_HANDLE_VALUE)
  {
	// Insert error handling here.
	return;
  }

  // Enumerate through all devices in Set.
  DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
  for (i = 0; di = SetupDiEnumDeviceInfo (hDevInfo, i, &DeviceInfoData); i++)
  {
	LPTSTR hardware_id_str = NULL;
    LPTSTR service_str = NULL;
    LPTSTR enumerator_str = NULL;
	_TCHAR *s = 0;
    _TCHAR *dev_id_str = 0;
    DWORD longest_dev_id = 0;

    int newBusNumber = 0;
    int newVendorId = -1;
    int newDeviceId = -1;
    int newFunc = -1;
    int newClass = -1;



    newBusNumber = getDevDword (hDevInfo, &DeviceInfoData, SPDRP_BUSNUMBER);

    hardware_id_str = getDevString (hDevInfo, &DeviceInfoData, SPDRP_HARDWAREID);
    if (NULL != hardware_id_str)
    {
      // Search strings, assume longest has the most information
	  s = (_TCHAR *)hardware_id_str;
      dev_id_str = s;
      longest_dev_id = _tcslen(s);
	  while (_tcslen (s) > 0)
	  {
        if (_tcslen (s) > longest_dev_id)
        {
          longest_dev_id = _tcslen(s);
          dev_id_str = s;
        }
	    s += _tcslen(s);
	    s++;
      }

      // Parse it... well, first search if string starts with PIC or AUDIO
      if (0 != dev_id_str)
      {
        if (0 == _tcsncmp (_T("PCI"), dev_id_str, 3) || 0 == _tcsncmp(_T("HDAUDIO"), dev_id_str, 7))
        {
          // Search for separating backslash (\)
          _TCHAR *context = 0, *tok = 0;
          _TCHAR *back = _tcschr (dev_id_str, '\\');
          if (NULL != back)
          {

            back++;
            tok = _tcstok_s (back, _T("&"), &context);
            while (NULL != tok)
            {
              // FUNC, VEN, DEV, SUBSYS, REV
              if (0 == _tcsncmp (_T("VEN_"), tok, 4))
              {
                newVendorId = parse_hex (tok + 4);
              }
              if (0 == _tcsncmp (_T("DEV_"), tok, 4))
              {
                newDeviceId = parse_hex (tok + 4);
              }
              if (0 == _tcsncmp(_T("FUNC_"), tok, 5))
              {
                newFunc = parse_hex (tok + 5);
              }
              tok = _tcstok_s (NULL, _T("&"), &context);
            }

          } // ..if back
        } // ..if "PCI" | "HDAUDIO"
      } // ..if dev_id_str
      LocalFree(hardware_id_str);
    }

#if 0
    service_str = getDevString (hDevInfo, &DeviceInfoData, SPDRP_SERVICE);
    if (NULL != service_str)
    {
      /* Hmm... where do we store the UHCI/OHCI/EHCI subselection? */
      if (!_tcscmp (_T("usbuhci"), service_str))
      {
        newClass = 0x0c03;
      }
      if (!_tcscmp (_T("usbohci"), service_str))
      {
        newClass = 0x0c03;
      }
      if (!_tcscmp (_T("usbehci"), service_str))
      {
        newClass = 0x0c03;
      }
      LocalFree (service_str);
    }
#endif

    // Get "CC" class, which is the hexadecimal representation of the PCI class and subclass
    service_str = getDevString (hDevInfo, &DeviceInfoData, SPDRP_COMPATIBLEIDS);
    newClass = 0;
    if (NULL != service_str)
    {
	  s = (_TCHAR *)service_str;
	  while (_tcslen (s) > 0)
      {
        if (0 == _tcsncicmp (s, _T("PCI\\CC_"), 7) && 11 == _tcslen (s))
        {
          // parse hex string
          newClass = parse_hex (s + 7);
          break;
        }
	    s += _tcslen(s);
	    s++;
      }
      LocalFree (service_str);
    }

    /* If we have something useful, store in new class */
    if (newVendorId >=0 && newDeviceId >= 0)
    {
      t = pci_alloc_dev(a);
      if (NULL != t)
      {
        t->known_fields = PCI_FILL_IDENT;
        t->vendor_id = newVendorId & 0xffff;
        t->device_id = newDeviceId & 0xffff;
        if (newFunc >= 0)
        {
          t->func = newFunc & 0xff;
        }
        if (newBusNumber >= 0)
        {
          t->bus = newBusNumber & 0xff;
        }
        if (newClass >= 0)
        {
          t->device_class = newClass;
          t->known_fields |= PCI_FILL_CLASS;
        }
        pci_link_dev(a, t);
      }
    } // ..if newVendorId newDeviceId

  } // ..for 
  //  Cleanup
  SetupDiDestroyDeviceInfoList(hDevInfo);
}





struct pci_methods pm_win_api_conf = {
  "intel-win-api",
  "PCI access (very limited) using Windows API calls",
  NULL,					/* config */
  win_api_detect,
  win_api_init,    
  win_api_cleanup, 
  win_api_scan,
  NULL, // pci_generic_fill_info,
  NULL, // win_api_read,
  NULL, // win_api_write,
  NULL,					/* read_vpd */
  NULL,					/* init_dev */
  NULL					/* cleanup_dev */
};

