// VMT_virtual_method_table_VTABLE.cpp

#include <format>
using std::format;
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert> 									// для assert()
#include <memory>
#include "VMT_virtual_method_table_VTABLE.hpp"
/*
	**********************************************************************************
	Dyzzet| C++ Data Science Алгоритмы Темы · Блог · YouTube
	Полиморфная кухня
	2. Таблицы виртуальных функций
	https://dyzzet.ru/a/vmt/
	----------------------------------------------------------------------------------
	Подробнее рассмотрим, как реализован механизм полиморфизма.

	Если в классе есть хотя бы одна функция с ключевым словом virtual, 
	то она считается виртуальной, 
	а сам класс — полиморфным. 
	
	Для каждого полиморфного класса создаётся специальная таблица — 
	'таблица виртуальных функций' (virtual method table, VMT — координирующая таблица или vtable). 
	
	Каждый объект любого из полиморфных классов 
	дополняется указателем на одну из таких таблиц.

	Рассмотрим простую иерархию: 
	'розетка' и 'заземлённая розетка'.

                                          Socket                                
                                            ↑
                                      GroundedSocket
	------------->	.hpp
*/									  
/*
	До начала выполнения функции main() 
	в программе есть две таблицы виртуальных функций (virtual method table, VMT) — 
	по одной на каждый полиморфный класс.

	Обратите внимание: функция GetVoltage() не переопределена в классе GroundedSocket, 
	поэтому в обеих таблицах напротив неё стоит один и тот же адрес. 
	
	Другие функции были замещены, поэтому и адреса их не совпадают. 
	
	Невиртуальные функции в таких таблицах отсутствуют.

	Cоздадим два объекта — объект класса Socket и объект класса GroundedSocket.
*/
void 	debug_dump(std::byte*	memory_socket_ptr){
	cout << "-------------- I'm void " <<  __func__  << "(std::byte*	memory_socket_ptr)" << endl;
	
	cout	<< "Адрес объекта типа Socket в динамической памяти:	" << memory_socket_ptr << endl;
	const unsigned char* 	virtual_func_name	=	reinterpret_cast<const unsigned char*>(memory_socket_ptr);
	for (auto index{0}; index < 32;  index++)
		cout << *(virtual_func_name++) << "  ";
	cout << endl;
	
	virtual_func_name	=	reinterpret_cast<const unsigned char*>(memory_socket_ptr);
	for (auto index{0}; index < 32;  index++)
		cout << std::hex << static_cast<int>(*(virtual_func_name++)) << "   ";
	cout << endl;
	
	// 	Попытки получить адрес таблицы виртуальных методов
	union	{
		void*		socket_VMT_ptr;
		std::byte	first8bytes[8];
	} 	trick;
	memcpy(trick.first8bytes, memory_socket_ptr, 8);	// Я пока думаю, что первые 8 байт - адрес VMT
	cout	<< "Адрес таблицы виртуальных методов:	" << trick .socket_VMT_ptr << endl;


	virtual_func_name	=	reinterpret_cast<const unsigned char*>(trick .socket_VMT_ptr);
	for (auto index{0}; index < 64;  index++)
		//cout << *(virtual_func_name++) << "  ";
		cout << std::hex << static_cast<int>(virtual_func_name[index]) << "   ";
	cout << endl;
}
	
