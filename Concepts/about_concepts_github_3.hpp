#pragma once
/*
	about_concepts_github_3.hpp
*/
#include	<iostream>
using std::cout; using std::endl; using std::boolalpha;
#include <limits>
using std::numeric_limits;
#include <string>
using std::string;
#include <cassert>
#include <cstring>
#include <concepts>
#include <iterator>
#include <optional>
#include <vector>
#include <list>
#include <string>
#include <type_traits>
#include <memory>
#include "..\type_name_as_str.h"
/*	
	***********************************************************************************************
	C++14/17 type_traits Часть 3.
	https://gist.github.com/Nekrolm/418043d6a1d2360020525edbc7c36dc8#file-cpp14_traits_3-md
	***********************************************************************************************

	В предыдущей серии мы изобретали нечто, называемое XXX_traits, 
	где под XXX скрывается какой-то концепт. 
	
	А сам трейт описывает, 
	какие операции доступны для типов, 
	реализующих данный концепт. 
	
	И все взаимодействие с шаблонными параметрами происходило с помощью вспомогательной структуры.

?????????????????????

template <class T, class S>
auto 	accumulate(const std::vector<T>& arr, S&& op) -> RESULT<T>::REQUIRES<SummatorTraits<S>::value> 
{
    using 	STraits 	= 	SummatorTraits<S>;
    
	auto 	sum = STraits::Zero(op);
	
    for (auto x : arr) {
        sum = STraits::Sum(op, sum, x);
    }
    return sum;
};

	При таком подходе замечательно все, 
	кроме громоздкости при обращении к интересующим нас методам. 
	
	Но C++ не был бы самим собой, 
	если бы в нем нельзя было бы решить задачу множеством самых жутких способов!

	Давайте вернемся к варианту с шаблонным интерфейсом:

template <class T>
class ISummator {
public:
	virtual ~ISummator() 			= 0;
	virtual T Sum(T a, T b) const 	= 0;
	virtual T Zero() 		const 	= 0;
};

	Это отличный способ, но у него, как мы уже обсуждали, множество недостатков. 
	
	Начиная от виртуальности, 
	заканчивая невозможностью никаких изменений в сигнатурах методов. 
	
	Но все эти недостатки мы можем исправить. 
	
	Добавив дополнительный уровень индирекции (косвенности)
	и вспомнив одно замечательное свойство шаблонов: 
	тип-параметр шаблона может быть неполным (incomplete) типом. 
	
	И он может оставаться таковым до тех пор, 
	пока к нему не обратятся как к полному типу 
	(применят sizeof, 
	попробуют объявить переменную, 
	заглянуть во вложенные типы 
	и т.д.). 
	
	На этом свойстве работает Curious Recurring Template Pattern (CRTP).

	Попробуем рассмотреть его на примере нашего сумматора.

	Начнется все с базового класса.
*/
template <class Impl> 	// Impl --- будет конечным типом, имплементирующим интерфейс
class ISummator {
public:
	/*
		Потребуем, чтоб Impl предоставлял 
		как минимум константный (!а может и статический!) метод Sum(), 
		принимающий какие-то два аргумента.
		
		Используем perfect forwarding для возвращаемого значения (dectype(auto))
		и аргументов (T&&, std::forward<T>), 
		чтобы все передавалось и возвращалось как есть:
		копии -- копиями, ссылки -- ссылками 
		(сохранить rvalue/lvalue и прочие *value категории)
	*/
    template <class T1, class T2> 
    decltype(auto) 	Sum(T1&& a, T2&& b) const {
		cout << "-ISum- I'm template <class T1, class T2> decltype(auto) " << __func__ << "(T1&& a, T2&& b) const" 
			<<endl;
		return AsImpl()->Sum(std::forward<T1>(a), std::forward<T2>(b));
    }
    /* 
		Аналогично потребуем, 
		чтоб Impl предоставил как минимум константный метод Zero, 
		не принимащий ничего
	*/	
    decltype(auto) Zero() const {
		cout << "-ISum- I'm decltype(auto) " << __func__ << "()" << endl;
        return AsImpl()->Zero();
    }
private:
    // не разрешаем никому, кроме имплементирующего типа, конструировать базовый класс
    friend 		Impl; 
    ISummator() = default;
    /*
		Мы не используем виртуальный деструктор,
		так что разрушать объект тоже запрещаем,
		чтоб никто не делал delete через указатель на этот класс
	*/
    ~ISummator() = default;
    
