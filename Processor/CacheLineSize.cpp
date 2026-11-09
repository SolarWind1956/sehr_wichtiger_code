#include <iostream>
using std::cout; using std::endl; 
#include <stdlib.h>

#include <windows.h>

#include "CacheLineSize.hpp"
size_t 	CacheLineSize() 
{
	size_t 									lineSize 	= 0;
	DWORD 									bufferSize 	= 0;
	DWORD 									i 			= 0;
	SYSTEM_LOGICAL_PROCESSOR_INFORMATION * 	buffer 		= 0;

	GetLogicalProcessorInformation	(		0
										, 	&bufferSize
									);
									
	buffer 	= (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *) malloc(bufferSize);
	
	GetLogicalProcessorInformation	(		&buffer[0]
										, 	&bufferSize
									);

	for (i = 0; i != bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) 
	{
		if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) 
		{
			lineSize = buffer[i].Cache.LineSize;
			break;
		}
	}

	free(buffer);
	return lineSize;
}

int		main()
{
	cout << "Размер строки кэш == " << CacheLineSize() << endl;
	return 0;
}