void 	test_1()
{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
    using namespace Electricity;
	
    typedef std::unique_ptr<Socket> 	socket_uptr_t;
   
    socket_uptr_t 	groundedSocket_uptr	{new GroundedSocket};
	
	socket_uptr_t 	socket_uptr			{new Socket};
	cout	<< "Значение указателя на объект типа Socket в динамической памяти:	" << socket_uptr.get() << endl;
	debug_dump(reinterpret_cast<std::byte*>(socket_uptr.get()));
	
	socket_uptr_t 	socket_uptr2		{new Socket};
	cout	<< "Значение указателя на объект типа Socket в динамической памяти:	" << socket_uptr.get() << endl;
	debug_dump(reinterpret_cast<std::byte*>(socket_uptr2.get()));
}
void 	test_2()
{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
	using namespace Electricity;
	
    typedef std::unique_ptr<Socket> 	socket_uptr_t;
    socket_uptr_t 	socket_uptr			{new Socket};
	cout << "Указатель на объект класса снаружи: " << socket_uptr << " " << socket_uptr.get() << endl;
	cout << "Указатель на объект класса изнутри: " << socket_uptr->get_vptr()  << endl;
	
	const unsigned char* 	virtual_func_name	=	reinterpret_cast<const unsigned char*>(socket_uptr->get_vptr());
	for (auto index{0}; index < 32;  index++)
		cout << std::hex << static_cast<int>(virtual_func_name[index]) << "   ";
	cout << endl;
	//debug_dump(reinterpret_cast<std::byte*>(socket_uptr.get()) + 4);
	
	//	-----------------------------------------------------------------------------------------
	std::byte**	memory_socket_ptr{reinterpret_cast<std::byte**>(socket_uptr.get())};
	cout << "Простой способ косвенной адресации: " << *memory_socket_ptr << endl;
	
	//const unsigned char* 	
	virtual_func_name	=	reinterpret_cast<const unsigned char*>(*memory_socket_ptr);
	for (auto index{0}; index < 32;  index++)
		cout << std::hex << static_cast<int>(virtual_func_name[index]) << "   ";
	cout << endl;
	//	-----------------------------------------------------------------------------------------
	union	{
		std::byte*	socket_VMT_ptr;
		std::byte	first8bytes[8];
	} 	trick;
	
	std::byte*		OBJECT_ptr		=	reinterpret_cast<std::byte*>(socket_uptr.get());
	//VMT_ptr						+=	8;
	
	memcpy	(trick .first8bytes, OBJECT_ptr, 8);	
	
	// Я пока думаю, что первые 4 бойта - тип класса, последующие 8 байт - адрес VMT	
	cout  << "Заумный способ косвенной адресации: " << (trick .socket_VMT_ptr) << endl;
	
	virtual_func_name	=	reinterpret_cast<const unsigned char*>(trick .socket_VMT_ptr);
	for (auto index{0}; index < 32;  index++)
		cout << std::hex << static_cast<int>(virtual_func_name[index]) << "   ";
	cout << endl;
}
/*	
	Посмотрим внимательно на их внутреннюю структуру и процесс их инициализации. 
	
	Каждый объект полиморфного класса 
	(строго говоря, это лишь один из популярных вариантов реализации) 
	дополняется указателем на таблицу виртуальных функций. 
	
	Класс Socket является базовым, 
	поэтому в указатель на таблицу виртуальных функций (варианты названий: vptr, __vfptr) 
	просто запишем соответствующий адрес.

	Создадим объект производного класса (GroundedSocket). 
	
	Он тоже дополнен указателем на таблицу виртуальных функций. 
	
	Помним, что сперва вызывается конструктор базовой части, 
	он инициализирует этот указатель «неправильно», 
	ведь базовому классу ничего неизвестно о таблице виртуальных функций производного класса.

	Когда инициализируется та часть, которая относится к самому производному классу, 
	она установит этот указатель в «правильное» значение: 
	теперь он ведёт к нужной таблице.

	Вызовем один и тот же метод для этих объектов.

    ...
 
    std::cout << socket->IsGrounded() << std::endl;
    std::cout << groundedSocket->IsGrounded() << std::endl;

	В первом случае адрес, хранящийся в vptr, равен 0x010dcd10, 
	во втором — 0x010dcd24. 
	
	Вот в этом и вся суть механизма полиморфизма: 
	каждый объект знает свой тип (свою таблицу виртуальных функций); 
	пусть даже объект производного класса скрывается за указателем на базовый класс, 
	в каждом объекте хранится вся нужная информация. 
	
	А далее достаточно вызвать функцию из нужной таблицы с соответствующим смещением. 
	
	----------------------------------------------------------------------------------------------------
	Вызов из конструктора (деструктора)
	
	Если вернуться к схемам, изображённым выше, можно заметить, 
	что состояние объекта формируется не сразу, а пошагово. 
	
	По этой причине не следует вызывать виртуальные функции из конструкторов 
	(объект может быть не до конца сформирован) 
	и деструкторов 
	(объект может начать разрушаться).

	Представьте, что базовый класс является абстрактным 
	(содержит хотя бы одну чисто виртуальную функцию),
	и мы пытаемся вызвать виртуальную функцию для объекта производного класса из конструктора, 
	но состояние указателя на таблицу виртуальных функций пока ещё неверное. 
	
	Произойдёт попытка вызова функции для абстрактного класса (pure function call).

	-----------------------------------------------------------------------------------------------
*/
/*
	********************************** 	main()	*************************************
*/
int main() 
{
	cout << "-------------- I'm int " <<  __func__  << "() of VMT_virtual_method_table_VTABLE.cpp" << endl;
	test_1();	cout << endl;
	test_2();	cout << endl;
	//test_3();	cout << endl;
}
