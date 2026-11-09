// public_protected_private_inheritance.cpp

#include <format>
using std::format;
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <fstream>
#include <iostream>
#include <vector>
#include <cassert> 									// для assert()
#include <memory>
#include "public_protected_private_inheritance.hpp"
/*
	****************************************************************************************************
	
	----------------------------------------------------------------------------------------------------
	
*/	
void	test_1()	{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
	//	---Base-----------------------------------------------------------------------------
	Base				base_Obj;
	cout	<<	base_Obj .public_base_get_m_sum		() 	<< endl;
	/*
		Нет прямого доступа к функциям [private] и [protected] из клиентского кода
		cout	<<	base_Obj .protected_base_get_m_sum		() 	<< endl;
		cout	<<	base_Obj .private_base_get_m_sum		() 	<< endl;
	*/
	//	---PublicDerived--------------------------------------------------------------------
	PublicDerived		public_derived_Obj;
	cout	<<	public_derived_Obj .public_public_derived_get_m_sum		() 	<< endl;
	/*
		Нет прямого доступа к функциям [private] и [protected] из клиентского кода
		public_derived_Obj .protected_public_derived_get_m_sum	();
		public_derived_Obj .private_public_derived_get_m_sum		();
	*/
	//	---ProtectedDerived-----------------------------------------------------------------
	ProtectedDerived	protected_derived_Obj;
	cout	<<	protected_derived_Obj .public_protected_derived_get_m_sum		() 	<< endl;
	/*
		Нет прямого доступа к функциям [private] и [protected] из клиентского кода
	*/	
	//	---PrivateDerived-------------------------------------------------------------------
	PrivateDerived		private_derived_Obj;
	cout	<<	private_derived_Obj .public_private_derived_get_m_sum		() 	<< endl;
	/*
		Нет прямого доступа к функциям [private] и [protected] из клиентского кода
	*/
	//	------------------------------------------------------------------------------------
};
/*
*/
void	test_2()	{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
	//	---Base-----------------------------------------------------------------------------
	Base				base_Obj;
	cout	<<	base_Obj .public_base_get_m_sum		() 	<< endl;
	
	//	---PublicDerived--------------------------------------------------------------------
	PublicDerived		public_derived_Obj;
	cout	<<	public_derived_Obj .public_base_get_m_sum		() 	<< endl;
	
	//	---ProtectedDerived-----------------------------------------------------------------
	ProtectedDerived	protected_derived_Obj;
	/*
		cout	<<	protected_derived_Obj .public_base_get_m_sum	() 	<< endl;
		Нет прямого доступа к унаследованным функциям базового класса из клиентского кода
	*/	
	//	---PrivateDerived-------------------------------------------------------------------
	PrivateDerived		private_derived_Obj;
	/*
		cout	<<	private_derived_Obj .public_base_get_m_sum		() 	<< endl;
		Нет прямого доступа к унаследованным функциям базового класса из клиентского кода
	*/	
	//	--------------------------------------------------------------------------------
};
/*
*/
void	test_3()	{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
	//	---Base-----------------------------------------------------------------------------
	Base				base_Obj;
	cout	<<	friend_for_base					(base_Obj) 				<< endl;
	
	//	---PublicDerived--------------------------------------------------------------------
	PublicDerived		public_derived_Obj;
	cout	<<	friend_for_public_derived		(public_derived_Obj)	<< endl;
	
	//	---ProtectedDerived-----------------------------------------------------------------
	ProtectedDerived	protected_derived_Obj;
	cout	<<	friend_for_protected_derived	(protected_derived_Obj)	<< endl;
	
	//	---PrivateDerived-------------------------------------------------------------------
	PrivateDerived		private_derived_Obj;
	cout	<<	friend_for_private_derived		(private_derived_Obj)	<< endl;
	
	//	--------------------------------------------------------------------------------
};
/*
	При защищенном [protected] наследовании 
	открытые [public] и защищенные [protected] элементы 'базового' класса 
	становятся защищенными [protected] элементами 'производного' класса. 

	При скрытом [private] наследовании 
	открытые [public] и защищенные [protected] элементы 'базового класса' 
	становятся скрытыми [private] элементами 'производного класса'. 
	
	Для 'производных классов', 
	созданных путем скрытого [private] и защищенного [protected] наследования, 
	не действует отношение 'является' применительно к их 'базовым классам', 
	поскольку открытые [public] элементы 'базовых классов'
	недоступны для клиентского кода 'производных классов'. 
*/
void	test_4()	{
	cout << "-------------- I'm void " <<  __func__  << "()" << endl;
	//	---Base-----------------------------------------------------------------------------
	Base					base_Obj;
	
	friend_for_base_class	friend_for_base_classObj;
	cout 	<< friend_for_base_classObj .public_base_get_m_sum		(base_Obj) << endl;
	
// cout 	<< friend_for_base_classObj .protected_base_get_m_sum	(base_Obj) << endl;
// cout 	<< friend_for_base_classObj .private_base_get_m_sum		(base_Obj) << endl;
	
	
	//	---PublicDerived--------------------------------------------------------------------
	PublicDerived						public_derived_Obj;
	
	friend_for_public_derived_class		friend_for_public_derived_classObj;
	cout 	<< friend_for_public_derived_classObj .public_public_derived_get_m_sum		(public_derived_Obj) << endl;
	
//cout 	<< friend_for_public_derived_classObj .protected_public_derived_get_m_sum	(public_derived_Obj) << endl;
//cout 	<< friend_for_public_derived_classObj .private_public_derived_get_m_sum	(public_derived_Obj) << endl;
	
	//	---ProtectedDerived-----------------------------------------------------------------
	ProtectedDerived	protected_derived_Obj;
	
		
	//	---PrivateDerived-------------------------------------------------------------------
	PrivateDerived		private_derived_Obj;
	
	
	//	--------------------------------------------------------------------------------
};

/*
	********************************** 	main()	********************************************************
*/
int main() 
{
	cout << "-------------- I'm int " <<  __func__  << "() of public_protected_private_inheritance.cpp" << endl;
	test_1();	cout << endl;
	test_2();	cout << endl;
	test_3();	cout << endl;
	test_4();	cout << endl;
}
