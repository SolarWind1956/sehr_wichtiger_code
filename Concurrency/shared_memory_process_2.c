//	shared_memory_process_2.c
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
	printf("I`m ---- reader -----  %s()\n", __PRETTY_FUNCTION__);
	
	HANDLE 		hMtx;
	printf("Ждём создания и открытия мьютекса\n");
	do {
			hMtx	=	OpenMutex	(	SYNCHRONIZE 
									,	TRUE
									,	"SEH_Mutex"
									);
			Sleep(2000);							
	}	while 	(hMtx == NULL);
	printf("Handl открытого мьютекса %p\n", hMtx);
	printf("Ждём освобождения мьютекса\n");
	WaitForSingleObject	( 	hMtx				// handle to mutex
							,   INFINITE  			// no time-out interval										
							);
	printf("Дождались освобождения мьютекса\n");

	
	printf("Пытаемся считать разделяемую память\n");
	HANDLE 		hFile 		= 	OpenFileMapping	(	FILE_MAP_READ 		// read access
													,	FALSE 				// do not inherit handle		
													,	"gfgfgf" 			// name of mapping object
													);
	LPDWORD 	lp 			= 	(LPDWORD) MapViewOfFile 	(	hFile
															, 	FILE_MAP_READ
															, 	0
															, 	0
															, 	8
															);
	//DWORD 		dw 			= 	GetLastError ();
	DWORD 		buf			= 	*lp;

	printf("Принято число %ld. Чтобы завершить 2-й процесс, press Enter\n", buf);
	
	system("pause");
	CloseHandle(hMtx);
	CloseHandle(hFile);
    return 0;
}

