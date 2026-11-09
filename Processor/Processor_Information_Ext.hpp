#pragma once
// Processor_Information_Ext.hpp
/*

Блок, относящийся только к системам Майкрософт

Создает инструкцию cpuid, доступную для x86 и x64. 

Эта инструкция запрашивает у процессора информацию о поддерживаемых возможностях и типе ЦП.


void __cpuid(
   int cpuInfo[4],
   int function_id
);

void __cpuidex(
   int cpuInfo[4],
   int function_id,
   int subfunction_id
);

Параметры

Cpuinfo [out] 
Массив из четырех целых чисел, 
содержащих сведения, возвращаемые в EAX, EBX, ECX и EDX о поддерживаемых функциях ЦП.

function_id 
[in] Код, указывающий сведения, передаваемые в EAX.

subfunction_id
[in] Дополнительный код, указывающий сведения для получения, передаваемого в ECX.


Intrinsic	Архитектура
__cpuid	x86, x64
__cpuidex	x86, x64
Файл заголовка <intrin.h>

Замечания

Эта встроенная функция сохраняет поддерживаемые функции и данные ЦП, 
возвращаемые cpuid инструкцией в cpuInfo, 
массив из четырех 32-разрядных целых чисел, 
заполненных значениями регистров EAX, EBX, ECX и EDX (в этом порядке). 

Возвращаемые сведения имеют другое значение в зависимости от значения, 
переданного в качестве параметра function_id. 

Данные, возвращаемые различными значениями function_id, 
зависят от процессора.

Встроенная функция __cpuid очищает регистр ECX перед вызовом инструкции cpuid. 

Встроенные __cpuidex задает значение регистра ECX для subfunction_id перед созданием инструкции cpuid. 

Он позволяет собирать дополнительные сведения о процессоре.

Дополнительные сведения об используемых параметрах и значениях, 
возвращаемых этими встроенными компонентами на процессорах Intel, 
см. в документации по cpuid инструкциям в справочнике по программированию:
расширений наборов инструкций Intel 64 и IA-32 
Для разработчиков программного обеспечения вручную, том 2. 

Справочник по набору инструкций и набор инструкций Intel. 

Документация Intel использует термины "leaf" и "subleaf" 
для параметров function_id и subfunction_id, 
передаваемых в EAX и ECX.

Дополнительные сведения о конкретных параметрах 
для использования и значениях, 
возвращаемых этими встроенными компонентами для процессоров AMD, 
см. в документации cpuid по инструкции в ручном режиме, том 3 
программиста архитектуры AMD64: 

общие и системные инструкции, 
а также в руководствах по редакциям для конкретных семейств процессоров. 

Ссылки на эти документы и другие сведения 
см. в руководствах разработчиков & AMD, на странице "Документы ISA" вручную. 

Документация AMD использует термины "номер функции" и "подфункционный номер" 
для параметров function_id и subfunction_id, 
передаваемых в EAX и ECX.

Если аргумент function_id равен 0, 
cpuInfo[0] возвращает наибольшее доступное не расширенное function_id значение, 
поддерживаемое процессором. 

Производитель процессора закодирован в cpuInfo[1], cpuInfo[2], и cpuInfo[3].

Поддержка определенных расширений наборов инструкций и функций ЦП кодируется в результатах cpuInfo, 
возвращаемых для более высоких function_id значений. 

Дополнительные сведения содержатся в разделе вышеописанные руководства и в следующем примере кода.

Некоторые процессоры поддерживают информацию о расширенной функции CPUID. 

При поддержке function_id значения из 0x80000000 могут использоваться для возврата сведений. 

Чтобы определить максимально допустимое значение, 
задайте function_id значение 0x80000000. 

Максимальное значение function_id, 
поддерживаемое для расширенных функций, 
будет записано в cpuInfo[0].

Пример

В этом примере показаны некоторые сведения, 
доступные через __cpuid и __cpuidex встроенных функций. 

Приложение выводит список расширений набора инструкций, 
поддерживаемых текущим процессором. 

Вывод показывает возможные результаты для конкретного процессора.

Копировать
// InstructionSet.cpp
// Compile by using: cl /EHsc /W4 InstructionSet.cpp
// processor: x86, x64
// Uses the __cpuid intrinsic to get information about
// CPU extended instruction set support.
*/
#include <iostream>
#include <vector>
#include <bitset>
#include <array>
#include <string>
#include <intrin.h>
#include "..\my_short_headers_list.hpp"

