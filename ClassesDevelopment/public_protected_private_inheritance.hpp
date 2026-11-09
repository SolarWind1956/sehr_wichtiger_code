//	public_protected_private_inheritance.hpp

//		#pragma once -> SF.8: Use #include guards for all header files
#ifndef public_protected_private_inheritance_hpp_INCLUDE_GUARD
#define public_protected_private_inheritance_hpp_INCLUDE_GUARD

#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <string>
#include <string_view>
#include <cassert>
#include <memory>
#include <vector>
#include "..\..\type_name_as_str.h"
/*
	****************************************************************************************************
	
	----------------------------------------------------------------------------------------------------
*/
//	--------------------------------------------------------------------------------------------
/*
	Доступ через дружеские функции 
	к открытым [public], защищенным [protected] и закрытым [private] членам класса
	а также
	через открытые [public], защищенные [protected] и закрытые [private] функции класса.
*/
class	Base	
{
	friend	int		friend_for_base		(Base&);
	
	friend	struct 	friend_for_base_class;
	
public:
	int		public_base_get_m_sum		()	{ return	m_public + m_protected + m_private; }
protected:
	int		protected_base_get_m_sum	()	{ return	m_public + m_protected + m_private; }
private:
	int		private_base_get_m_sum		()	{ return	m_public + m_protected + m_private; }
public:
	int		m_public					{1};
protected:
	int		m_protected					{10};
private:	
	int		m_private					{100};
};
//	-----------------------------------------------------------------------------
int			friend_for_base				(Base& base)		{ 
	return 		base .m_public + base .m_protected + base .m_private
			+	base .public_base_get_m_sum		()
			+	base .protected_base_get_m_sum	()
			+	base .private_base_get_m_sum	()
			;
}
//	-----------------------------------------------------------------------------
struct	friend_for_base_class{
public:
	int		public_base_get_m_sum		(Base& base)	{ return	base .m_public 
																+ 	base .m_protected 
																+ 	base .m_private; 
														}
protected:
	int		protected_base_get_m_sum	(Base& base)	{ return	base .m_public 
																+ 	base .m_protected 
																+ 	base .m_private; 
														}
private:
	int		private_base_get_m_sum		(Base& base)	{ return	base .m_public 
																+ 	base .m_protected 
																+ 	base .m_private; 
														}														
};
//	--------------------------------------------------------------------------------------------
/*
	Открытое [public] наследование 
	
	К переменным и функциям [private] базового класса нет прямого доступа в производных классах.
	
	Доступ через открытые [public] или защищенные [protected] функции, унаследованные  от базового класса.
	
	Доступ через дружеские функции через открытые [public] или защищенные [protected] функции к 
	открытым [public] или защищенным [protected] членам базового класса.
*/
class	PublicDerived	: public Base 	
{
	friend	int		friend_for_public_derived		(PublicDerived&);
	
	friend	struct	friend_for_public_derived_class;
	
public:
	int		public_public_derived_get_m_sum			()	{ return		m_public + m_protected /* + m_private */
																	+ 	public_base_get_m_sum		()
																	+	protected_base_get_m_sum	()
																	/*+	private_base_get_m_sum		()*/; }
protected:
	int		protected_public_derived_get_m_sum		()	{ return		m_public + m_protected /* + m_private */
																	+ 	public_base_get_m_sum		()
																	+	protected_base_get_m_sum	()
																	/*+	private_base_get_m_sum		()*/; }
private:
	int		private_public_derived_get_m_sum		()	{ return	m_public + m_protected /* + m_private */
																	+ 	public_base_get_m_sum		()
																	+	protected_base_get_m_sum	()
																	/*+	private_base_get_m_sum		()*/; }
};
//	----------------------------------------------------------
int			friend_for_public_derived				(PublicDerived& public_derived)		{ 
	return 		public_derived .m_public + public_derived .m_protected /*+ public_derived .m_private*/
			+	public_derived .public_base_get_m_sum		()
			+	public_derived .protected_base_get_m_sum	()
			/*+	public_derived .private_base_get_m_sum	()*/
			;
}
//	----------------------------------------------------------
struct	friend_for_public_derived_class{
public:
	int		public_public_derived_get_m_sum		(PublicDerived& public_derived)	{ return	public_derived .m_public 
																					+ 	public_derived .m_protected 
																					//+ 	public_derived .m_private
																					; 
																				}
protected:
	int		protected_public_derived_get_m_sum	(PublicDerived& public_derived)	{ return	public_derived .m_public 
																				//+ 	public_derived .m_protected 
																				//+ 	public_derived .m_private
																				; 
																				}
private:
	int		private_public_derived_get_m_sum	(PublicDerived& public_derived)	{ return	public_derived .m_public 
																				//+ 	public_derived .m_protected 
																				//+ 	public_derived .m_private
																				; 
																				}
														
};

