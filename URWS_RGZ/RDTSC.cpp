#include <windows.h> 
#include <Lmcons.h>
// Read Time Stamp Counter
int getCPUInfo()
{
	int iedx;
	_asm
	{

		mov eax, 1
		CPUID
		mov iedx, edx
		; test edx, 0x04
		; jnz YES

		; YES:
		; mov iedx, 1
	}
	if (iedx & 0b0100 != 0)
	{
		return 1;
	}
	return 0;
}

extern "C" __declspec(dllexport) void getRDTSCInfo(int RDTSC)
{
	int value = 0;

	value = getCPUInfo();

}