class InstructionSet
{
    // forward declarations
    class InstructionSet_Internal;

public:
    // getters
    static 	std::string 	Vendor			(void) 	{ return CPU_Rep.vendor_; }
    static 	std::string 	Brand			(void) 	{ return CPU_Rep.brand_; }

    static 	bool 			SSE3			(void) 	{ return CPU_Rep.f_1_ECX_[0]; }
    static 	bool 			PCLMULQDQ		(void)	{ return CPU_Rep.f_1_ECX_[1]; }
    static 	bool 			MONITOR			(void)	{ return CPU_Rep.f_1_ECX_[3]; }
    static 	bool 			SSSE3			(void)	{ return CPU_Rep.f_1_ECX_[9]; }
    static 	bool 			FMA				(void) 	{ return CPU_Rep.f_1_ECX_[12]; }
    static 	bool 			CMPXCHG16B		(void) 	{ return CPU_Rep.f_1_ECX_[13]; }
    static 	bool 			SSE41			(void) 	{ return CPU_Rep.f_1_ECX_[19]; }
    static 	bool 			SSE42			(void) 	{ return CPU_Rep.f_1_ECX_[20]; }
    static 	bool 			MOVBE			(void) 	{ return CPU_Rep.f_1_ECX_[22]; }
    static 	bool 			POPCNT			(void) 	{ return CPU_Rep.f_1_ECX_[23]; }
    static 	bool 			AES				(void) 	{ return CPU_Rep.f_1_ECX_[25]; }
    static 	bool 			XSAVE			(void) 	{ return CPU_Rep.f_1_ECX_[26]; }
    static 	bool 			OSXSAVE			(void) 	{ return CPU_Rep.f_1_ECX_[27]; }
    static 	bool 			AVX				(void) 	{ return CPU_Rep.f_1_ECX_[28]; }
    static 	bool 			F16C			(void) 	{ return CPU_Rep.f_1_ECX_[29]; }
    static 	bool 			RDRAND			(void) 	{ return CPU_Rep.f_1_ECX_[30]; }

    static 	bool 			MSR				(void) 	{ return CPU_Rep.f_1_EDX_[5]; }
    static 	bool 			CX8				(void) 	{ return CPU_Rep.f_1_EDX_[8]; }
    static 	bool 			SEP				(void) 	{ return CPU_Rep.f_1_EDX_[11]; }
    static 	bool 			CMOV			(void) 	{ return CPU_Rep.f_1_EDX_[15]; }
    static 	bool 			CLFSH			(void) 	{ return CPU_Rep.f_1_EDX_[19]; }
    static 	bool 			MMX				(void) 	{ return CPU_Rep.f_1_EDX_[23]; }
    static 	bool 			FXSR			(void) 	{ return CPU_Rep.f_1_EDX_[24]; }
    static 	bool 			SSE				(void) 	{ return CPU_Rep.f_1_EDX_[25]; }
    static 	bool 			SSE2			(void) 	{ return CPU_Rep.f_1_EDX_[26]; }

