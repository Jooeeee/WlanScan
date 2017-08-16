#pragma once

#include "winioctl.h"
#include "ntddndis.h"

struct deviceInfo
{
	char *description;
	char *name;
	deviceInfo* next;
};

class airctl
{
	HANDLE m_handle;
	deviceInfo* m_devices;
public:
	airctl(void);
		// Frees the list of wlans
	void freeScanList(void);
	NDIS_802_11_BSSID_LIST* scan(void);

bool open( char *device_name);
	BOOL list_devices( void);
	inline deviceInfo* getDevList( ){return m_devices;};
	
public:
	~airctl(void);
private:
	// adds one device to the list
	void AddDevice(char * desc, char * name);
	// clears the intern list
	void clearDeviceList(void);
	NDIS_802_11_BSSID_LIST* m_pBSSIDList;

	BOOL get_device_info(   int Index,
                        char *key_name,
                        char *device_info,
                        char *device_description);
	

};
