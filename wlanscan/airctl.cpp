#include "StdAfx.h"
#include "airctl.h"


/**
* maybe increase it for yor own pupose
*/
#define SIZEOF_DEVICE_NAME 256
/**
*Contructor... set them NULL
*/
airctl::airctl(void)
{
	m_handle =NULL;
	m_devices = NULL;
	m_pBSSIDList = NULL;

}

/**
*Destructor.. free the memory
*/
airctl::~airctl(void)
{
	if(m_handle != NULL)
		CloseHandle( m_handle);

	clearDeviceList();
}	

/**
* maybe increase it for yor own pupose
*/
#define NUMBEROF_BSSIDS 10  

/**
* Scan for wlans.
* the class will free it every scan
* @return returns the list...
*/
NDIS_802_11_BSSID_LIST* airctl::scan(void)
{


ULONG oidcode ;
ULONG bytesreturned ;

clearDeviceList();

m_pBSSIDList = ( NDIS_802_11_BSSID_LIST *) VirtualAlloc(  NULL,
                                                        sizeof( NDIS_802_11_BSSID_LIST) * NUMBEROF_BSSIDS,
                                                        MEM_RESERVE | MEM_COMMIT,
                                                        PAGE_READWRITE) ;

if( m_pBSSIDList == NULL)
{
	return NULL;//error
}
else
{
        memset( m_pBSSIDList, 0, sizeof( NDIS_802_11_BSSID_LIST) * NUMBEROF_BSSIDS) ;
        oidcode = OID_802_11_BSSID_LIST_SCAN ;

        DeviceIoControl(        m_handle,
                                IOCTL_NDIS_QUERY_GLOBAL_STATS,
                                &oidcode,
                                sizeof( oidcode),
                                ( ULONG *) NULL,
                                0,
                                &bytesreturned,
                                NULL) ;

        Sleep( 2000) ;// i dont know...its seems we give him some time to let me know

        memset( m_pBSSIDList, 0, sizeof( NDIS_802_11_BSSID_LIST) * NUMBEROF_BSSIDS) ;
        oidcode = OID_802_11_BSSID_LIST ;

        if( DeviceIoControl(    m_handle,
                                IOCTL_NDIS_QUERY_GLOBAL_STATS,
                                &oidcode,
                                sizeof( oidcode),
                                ( ULONG *) m_pBSSIDList,
                                sizeof( NDIS_802_11_BSSID_LIST) * NUMBEROF_BSSIDS,
                                &bytesreturned,
                                NULL) == 0)
        {
               // List failed
			  return NULL;
        }
        else
        {

			 
			  return m_pBSSIDList;
        }
	}
}

BOOL airctl::get_device_info(   int Index,
                        char *key_name,
                        char *device_info,
                        char *device_description)
{
        HKEY hkey ;
        DWORD size ;
        DWORD type ;
        BOOL retval ;

        retval = FALSE ;

      memset( device_info, 0, SIZEOF_DEVICE_NAME) ;

		if( RegOpenKeyExA(       HKEY_LOCAL_MACHINE,
                                key_name,
                                0,
                                KEY_READ,
                                &hkey) == ERROR_SUCCESS)
        {
                type = REG_SZ ;
                size = SIZEOF_DEVICE_NAME ;

                if( RegQueryValueExA(    hkey,
                                        "ServiceName",
                                        NULL,
                                        &type,
                                        ( BYTE *) device_info,
                                        &size) == ERROR_SUCCESS)
                {
                        type = REG_SZ ;
                        size = SIZEOF_DEVICE_NAME ;

                        if( RegQueryValueExA(    hkey,
                                                "Description",
                                                NULL,
                                                &type,
                                                ( BYTE *) device_description,
                                                &size) == ERROR_SUCCESS)
                        {
                                retval = TRUE ;
                        }
                }

                RegCloseKey( hkey) ;
        }

        return retval ;
}
/**
* Fills his own list with devices
* @return returns true if there are some devices
*/
BOOL airctl::list_devices( void)
{
		
        char key_name[ SIZEOF_DEVICE_NAME] ;
        char full_name[ SIZEOF_DEVICE_NAME] ;
        char device_info[ SIZEOF_DEVICE_NAME] ;
        char device_description[ SIZEOF_DEVICE_NAME] ;

		this->clearDeviceList ();

        FILETIME file_time ;

        HKEY hkey ;
        int index ;
        DWORD size ;

        index = 0 ;

		if( RegOpenKeyExA(       HKEY_LOCAL_MACHINE,
                                "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards",
                                0,
                                KEY_READ,
                                &hkey) == ERROR_SUCCESS)
        {
                size = SIZEOF_DEVICE_NAME ;

                while(  RegEnumKeyExA(   hkey,
                                        index,
                                        key_name,
                                        &size,
                                        NULL,
                                        NULL,
                                        NULL,
                                        &file_time) == ERROR_SUCCESS)
                {
                        sprintf(        full_name,
                                        "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\%s",
                                        key_name) ;

                        get_device_info(        index,
                                                full_name,
                                                device_info,
                                                device_description) ;

						this->AddDevice(device_description,device_info);
                        printf( "Index= %d\nName= %s\nDesc=%s\nKey=%s\n\n",
                                index + 1,
                                device_info,
                                device_description,
                                full_name) ;
                        index++ ;

                        size = SIZEOF_DEVICE_NAME ;
                }

                RegCloseKey( hkey) ;
        }

        if( index == 0)
        {
               return false;// printf( "No devices found\n\n") ;
        }

        return TRUE ;
}


/**
*Opens the Device and saves the handle
*/
bool airctl::open( char *device_name)
{
        
        char device_file[ SIZEOF_DEVICE_NAME] ;

        sprintf( device_file, "\\\\.\\%s", device_name) ;

        m_handle = CreateFileA(   device_file,
                                GENERIC_READ,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_EXISTING,
                                0,
                                NULL) ;

		if( m_handle == INVALID_HANDLE_VALUE)
        {
               return false;// printf( "Error: Device not available\n\n") ;
        }
        else
        {
               // ... open
			return true;//printf( "Open Device:%s\n\n",device_name) ;

        }

    //    CloseHandle( hdevice) ;
}


// adds one device to the list
void airctl::AddDevice(char * desc, char * name)
{	
	deviceInfo *tmp= this->m_devices;
	deviceInfo *xtmp;


	xtmp = new deviceInfo;
		xtmp->description = new char[strlen(desc)+1];
		xtmp->name = new char[strlen(name)+1];
		strcpy(xtmp->description,desc);
		strcpy(xtmp->name,name);
		xtmp->next = NULL;


	if(tmp != NULL)
	{
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next= xtmp;
	}
	else
	{
		m_devices = xtmp;
	}

	

}

// clears the intern list
void airctl::clearDeviceList(void)
{
	if(m_devices == NULL)
	{
		return;
	}

	deviceInfo *tmp;
	while (m_devices != NULL)
	{
		tmp =m_devices->next;
		delete[] m_devices->name;
		delete[] m_devices->description ;
		delete m_devices;
		m_devices =tmp;
	}

}

// Frees the list of wlans
void airctl::freeScanList(void)
{
	if(m_pBSSIDList !=NULL){
		::VirtualFree(m_pBSSIDList,sizeof( NDIS_802_11_BSSID_LIST) * NUMBEROF_BSSIDS,0);
		m_pBSSIDList =NULL;
	}
}