    static 	bool 			FSGSBASE		(void) 	{ return CPU_Rep.f_7_EBX_[0]; }
    static 	bool 			BMI1			(void) 	{ return CPU_Rep.f_7_EBX_[3]; }
    static 	bool 			HLE				(void) 	{ return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
    static 	bool 			AVX2			(void) 	{ return CPU_Rep.f_7_EBX_[5]; }
    static 	bool 			BMI2			(void) 	{ return CPU_Rep.f_7_EBX_[8]; }
    static 	bool 			ERMS			(void) 	{ return CPU_Rep.f_7_EBX_[9]; }
    static 	bool 			INVPCID			(void) 	{ return CPU_Rep.f_7_EBX_[10]; }
    static 	bool 			RTM				(void) 	{ return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
    static 	bool 			AVX512F			(void) 	{ return CPU_Rep.f_7_EBX_[16]; }
    static 	bool 			RDSEED			(void) 	{ return CPU_Rep.f_7_EBX_[18]; }
    static 	bool 			ADX				(void) 	{ return CPU_Rep.f_7_EBX_[19]; }
    static 	bool 			AVX512PF		(void) 	{ return CPU_Rep.f_7_EBX_[26]; }
    static 	bool 			AVX512ER		(void) 	{ return CPU_Rep.f_7_EBX_[27]; }
    static 	bool 			AVX512CD		(void) 	{ return CPU_Rep.f_7_EBX_[28]; }
    static 	bool 			SHA				(void) 	{ return CPU_Rep.f_7_EBX_[29]; }

    static 	bool 			PREFETCHWT1		(void) 	{ return CPU_Rep.f_7_ECX_[0]; }

    static 	bool 			LAHF			(void) 	{ return CPU_Rep.f_81_ECX_[0]; }
    static 	bool 			LZCNT			(void) 	{ return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
    static 	bool 			ABM				(void) 	{ return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
    static 	bool 			SSE4a			(void) 	{ return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
    static 	bool 			XOP				(void) 	{ return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
    static 	bool 			TBM				(void) 	{ return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }

    static 	bool 			SYSCALL			(void) 	{ return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
    static 	bool 			MMXEXT			(void) 	{ return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
    static 	bool 			RDTSCP			(void) 	{ return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
    static 	bool 			_3DNOWEXT		(void) 	{ return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
    static 	bool 			_3DNOW			(void) 	{ return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

private:
    static 	const 	InstructionSet_Internal 	CPU_Rep;

    class InstructionSet_Internal
    {
    public:
        InstructionSet_Internal() : 	nIds_		{ 0 }
									,   nExIds_		{ 0 }
									,   isIntel_	{ false }
									,   isAMD_		{ false }
									,   f_1_ECX_	{ 0 }
									,   f_1_EDX_	{ 0 }
									,	f_7_EBX_	{ 0 }
									,   f_7_ECX_	{ 0 }
									,	f_81_ECX_	{ 0 }
									,	f_81_EDX_	{ 0 }
									,   data_		{}
									,   extdata_	{}
        {
            //int cpuInfo[4] = {-1};
            std::array<int, 4> 			cpui;

            // Calling __cpuid with 0x0 as the function_id argument
            // gets the number of the highest valid function ID.
            __cpuid(cpui.data(), 0);
			
            nIds_ 	= 	cpui[0];

            for (int i = 0; i <= nIds_; ++i)
            {
				
                __cpuidex(cpui.data(), i, 0);
                data_ .push_back(cpui);
            }

            // Capture vendor string
            char 	vendor[0x20];
            memset(vendor, 0, sizeof(vendor));
			
            *reinterpret_cast<int*>(vendor) 		= data_[0][1];
            *reinterpret_cast<int*>(vendor + 4) 	= data_[0][3];
            *reinterpret_cast<int*>(vendor + 8) 	= data_[0][2];
			
            vendor_ = vendor;
            
			if (vendor_ == "GenuineIntel")
            {
                isIntel_ = true;
            }
            else if (vendor_ == "AuthenticAMD")
            {
                isAMD_ = true;
            }

            // load bitset with flags for function 0x00000001
            if (nIds_ >= 1)
            {
                f_1_ECX_ = data_[1][2];
                f_1_EDX_ = data_[1][3];
            }

            // load bitset with flags for function 0x00000007
            if (nIds_ >= 7)
            {
                f_7_EBX_ = data_[7][1];
                f_7_ECX_ = data_[7][2];
            }

            // Calling __cpuid with 0x80000000 as the function_id argument
            // gets the number of the highest valid extended ID.
            __cpuid(cpui.data(), 0x80000000);
			
            nExIds_ = cpui[0];

            char brand[0x40];
			
            memset(brand, 0, sizeof(brand));

            for (int i = 0x80000000; i <= nExIds_; ++i)
            {
                __cpuidex(cpui.data(), i, 0);
                extdata_ .push_back(cpui);
            }

            // load bitset with flags for function 0x80000001
            if (nExIds_ >= 0x80000001)
            {
                f_81_ECX_ = extdata_[1][2];
                f_81_EDX_ = extdata_[1][3];
            }

            // Interpret CPU brand string if reported
            if (nExIds_ >= 0x80000004)
            {
                memcpy(brand, 		extdata_[2].data(), sizeof(cpui));
                memcpy(brand + 16, 	extdata_[3].data(), sizeof(cpui));
                memcpy(brand + 32, 	extdata_[4].data(), sizeof(cpui));
				
                brand_ = brand;
            }
        };

        int 								nIds_;
        int 								nExIds_;
        std::string 						vendor_;
        std::string 						brand_;
        bool 								isIntel_;
        bool 								isAMD_;
        std::bitset<32> 					f_1_ECX_;
        std::bitset<32> 					f_1_EDX_;
        std::bitset<32> 					f_7_EBX_;
        std::bitset<32> 					f_7_ECX_;
        std::bitset<32> 					f_81_ECX_;
        std::bitset<32> 					f_81_EDX_;
        std::vector<std::array<int, 4>> 	data_;
        std::vector<std::array<int, 4>> 	extdata_;
    };
};

// Initialize static member data
//const InstructionSet::InstructionSet_Internal 	InstructionSet::CPU_Rep;
