// ClaassHierarchy_FishTunaCarp.hpp
#pragma once

#include <iostream>
#include <memory>
#include <windows.h>
#include <math.h>       /* sqrt */

#include "type_name_as_str.h"

#include "stopwatch.h"
#include "TimerBase.h"

using namespace std;

void TestForSlice();

class AnyFish { 
public:
	AnyFish() : allFisesHave(0)					{ cout << "I'm CTOR of AnyFish" << endl; }
	virtual ~AnyFish()							{ cout << "I'm DTOR of AnyFish" << endl; }

	// Конструктор копий
	AnyFish(const AnyFish& rhs) : allFisesHave(rhs.allFisesHave)
	{ cout << "I'm COPY CTOR of AnyFish" << endl; }
	// Оператор копирующего присваивания
	AnyFish& operator= (const AnyFish& rhs)
	{  
		cout << "I'm COPY ASSINGMENT of AnyFish" << endl; 
		allFisesHave = rhs.allFisesHave; 
		return *this;	
	}
	virtual AnyFish* clone() 
	{ 
		cout << "I'm AnyFish* clone() override of AnyFish" << endl;
		return new AnyFish(*this); 
	}

	virtual void Swim()							{ cout << "I'm AnyFish and I swim" << endl;	};

private:
	int allFisesHave;
};

class TunaFish : public AnyFish {
public:
	TunaFish() : tunaFishHas(1)					{ cout << "I'm CTOR of TunaFish" << endl; }
	virtual ~TunaFish()							{ cout << "I'm DTOR of TunaFish" << endl; }

	// Конструктор копий  производного класса
	TunaFish(const TunaFish& rhs) : AnyFish(rhs),	tunaFishHas(rhs.tunaFishHas)
	{ 
		cout << "I'm COPY CTOR of TunaFish" << endl; 
	}
	// Оператор копирующего присваивания
	TunaFish& operator= (const TunaFish& rhs)	
	{ 
		cout << "I'm COPY ASSINGMENT of TunaFish" << endl; 
		AnyFish::operator= (rhs);	// присваивает базовую часть присвоение членов в производном классе, как обычно, 
									// отработка самоприсвоения и освобождения ресурсов 
		tunaFishHas = rhs.tunaFishHas;
		return *this; 
	}
	virtual TunaFish*	clone() override	
	{
		cout << "I'm TunaFish* clone() override of TunaFish" << endl;
		return new TunaFish(*this);
	}

	virtual void		Swim() override		{ cout << "I'm void Swim() override of TunaFish" << endl; }
private:
	int tunaFishHas;
};

class CarpFish : public AnyFish {
public:
	CarpFish() : carpFishHas(1)				{ cout << "I'm CTOR of CarpFish" << endl; }
	virtual ~CarpFish()						{ cout << "I'm DTOR of CarpFish" << endl; }

	// Конструктор копий производного класса
	CarpFish(const CarpFish& rhs) : AnyFish(rhs), carpFishHas(rhs.carpFishHas)
	{ 
		cout << "I'm COPY CTOR of CarpFish" << endl; 
	}
	// Оператор копирующего присваивания
	CarpFish& operator= (const CarpFish& rhs)	
	{ 
		cout << "I'm COPY ASSINGMENT of CarpFish" << endl; 
		AnyFish::operator= (rhs);	// присваивает базовую часть присвоение членов в производном классе, как обычно, 
									// отработка самоприсвоения и освобождения ресурсов
		carpFishHas = rhs.carpFishHas;
		return *this; 
	}
	virtual CarpFish*	clone() override		
	{ 
		cout << "I'm CarpFish* clone() override of CarpFish" << endl; 
		return new CarpFish(*this); 
	}
	virtual void		Swim()  override		{ cout << "I'm void Swim() override of CarpFish" << endl; }
private:
	int carpFishHas;
};
// -------------------------------------------------------------------------------------------------------------------
// ТЕСТЫ
// -------------------------------------------------------------------------------------------------------------------
// 

inline void MakeFishSwim(AnyFish aFish)		// Обратите внимание на тип параметра - суперкласс
{
	cout << "\nI'm void MakeFishSwim(AnyFish aFish)" << endl;
	aFish.Swim();						// Виртуальная функция
}

inline void MakeFishSwim(AnyFish* r_ptrFishObj)		// Обратите внимание на тип параметра - суперкласс
{
	cout << "\nI'm void MakeFishSwim(AnyFish* r_ptrFishObj)" << endl;
	r_ptrFishObj->Swim();						// Виртуальная функция
}

void MakeFishSwim(const unique_ptr<AnyFish>& u_ptrAnyFishObjRef)
{
	cout << "\nI'm void MakeFishSwim(const unique_ptr<AnyFish>& u_ptrAnyFishObjRef)" << endl;
	u_ptrAnyFishObjRef->Swim();
}

void TestForSlice()
{
	cout << "\nI'm void TestForSlice()" << endl;

	AnyFish* r_ptrAnyFishhObj = new AnyFish{};

	cout << "\nI'm Trasser 0\n" << endl;

	MakeFishSwim(*r_ptrAnyFishhObj);
	MakeFishSwim(r_ptrAnyFishhObj);
	delete r_ptrAnyFishhObj;	r_ptrAnyFishhObj = nullptr;

	cout << "\nI'm Trasser 1\n" << endl;

	CarpFish* r_ptrCarpFishObj = new CarpFish{};
	TunaFish* r_ptrTunaFishObj = new TunaFish{};

	// Конструкторы копирования производных классов не отрабатывают
	cout << "\nI'm Trasser 2\n" << endl;
	MakeFishSwim(*r_ptrCarpFishObj);		// Отработает только конструктор копирования суперкласса - СРЕЗКА
	cout << "\nI'm Trasser 2+\n" << endl;
	MakeFishSwim(*r_ptrTunaFishObj);		// Отработает только конструктор копирования суперкласса - СРЕЗКА
	
	cout << "\nI'm Trasser 3\n" << endl;

	// Конструкторы копирования производных классов НЕ НУЖНЫ, они и не отрабатывают
	MakeFishSwim(r_ptrCarpFishObj);			// Передается же указатель на объект производного класса, какое копирование?
	MakeFishSwim(r_ptrTunaFishObj);			// Передается же указатель на объект производного класса, какое копирование?
	
	cout << "\nI'm Trasser 4\n" << endl;

	// Где будет создан объект? В стеке или в куче?
	CarpFish copyCarpObj(*r_ptrCarpFishObj);

	cout << "\nI'm Trasser 5\n" << endl;

	TunaFish* r_ptrTunaFishCloneObj = r_ptrTunaFishObj->clone();

	cout << "\nI'm Trasser 6\n" << endl;

	delete r_ptrTunaFishCloneObj;	r_ptrTunaFishCloneObj	= nullptr;
	delete r_ptrCarpFishObj;		r_ptrCarpFishObj		= nullptr;
	delete r_ptrTunaFishObj;		r_ptrTunaFishObj		= nullptr;
}