//	--------------------------------------------------------------------------------------------
/*
	Защищенное [protected] наследование 
	
	К переменным и функциям [private] базового класса нет прямого доступа в производных классах.
	
	Доступ через открытые [public] или защищенные [protected] функции, унаследованные  от базового класса.
	
	Доступ через дружеские функции через открытые [public] или защищенные [protected] функции к 
	открытым [public] или защищенным [protected] членам базового класса.
	
	При защищенном [protected] наследовании открытые и защищенные элементы базового класса 
	становятся защищенными элементами производного класса. 

*/
class	ProtectedDerived : protected Base 	
{
	friend	int	friend_for_protected_derived			(ProtectedDerived&);
public:
	int		public_protected_derived_get_m_sum		()	{ return		m_public + m_protected /* + m_private */
																	+ 	public_base_get_m_sum		()
																	+	protected_base_get_m_sum	()
																	/*+	private_base_get_m_sum		()*/; }
protected:
	int		protected_protected_derived_get_m_sum	()	{ return		m_public + m_protected /* + m_private */
																	+ 	public_base_get_m_sum		()
																	+	protected_base_get_m_sum	()
																	/*+	private_base_get_m_sum		()*/; }
private:
	int		private_protected_derived_get_m_sum		()	{ return		m_public + m_protected /* + m_private */
																	+ 	public_base_get_m_sum		()
																	+	protected_base_get_m_sum	()
																	/*+	private_base_get_m_sum		()*/; }
};
int			friend_for_protected_derived				(ProtectedDerived& 	protected_derived)		{ 
	return 		protected_derived .m_public + protected_derived .m_protected /*+ protected_derived .m_private*/
			+	protected_derived .public_base_get_m_sum		()
			+	protected_derived .protected_base_get_m_sum	()
			/*+	protected_derived .private_base_get_m_sum	()*/
			;
}

//	--------------------------------------------------------------------------------------------
/*
	Закрытое [private] наследование 
	
	К переменным и функциям [private] базового класса нет прямого доступа в производных классах.
	
	Доступ через открытые [public] или защищенные [protected] функции, унаследованные  от базового класса.
	
	Доступ через дружеские функции через открытые [public] или защищенные [protected] функции к 
	открытым [public] или защищенным [protected] членам базового класса.
	
	При скрытом [private] наследовании открытые и защищенные элементы базового класса 
	становятся скрытыми элементами производного класса. 
*/
class	PrivateDerived	: private Base 	
{
	friend	int	friend_for_private_derived			(PrivateDerived&);
public:
	int		public_private_derived_get_m_sum		()	{ return		m_public + m_protected /* + m_private */
																	+ 	public_base_get_m_sum		()
																	+	protected_base_get_m_sum	()
																	/*+	private_base_get_m_sum		()*/; }
protected:
	int		protected_private_derived_get_m_sum		()	{ return		m_public + m_protected /* + m_private */
																	+ 	public_base_get_m_sum		()
																	+	protected_base_get_m_sum	()
																	/*+	private_base_get_m_sum		()*/; }
private:
	int		private_private_derived_get_m_sum		()	{ return		m_public + m_protected /* + m_private */
																	+ 	public_base_get_m_sum		()
																	+	protected_base_get_m_sum	()
																	/*+	private_base_get_m_sum		()*/; }
};
int			friend_for_private_derived				(PrivateDerived& 	private_derived)		{ 
	return 		private_derived .m_public + private_derived .m_protected /*+ private_derived .m_private*/
			+	private_derived .public_base_get_m_sum		()
			+	private_derived .protected_base_get_m_sum	()
			/*+	private_derived .private_base_get_m_sum	()*/
			;
}

//	---------------------------------------------------------------------------
/*
	При защищенном [protected] наследовании открытые и защищенные элементы базового класса 
	становятся защищенными элементами производного класса. 

	При скрытом [private] наследовании открытые и защищенные элементы базового класса 
	становятся скрытыми элементами производного класса. 
	
	Для производных классов, созданных путем скрытого [private] и защищенного [protected] наследования, 
	не действует отношение 'является' применительно к их базовым классам, 
	поскольку открытые элементы базовых классов недоступны для клиентского кода производных классов. 
*/
/*
	****************************************************************************************************
*/
#endif 	// End of public_protected_private_inheritance_hpp_INCLUDE_GUARD
