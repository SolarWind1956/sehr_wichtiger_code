#include <iostream>
using std::cout; using std::endl; 
#include <stdlib.h>
#include <windows.h>
#include <vector>
#include <tuple>

struct CacheInfo {
    size_t l1Data 		= 0;
    size_t l1Instruction 	= 0;
	size_t l1LineSize 	= 0;
	
    size_t l2 			= 0;
	size_t l2LineSize 	= 0;
	
    size_t l3 			= 0;
	size_t l3LineSize 	= 0;
};

CacheInfo 	GetCacheParams() 
{
    CacheInfo 			sizes;
    DWORD 				bufferSize = 0;

    // 	Сначала узнаем размер необходимого буфера
    GetLogicalProcessorInformationEx(RelationCache, nullptr, &bufferSize);
    
    std::vector<BYTE> 	buffer(bufferSize);
	
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX ptr = 
        reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data());

    if (GetLogicalProcessorInformationEx(RelationCache, ptr, &bufferSize)) {
        DWORD offset = 0;
		int struct_count{};
        while (offset < bufferSize) {
			++struct_count;
            // 	Переходим к следующей структуре в буфере
            ptr = reinterpret_cast<PSYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(buffer.data() + offset);
            
            if (ptr->Relationship == RelationCache) 
			{
                const auto& cache = ptr->Cache;
                if (cache.Level == 1) {
                    if (cache.Type == CacheData) {
						sizes.l1Data 		= cache.CacheSize;
					}	
                    else if (cache.Type == CacheInstruction) {
						sizes.l1Instruction 	= cache.CacheSize;
					}
					sizes.l1LineSize 		= cache.LineSize;
                } 
                else if (cache.Level == 2) {
					sizes.l2 				= cache.CacheSize;
					sizes.l2LineSize 		= cache.LineSize;
				}	
                else if (cache.Level == 3) {
					sizes.l3 				= cache.CacheSize;
					sizes.l3LineSize 		= cache.LineSize;
				}	
            }
            offset += ptr->Size;
        }
		cout << "Всего структур в буфре : " << struct_count << endl;
    }
    return sizes;
}

int		main()
{
	auto info = GetCacheParams();
    cout << "L1 Data: " << info.l1Data / 1024 << " KB\n";
    cout << "L1 Instruction: " << info.l1Instruction / 1024 << " KB\n";
	cout << "L1 LineSize: " << info.l1LineSize << " B\n\n";
    cout << "L2: " << info.l2 / 1024 << " KB\n";
	cout << "L2 LineSize: " << info.l2LineSize << " B\n\n";
    cout << "L3: " << info.l3 / 1024 << " KB\n";
	cout << "L3 LineSize: " << info.l3LineSize << " B\n\n";

	return 0;
}