    const Impl* 	AsImpl() const {
		cout << "-ISum- I'm const Impl* " << __func__ << "() const" << endl;
        /*
			Будем требовать, чтоб Impl был нашим наследником,
			иначе static_cast просто не скомпилируется,
			причем мы можем здесь обращаться с Impl как с полным типом. 
			
			Потому что методы шаблонов компилируются только тогда, 
			когда к ним будут обращаться вне шаблонов
		*/	
        return 	static_cast<const Impl*>(this);
    }
};
/*
	А теперь имплементируем наш интерфейс

	Наследуемся от базового класса, 
	указывая параметром шаблона самих себя!

	Внутри шаблона в этот момент DefaultSummator является неполным типом 
	(конечно, мы его только начали описывать!)
	
	Однако никаких ошибок не возникает -- мы не трогаем методы ISummator, 
	они не компилируеются,
	и мы не обращаемся к нашему пока еще не полному типу!
*/
class DefaultSummator : public ISummator<DefaultSummator> {
public:
    static float Zero() {
		cout << "--DefaultSummator-- I'm static float " << __func__ << "()" << endl;
        return 0;
    }
    static float Sum(float a, float b) {
		cout << "--DefaultSummator-- I'm static float " << __func__ << "(float a, float b)" << endl;
        return a + b;
    }
};
/*
	Заметим, что мы не используем виртуальные методы и виртуальный деструктор.

	Используем теперь его в функции accumulate():
*/
template<class T, class Summator>
auto 	accumulate	(	const std::vector<T>& 		arr
					, 	const ISummator<Summator>& 	op
					) 
{
	cout << "--- I'm template<class T, class Summator> auto " << __func__ 
	<< "(std::vector<T>&, ISummator<Summator>&)" << endl;
	
	auto sum = op.Zero();

	for (auto x : arr) 
		sum = op.Sum(sum, x);
	
	return sum;
}
void test_1()	{
	cout << "-------------- I'm void " << __func__ << endl;
	std::vector<int>	int_vector{11, 21, 42, 84};
	cout << accumulate(int_vector, DefaultSummator()) << endl;
	/*
		И попробуем скормить ей что-то неправильное. 
		
		Получили не очень длинную, но все же неприятную ошибку компиляции. 
		
		Неприятную от того, что она изнутри функции accumulate. 
		
		Конечно, у нас потеряна зависимость сумматора от типа 'T', 
		поэтому никак пораньше мы уловить ошибку не смогли. 
	*/	
	std::vector<std::string> 	v = {"a", "b", "c"};
    /*
	auto r = accumulate	(	v
						, 	DefaultSummator() 	// вот тут нужно не перепутать скобки и версии стандарта! 
												// {} дадут ошибку компиляции из-за переосмысления агрегатов
                        );
	*/
};
/*
	Но мы можем это исправить -- подкорректировав интерфейс. 
	
	Потребуем, чтоб сумматор предоставлял тип, 
	с которым он работает, 
	и налaдим ограничение, 
	что он должен совпадать 
	(или быть совместимым) с 'T'.

	Наивный способ запросить у имплементации тип выглядит довольно логично и красиво:

template <class Impl> 
class ISummator {
public:
    using value_type = typename Impl::value_type;
....
};
	
	Увы, он не работает. 
	
	Мы обратились к Impl в поисках вложенных типов -- а значит, 
	Impl должен быть полным типом. 
	
	Таковым он в момент инстанциирования шаблона не является. 
	
	Стандартная ситуация для C++: 
	если очевидное и красивое решение не работает, 
	то сработает неочевидное и уродливое!

	Сделаем метод, 
	внутри которого Impl уже будет полным типом, 
	а значит, из него можно будет доставать все что угодно.

	Заведем себе оберточку, 
	с помощью которой будем доставать тип.
	
	В C++20 для подобных махинаций есть std::type_identity<T>,
	но будь у нас С++20, мы бы всем этим не занимались
*/
template <class T>
struct TypeWrapper {
    using type = T;
};

