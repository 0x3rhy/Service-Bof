#include <windows.h>
#include <stdio.h>
#include "bofdefs.h"



void InstallSCV(LPCTSTR scvName, LPCTSTR szBin)
{

	SC_HANDLE scmHandle = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (scmHandle == NULL)
	{
		BeaconPrintf(CALLBACK_ERROR, "OpenSCManager failed (%u)\n", GetLastError());
		return;
	}
	BeaconPrintf(CALLBACK_OUTPUT, "[+] OpenSCManager success!\n");

	SC_HANDLE scService = CreateServiceA(
		scmHandle,              // SCM database 
		scvName,                   // name of service 
		scvName,                   // service name to display 
		SERVICE_ALL_ACCESS,        // desired access 
		SERVICE_WIN32_OWN_PROCESS, // service type 
		SERVICE_AUTO_START,      // start type 
		SERVICE_ERROR_IGNORE,      // error control type 
		szBin,                    // path to service's binary 
		NULL,                      // no load ordering group 
		NULL,                      // no tag identifier 
		NULL,                      // no dependencies 
		NULL,                      // LocalSystem account 
		NULL);

	if (scService == NULL)
	{
		BeaconPrintf(CALLBACK_ERROR, "CreateService failed (%u)\n", GetLastError());
		CloseServiceHandle(scmHandle);
		return;
	}
	BeaconPrintf(CALLBACK_OUTPUT, "[+] CreateService %s success!\n", scvName);

	CloseServiceHandle(scService);
	CloseServiceHandle(scmHandle);

}

void RmSCV(LPCTSTR scvName)
{
	SC_HANDLE scmHandle = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == scmHandle)
	{
		BeaconPrintf(CALLBACK_ERROR, "OpenSCManager failed (%u)\n", GetLastError());
		return;
	}
	BeaconPrintf(CALLBACK_OUTPUT, "[+] OpenSCManager success!\n");

	SC_HANDLE serviceHandle = OpenServiceA(scmHandle, scvName, DELETE);
	if (NULL == serviceHandle) {
		BeaconPrintf(CALLBACK_ERROR, "OpenService %s failed (%u)\n", scvName, GetLastError());
		CloseServiceHandle(scmHandle);
		return;
	}
	BeaconPrintf(CALLBACK_OUTPUT, "[+] OpenService %s success!\n", scvName);

	if (!DeleteService(serviceHandle))
	{
		BeaconPrintf(CALLBACK_ERROR, "DeleteService failed (%u)\n", GetLastError());
		return;
	}
	else
	{
		BeaconPrintf(CALLBACK_OUTPUT, "[+] DeleteService %s success!\n", scvName);
	}

	CloseServiceHandle(serviceHandle);
	CloseServiceHandle(scmHandle);
}


#ifdef BOF
void go(char* buff, int len) {

	if (!BeaconIsAdmin())
	{
		BeaconPrintf(CALLBACK_ERROR, "Requires administrator privileges\n");
		return;
	}

	datap parser;
	BeaconDataParse(&parser, buff, len);

	char* scvName, * szBin, * flag;
	scvName = BeaconDataExtract(&parser, 0);
	szBin = BeaconDataExtract(&parser, 0);
	flag = BeaconDataExtract(&parser, 0);

	char* del = "deleteservice";
	if (_stricmp(flag, del) == 0)
	{
		RmSCV(scvName);
	}
	else
	{
		InstallSCV(scvName, szBin);
	}



}


#else

void main(int argc, char* argv[]) {


}

#endif