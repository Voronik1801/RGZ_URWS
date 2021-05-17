#include <windows.h> 
#include <WinUser.h>

extern "C" __declspec(dllexport) int getMonitorInfo(int mode)
{
	return GetSystemMetrics(mode);
}