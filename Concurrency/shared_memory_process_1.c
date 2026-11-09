//	shared_memory_process_1.c
/*
	*******************************************************************************************
	https://forum.ixbt.com/topic.cgi?id=26:37124
	
	*******************************************************************************************
	
*/			
#include <synchapi.h>
#include <windows.h>
#include <stdio.h>

int main()
{
	printf("I`m ---- writer ----- %s()\n", __PRETTY_FUNCTION__);
	
	HANDLE 		hFile 	= 	CreateFileMapping	(	INVALID_HANDLE_VALUE 	// use paging file
												,	NULL 					// default security
												,	PAGE_READWRITE 		// read/write access
												,	0
												, 	100 					// buffer size
												,	"gfgfgf" 				// name of mapping object
												);
	
	HANDLE 		hMtx	=	CreateMutexA	(	NULL
											,	TRUE
											,	"SEH_Mutex"
											);
	printf("Handl созданного мьютекса %p\n", hMtx);
	DWORD 		buf 	= 	345;
	
	LPDWORD 	lp 		= 	(LPDWORD) MapViewOfFile 	(	hFile
														, 	FILE_MAP_WRITE | FILE_MAP_READ
														, 	0
														, 	0
														, 	8
														);
	*lp 				= 	buf;
	
	Sleep(10000);
	printf("Спали 10 секунд. Сейчас освободим мьютекс.\n");
	ReleaseMutex(hMtx);	
	
	printf("Передано число %ld. Чтобы завершить 1-й процесс, press Enter\n", buf);
	
	system("pause");
	
	CloseHandle(hMtx);
	CloseHandle(hFile);
    return 0;
}