template <class Impl> 		// Impl --- будет конечным типом, имплементирующим интерфейс
class ISummator_n {
public:
	/*
		Потребуем, чтоб Impl предоставлял 
		как минимум константный (!а может и статический!) метод Sum(), 
		принимающий какие-то два аргумента.
		
		Используем perfect forwarding для возвращаемого значения (dectype(auto))
		и аргументов (T&&, std::forward<T>), 
		чтобы все передавалось и возвращалось как есть:
		копии -- копиями, ссылки -- ссылками 
		(сохранить rvalue/lvalue и прочие *value категории)
	*/
    template <class T1, class T2> 
    decltype(auto) 	Sum(T1&& a, T2&& b) const {
		cout << "-ISum_n- I'm template <class T1, class T2> decltype(auto) " << __func__ << "(T1&& a, T2&& b) const" 
			<<endl;
		return AsImpl()->Sum(std::forward<T1>(a), std::forward<T2>(b));
    }
    /* 
		Аналогично потребуем, 
		чтоб Impl предоставил как минимум константный метод Zero, 
		не принимащий ничего
	*/	
    decltype(auto) Zero() const {
		cout << "-ISum_n- I'm decltype(auto) " << __func__ << "()" << endl;
        return AsImpl()->Zero();
    }
private:
    // не разрешаем никому, кроме имплементирующего типа, конструировать базовый класс
    friend 		Impl; 
    ISummator_n() = default;
    /*
		Мы не используем виртуальный деструктор,
		так что разрушать объект тоже запрещаем,
		чтоб никто не делал delete через указатель на этот класс
	*/
    ~ISummator_n() = default;
    
    const Impl* 	AsImpl() const {
		cout << "-ISum_n- I'm const Impl* " << __func__ << "() const" << endl;
        /*
			Будем требовать, чтоб Impl был нашим наследником,
			иначе static_cast просто не скомпилируется,
			причем мы можем здесь обращаться с Impl как с полным типом. 
			
			Потому что методы шаблонов компилируются только тогда, 
			когда к ним будут обращаться вне шаблонов
		*/	
        return 	static_cast<const Impl*>(this);
    }
public:
    /*
		В возвращаемом значении мы не можем указать ничего, кроме auto
		иначе мы полезем внутрь Impl пока он еще incomplete
	*/	
    static constexpr auto ValueType() {
		/*
			Impl уже complete. 
			
			Требуем, чтоб он предоставлял тип value_type.
			
			Мы не можем просто вернуть value_type{} -- 
			у него может не быть конструктора по умолчанию
			
			Можно вернуть std::declval<value_type>()
			но тогда этот метод не будет компилироваться вне невычисляемого контекста (decltype, sizeof)
			
			Поэтому нам и нужна обертка
		*/	
        return TypeWrapper<typename Impl::value_type>{};
    }
};
class DefaultSummator_n : public ISummator_n<DefaultSummator_n> 
{
public:
	/*
		Добавим в DefaultSummator тип, с которым он будет работать. 
		
		И наконец-то напишем восхитительную по своей красоте 
		проверку для функции accumulate(), 
		что этот тип совпадает с типом элементов вектора.
	*/
	using value_type	=	float;

    static float Zero() {
		cout << "--DefaultSummator_n-- I'm static float " << __func__ << "()" << endl;
        return 0;
    }
    static float Sum(float a, float b) {
		cout << "--DefaultSummator_n-- I'm static float " << __func__ << "(float a, float b)" << endl;
        return a + b;
    }
};
template <class T, class Summator>
auto 	accumulate_n(	const std::vector<T>& 			arr
					, 	const ISummator_n<Summator>& 	op
					) -> std::enable_if_t	<	std::is_same_v	<	T 	// если T совпадает с типом, 
																		// который мы вытащили из обертки,
																		// предоставленной методом ValueType,
																,	typename decltype(op.ValueType())::type
																>
											, 	T // то accumulate вернет T
											>
{
    cout << "--- I'm template<class T, class Summator> auto " << __func__ 
	<< "........................" << endl;
	
	auto sum = op.Zero();

	for (auto x : arr) 
		sum = op.Sum(sum, x);
	
	return sum;
};
void test_2()	{
	cout << "-------------- I'm void " << __func__ << endl;
	std::vector<float>	flt_vector{11.1, 21.2, 42.4, 84.8};
	cout << accumulate_n(flt_vector, DefaultSummator_n()) << endl;
	/*
		И снова не работает для строк. !!!!!!!
		
	std::vector<std::string> 	v = {"a", "b", "c"};
    
	auto r = accumulate_n	(	v
							, 	DefaultSummator_n() 	
							);
	*/							
};
