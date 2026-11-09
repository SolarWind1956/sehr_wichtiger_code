/* 
	Ch_22_4_Vector_Arithmetic.cpp
*/
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;

#include <cstddef>
#include <iterator>
#include <exception>
#include <vector>

#include <numeric>
#include <limits>
#include <climits>
#include <cmath> 
#include <math.h>
#include <cerrno>
#include <valarray>
using	std::valarray;
using namespace std;

#include "..\type_name_as_str.h"

#include "Ch_22_4_Vector_Arithmetic.hpp"
#include "Ch_22_4_Vector_Arithmetic_cont.hpp"
/*
	**************************************************************************************
	22.4. Векторная арифметика
	
	Часто в программах математические вычисления 
	сводятся к относительно простой обработке одномерных массивов (векторов) чисел с плавающей запятой. 
	
	Такие векторы, в частности, поддерживаются на аппаратном уровне 
	для высокопроизводительных суперкомпьютеров, 
	для них написано множество библиотек общего назначения, 
	и, кроме того, имеются их глубоко оптимизированные версии для специальных применений. 
	
	Как следствие, стандартная библиотека языка C++ 
	также предоставляет специальный векторный класс — std::valarray — 
	специально разработанный для быстрых численных операций над векторами.			---------------> .hpp
*/ 
/*
	------------------------------------------------------------------------------------------------
	Hабор конструкторов позволяет нам инициализировать std::valarray, 
	используя числовые массивы вспомогательных типов или отдельные значения.
*/
void 	f_w_cunstructors_set()	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	valarray<double> v0; 					// 	можно присвоить что-нибудь позже
	valarray<float> 	v1(1'000); 				// 	1000 элементов со значением float() == 0.0F
	valarray<int> 	v2(-1, 2'000);  		// 	2000 элементов со значением -1
	//valarray<double> v3(100, 9.8064); 		// 	ошибка: размер задан числом с плав, запятой 
	valarray<float> 	v4 = v1; 				// 	v4 содержит v1.size() элементов
	
	/*
		В двухаргументных конструкторах "значение" указывается 'перед' "числом элементов". 
		
		Это отличается от порядка следования аргументов для стандартных контейнеров.
		
		Число элементов в качестве аргумента конструкторов определяет итоговый размер valarray.
		
		Большинству программ нужны данные из таблиц или из ввода; 
		это поддерживается конструктором, 
		копирующим элементы массивов встроенных типов. 
		
		Например:
	*/
	const double 		vd[] 	= 	{0.1, 1.1, 2.1, 3.1, 4.1};
	const int 			vi[] 	=	{0, 1, 2, 3, 4};
	
	valarray<double> 	v3 	(vd, 4); 	//	4 элемента: 0,1,2,3
	//valarray<double> 	v14 	(vi, 4); 	// 	type error: vi - это не указатель на double
	valarray<double> 	v5 	(vd, 8); 	// 	!!!! undefined behavior: слишком мало элементов для инициализации	
	/*
		Эта форма инициализации крайне важна, 
		поскольку вычислительные программы часто выдают данные в виде больших массивов.
		
		Тип valarray и вспомогательные средства, связанные с этим типом, 
		разработаны для высокопроизводительных вычислений. 
		
		Это отражается в ряде ограничений для пользователей и в определенной свободе для разработчиков, 
		которым разрешено использовать все мыслимые способы оптимизации. 
		
		Кроме того, операции с valarray можно делать встраиваемыми, 
		они не должны иметь побочных эффектов и не должны иметь синонимов. 
		
		При сохранении базовой семантики 
		допускаются введение вспомогательных типов 
		и устранение временных объектов. 
		
		В итоге, объявления в заголовочном файле <valarray> могут выглядеть не совсем так, 
		как я представляю их здесь (и в стандарте), 
		но они должны предоставлять те же самые операции 
		с тем же самым смыслом и ожидаемым результатом 
		для любого клиентского кода, 
		не нарушающего установленных правил. 
		
		В частности, элементы массива valarray 
		должны обладать обычной семантикой копирования.
	*/
}
double f_any_expr	(double x) 	{ 
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;	
	return 2.0 * x - 1.0; 
}
template		<class T>
void 	print_valarray	(	const char* 			lbl
						, 	const valarray<T>& 	va
						) 
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	cout << lbl << ": ";
	for	(size_t i = 0; i < va.size(); ++i)
		cout << va[i] << ' ';
	cout << endl;
}
//	--------------------------------------------------------------------------------------------------
template <typename T>
void printValarray_line_by_line 	(	const T& 	va			//	Массив одномерный
								, 	int 			num			//	условно разделенный на строки
								)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	//	Цикл по количеству строк
    for (int i = 0; i < va.size() / num; ++i) {
		//	Цикл вывода элементов, условно принадлежащих очередной условной строке
        for (int j = 0; j < num; ++j) {
            cout << va[i * num + j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}
//	--------------------------------------------------------------------------------------------------
void 	f_w_cunstructors_set_2()	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	double 				n[] = {1.0, 2.0, 3.0, 4.0};
	
	valarray<double> 	v	{	n, 	sizeof n / sizeof n[0]	};
	print_valarray("v", v);
	
	valarray<double> 	sh	{	v.shift(1)	};
	print_valarray("shift 1", sh);
	
	valarray<double> 	acc	{	v + sh		};
	print_valarray("sum", acc);
	
	valarray<double> 	trig	{	sin(v) + cos(acc)			};
	print_valarray("trig", trig);
	
	valarray<double> 	p	{	pow(v, 3.0)					};
	print_valarray("3rd power", p);
	
	valarray<double> 	app	{	v.apply(f_any_expr)			};
	print_valarray("f_any_expr(v)", app);
	
	valarray<bool> 		eq	{	v == app		};
	print_valarray("v == app?", eq);
	
	double 				x 	= 	v.min();
	double 				y 	= 	v.max();
	double 				z 	= 	v.sum();
	cout << "x = v.min() == " << x << ", y = v.max() == " << y << ", z = v.sum() == " << z	<< endl;
}
//	--------------------------------------------------------------------------------------------------
static void		test_1()	
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	f_w_cunstructors_set();
	f_w_cunstructors_set_2();
}
/*
	----------------------------------------------------------------------------------------------------
	Массив std::valarray можно присвоить другому массиву этого же типа и размера. 
	
	С очевидностью v1 = v2 копирует каждый элемент v2 
	в соответствующую позицию массива v1. 
	
	В случае разных размеров результат такого копирования не определен, 
	а из-за того, что реализация valarray сильно оптимизирована по скорости работы, 
	не следует ожидать в этом случае какого-либо информативного исключения 
	или иной разумной формы реакции на ошибку.
 
	Дополнительно к рассмотренному общепринятому присваиванию 
	допускается также присваивание скалярной величины. 
	
	Например, v = 7 присваивает каждому элементу v значение 7.
	
	Индексация целыми числами вполне традиционна,
	и при этом никакого контроля выхода за допустимые границы индексов не выполняется.
	
	Кроме извлечения индивидуальных элементов 
	операция индексации для valarray предоставляет 
	четыре способа извлечения подмассивов. 
	
	Операции присваивания (и конструкторы) 
	принимают такие подмассивы в качестве операндов. 
	
	Подходящий набор операций присваивания для типа valarray 
	делает ненужным перед выполнением присваивания 
	преобразование вспомогательных типов, 
	таких как slice_array, в тип valarray. 
	
	Любая реализация может аналогичным образом определить для типа valarray 
	другие эффективные векторные операции, 
	например, операции + и *. 
	
	Наконец, существуют известные приемы оптимизации для векторных операций, 
	включающих срезы (slices) и иные вспомогательные векторные типы.
*/
void		f_w_assign()		{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	std::valarray<int>	int_vlarr(7);
	int_vlarr	=	42;						//	Всем элементам массива будет присвоено это значение 42
	for (size_t idx = 0; idx < 7; ++idx)
		cout << int_vlarr[idx] << " ";
	cout << endl;	
}	
/*
	Oперации, отличные от присваивания, конструируют новый valarray. 
	Например:
*/
double incr (double d) 
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	return d + 1;
}
void f_w_apply (valarray<double>& v)
{ 
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	//	Это не присваивание, это инициализация
	valarray<double> 	v2 	= 	v .apply(incr); 		// получаем инкрементированный valarray
	
	for (size_t idx = 0; idx < v.size(); ++idx)		cout << v[idx] << " ";	cout << endl;	
	
	for (size_t idx = 0; idx < v.size(); ++idx)		cout << v2[idx] << " ";	cout << endl;	
	
	valarray<double> 	v3;
	v3	=	v2;
	for (size_t idx = 0; idx < v.size(); ++idx)		cout << v3[idx] << " ";	cout << endl;	
	
	//	Это присваивание, это не инициализация
	v3	=	v2 .apply(incr);
	for (size_t idx = 0; idx < v.size(); ++idx)		cout << v3[idx] << " ";	cout << endl;	
}
/*
	Здесь значение 'v' не изменяется. 
	
	???? Функция apply() не принимает в качестве аргумента, к сожалению, объекты функциональных классов,
	как это делают алгоритмы стандартной библиотеки.
*/
static void		test_2()	
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	
	f_w_assign();
	
	valarray<double> v{1.2, 3.14, 2.7};
	for (size_t idx = 0; idx < v.size(); ++idx)		cout << v[idx] << " ";	cout << endl;
	f_w_apply (v);
}
/*
	Функции логического и циклического сдвигов, shift() и cshift(), 
	возвращают новый объект типа valarray с соответствующим образом сдвинутыми элементами, 
	а исходный valarray также оставляют неизменным. 
	
	Например, циклический сдвиг 
	v2 = v. cshift(п) 
	породит v2 таким образом, что 
	v2[i] == v[(i + n) % v.size()]. 
	
	Логический сдвиг v3 = v. shift(п) породит v3 таким образом, 
	что v3[i] равно v[i + n], если 'i + n' — допустимый индекс для 'v', 
	а в противном случае элемент принимает значение по умолчанию. 
	
	Считается, что shift() и cshift() осуществляют сдвиг влево, 
	если их аргумент положительный, и сдвиг вправо при отрицательном аргументе. 
*/
static void		test_3()		{	
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	valarray<double> v{1.2, 3.14, 2.7};
	
	for (size_t idx = 0; idx < v.size(); ++idx)		cout << v[idx] << " ";	cout << endl;
	
	int	n{2};
	
	valarray<double> v2 = v. cshift(n);
	for (size_t idx = 0; idx < v2.size(); ++idx)		cout << v2[idx] << " ";	cout << endl;
	
	valarray<double> v3 = v2. shift(n);
	for (size_t idx = 0; idx < v3.size(); ++idx)		cout << v3[idx] << " ";	cout << endl;
}
/*
	-------------------------------------------------------------------------------------------
	Если же мы захотим, чтобы массив valarray с ценными данными рос динамически, 
	нужно обратиться к временному хранилищу:
*/ 
void f_w_grow (valarray<double> & v, size_t n)		{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	if (n <= v. size()) 
		return;
	
	valarray<double>	tmp(n); 						//	n умолчательных элементов
	//	копирующий алгоритм
	std::copy 	(	&v[0]
				, 	&v[v .size()] 
				, 	&tmp[0]
				); 
	v. resize(n);
	std::copy 	(	&tmp[0] 
				, 	&tmp[v.size()] 
				, 	&v[0] 
				);
}
/*
	Но это не основное предназначение массивов valarray. 
	
	Тип valarray рассчитан на то, что после инициализации размер массива этого типа не меняется.
	
	Элементы массива valarray формируют последовательность, 
	так что v[0]...v[n-1] располагаются в памяти друг за другом. 
	
	Это означает, что для valarray<T> Т* является итератором произвольного доступа, 
	и можно использовать стандартные алгоритмы, такие как std::сору(), например. 
	
	Однако в духе valarray 
	лучше применять для копирования 'операцию присваивания' и 'подмассивы':
*/
void		f_w_grow_2 (valarray<double>& v, size_t n)	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	if	(n <= v. size()) 
		return;
	
	valarray<double> 	tmp 		= 	v;
	
	//	срез, (подмассив, подмножество) из v.size()
	std::slice 	slc	{	0					//	Начальный индекс
					, 	v .size()			//	Количество элементов
					, 	1					//	Шаг (stride)
					}; 	
	v. resize(n); 							//	resize() не сохраняет значения элементов
	v[slc] 						= 	tmp;		//	копируем элементы назад в начальную часть 'v'
}
static void		test_4()	{	
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	valarray<double> v{1.2, 3.14, 2.7};
	f_w_grow(v, 7);
	for (size_t idx = 0; idx < v.size(); ++idx)		cout << v[idx] << " ";	cout << endl;
	
	v[3]						=	273.5;
	f_w_grow_2(v, 10);
	for (size_t idx = 0; idx < v.size(); ++idx)		cout << v[idx] << " ";	cout << endl;
}
/*
	***********************************************************************************************
	22.4.5. Срезы
 
	'Срез' (slice) 	— 	это абстракция, которая позволяет эффективно работать 
							с векторами как с матрицами произвольной размерности. 
							
	Это ключевое понятие для векторов языка Fortran и библиотеки BLAS (Basic Linear Algebra Subprograms), 
	являющейся базисом для многих численных расчетов. 
	
	https://en.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms	
	https://ru.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms
	
	Объект типа std::slice содержит три целочисленных значения: 
	
	-	начальный индекс, 
	-	шаг (stride)
	и 
	-	общее количество значений в подмножестве. 
	
	Объекты типа std::slice 
	можно использовать в качестве индексов 
	с помощью оператора [].
*/
size_t 	slice_index (const slice& slc, size_t i)	{
	// cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	return slc. start() + i * slc. stride();
} 
// вывод элементов среза slc
void		print_seq 	(const slice& slc) 	 {
	// cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	for (size_t i = 0; i < slc .size(); i++) 
		cout << slice_index(slc, i) << " ";
	//cout << endl;
}	
void		f_w_slice_index() 	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	/*
		std::slice(start, number, stride)

		Логика такова: соответствующими формированиями индексов
		мы как бы формируем двумерные массивы различных размерностей,
		в зависимости от параметров конструктора класса std::slice
	*/
	print_seq	(std::slice{ 0, 3, 4 }); 		// 	строка 0		0   4   8
	cout << ", ";				
	
	print_seq	(std::slice{ 1, 3, 4 }); 		// 	строка 1		1   5   9
	cout << ", ";
	
	print_seq	(std::slice{ 2, 3, 4 }); 		// 	строка 2		2   6   10
	cout << ", ";
	
	print_seq	(std::slice{ 3, 3, 4 }); 		// 	строка 3		3   7   11
	cout << ", ";
	
	print_seq	(std::slice{ 0, 4, 1 }); 		// 	столбец 0      0
												//					1
												//					2
												//					3					
	cout << ", ";
	
	print_seq 	(std::slice{ 4, 4, 1}); 			// 	столбец 1			4
												//						5
												//						6
												//						7
	cout << ", ";
	
	print_seq 	(std::slice{ 8, 4, 1}); 			// 	столбец 2				8
												//							9
												//							10
												//							11
	cout << endl;

}	
/*
	Другими словами, 'срез' описывает отображение неотрицательных целых чисел в индексы. 
	
	Число элементов (size()) не влияет на отображение, 
	а просто позволяет нам найти конец последовательности. 
	
	Такое отображение позволяет 
	имитировать 'двумерные массивы'
	внутри 'одномерного массива' (такого как valarray)
	эффективно, универсально и относительно просто (удобно). 
*/
static void		test_5()	{	
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	f_w_slice_index();
}
/*
	*********************	slice_array		*********************************************************
	https://runebook.dev/ru/docs/cpp/numeric/valarray/slice_array
		
	22.4.6. Массив slice_array
		
	Из массива std::valarray и среза std::slice 
	мы можем построить нечто похожее на std::valarray, 
	но на самом деле являющееся лишь способом 
	'обратиться к "подмножеству" массива', 
	"адресуемого срезом". 
		
	Это std::slice_array, который определяется следующим образом:	-----> Ch_22_4_Vector_Arithmetic_cont.hpp

	Пользователь не может 
	напрямую создавать объекты типа std::slice_array
	из среза std::slice, 
	так как нет соответствующего конструктора у std::slice_array. 
	
	Вместо этого пользователь индексирует std::valarray, 
	чтобы создать std::slice_array для данного среза std::slice. 
	
	Это связано с тем, что класс std::valarray 
	не имеет конструктора с парметром среза, 
	а имеет конструктор с параметром массива среза.

	Как только std::slice_array инициализирован, 
	все обращения к нему косвенно передаются std::valarray, 
	для которого он создан. 
	
	Например, мы можем создать нечто, 
	что представляет каждый второй элемент массива 
	следующим образом:
*/
void		f_w_slice_array (valarray<double>& 	v_a_dbl)	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	//	С учетом начала нулевым индексом, количество четных членов равно или на единицу больше нечетных
	const auto					idx_for_even		=	std::slice	(	0
																, 	v_a_dbl.size() / 2 + v_a_dbl.size() % 2
																, 	2
																);
	// 	Создаем объект std::slice_array индексируя срезом std::slice массив std::valarray
	const std::slice_array<double>& 	 s_a_even		= 	v_a_dbl[idx_for_even];
	// 	Создаем объект std::valarray, индексируя массиво-срезом std::slice_array  
	std::valarray	v_even	=	valarray<double>{s_a_even};
	//	Попробую это сделать напрямую через индексацию срезом
	//	НЕ КАТИТ!!!!!!! std::valarray	v_even	=	valarray<double>{idx_for_even};
	for (size_t idx = 0; idx < v_even.size(); ++idx)		cout << v_even[idx] << " ";	cout << endl;	
	/*
		---------------------------------------------------------------------------------------
		Таким образом, чтобы создать массив std::valarray из другого массива std::valarray,
		с использованием выборки элементов при помощи среза std::slice,
		необходимо выполнить следующую последовательность операций:
		
		1. 	Создать срез std::slice из исходного массива std::valarray.
		
		2.	Косвенно создать массив-срез std::slice_array, 
			индексируя срезом std::slice снова таки исходный массив std::valarray.
			
		3.	Создать новый, производный срезом массив std::valarray,
			используя соответствующий конструктор 
			valarray(const std::slice_array<T>& s_a);	//	(7)	.
		---------------------------------------------------------------------------------------
	*/
	//	1. 	Создать срез std::slice из исходного массива std::valarray.
	const std::slice				idx_for_odd		=	std::slice 	{	1
																,	v_a_dbl.size() / 2
																,	2
																};
	//	2.	Косвенно создать массив-срез std::slice_array, индексируя срезом std::slice исходный массив valarray
	const std::slice_array<double>& 	s_a_odd 		= 	v_a_dbl[idx_for_odd];
	
	//	3.	Создать новый массив std::valarray, используя конструктор std::valarray(const std::slice_array<T>&)
	std::valarray<double>				v_odd		=	std::valarray<double>	{s_a_odd};
	
	for (size_t idx = 0; idx < v_odd.size(); ++idx)		cout << v_odd[idx] << " ";	cout << endl;	
	
	//	Размеры массивов v_odd и v_even должны быть одинаковы, иначе исключение может завалить прогу
	#if defined (__clang__) 
	// 	clang не хочет признавать составную операцию умножения; 
	v_even = v_even * v_odd;	
	#elif defined (__GNUC__) 	|| 	defined (__GNUG__) || defined (_MSC_VER) || defined (_MSC_FULL_VER) 
	v_even *= (v_odd);		// 	перемножить элементы попарно и сохранить результат в четных элементах
	#endif
	for (size_t idx = 0; idx < v_even.size(); ++idx)		cout << v_even[idx] << " ";	cout << endl;	
	
	v_odd = 0;							// 	присвоить 0 каждому нечетному элементу 'v_a_dbl'
}
/*
	Запрет на копирование slice_array необходим, чтобы поддержать оптимизации, 
	рассчитывающие на отсутствие синонимов объектов. 
	
	Это может оказаться стеснительным. 
	Например:
*/
#if 0	
std::slice_array<double> 	f_w_row (valarray<double>& v_a_dbl, int i)
{
	std::slice_array<double> 	v 	= 	v_a_dbl[std::slice(0, 2, v_a_dbl.size() / 2)]; 	//	error: попытка копирования 
	
	return 	v_a_dbl[slice(i % 2, i, v_a_dbl.size() / 2)]; 							//	error: попытка копирования
}
#endif	
/*
	Часто копирование срезов является разумной альтернативой копирования slice_array.
	
	Срезы могут использоваться для формирования разных подмножеств массивов. 
	
	Например, можно применить срезы для манипулирования непрерывно расположенными подмассивами 
	следующим образом:
*/
static inline std::slice 	sub_array (size_t first, size_t count) 
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	return 	std::slice (first, count, 1);			// 	[first; first + count[
}	
void		f_w_slice_valarray (valarray<double>& v)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;	
	size_t 	sz 	= 	v.size();
	
	if	(sz < 2)
		return;
	
	size_t 	n 	= 	sz / 2;		//	При нечетном количестве членов - на единицу меньше, чем вторая половина
	size_t 	n2 	= 	sz - n;
	
	valarray<double> 	half1 (n);
	valarray<double> 	half2 (n2);
	
	half1 		= 	v[sub_array(0, n)]; 			// 	копирование первой половины 'v'
	for (size_t idx = 0; idx < half1.size(); ++idx)		cout << half1[idx] << " ";	cout << endl;	
	half2 		= 	v[sub_array(n, n2)]; 		// 	копирование второй половины 'v'
	for (size_t idx = 0; idx < half2.size(); ++idx)		cout << half2[idx] << " ";	cout << endl;	
	//...
}
static void		test_6()	{	
	cout << "\n-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	valarray<double> v{1.2, 3.14, 2.7, 273.5, 9.81, 7.23};
	f_w_slice_array (v);
	
	f_w_slice_valarray (v);
}	
/*
	***********************************************************************************
	22.4.8. Обобщенные срезы
*/
void 	print_planes_rows_cols	(	std::valarray<int>& 	v
								, 	int 					planes	//	Плоскости
								, 	int 					rows		//	Строки в плоскости
								, 	int 					cols		//	Столбцы в плоскости
								)		{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	if (v.size() != (planes * rows * 	cols	)) {
		cout << "Ошибка в переданных параметрах. Завершение работы функции!" << endl;
		return;
	}	
		
	//	Цикл по плоскостям
    for (int z = 0; z < planes; ++z)	    {
		//	Цикл по строкам плоскостей
        for (int r = 0; r < rows; ++r)	    {
			//	Цикл по столбцам плоскостей
			for (int c = 0; c < cols; ++c)	
                cout << v[z * rows * cols + r * cols + c] << ' ';
            cout << endl;
        }
        cout << endl;
    }
}
void 	first_gslice_test()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
    std::valarray<int> 	v 	= 	// 3D-массив: 2 x 4 x 3 элемента
								{	111, 112, 113, 
									121, 122, 123, 
									131, 132, 133, 
									141, 142, 143,
									
									211, 212, 213, 
									221, 222, 223, 
									231, 232, 233, 
									241, 242, 243
								};
								
    // 	целое число ar3d[2][4][3]
    cout << "Initial 2 x 4 x 3 array:\n";
    print_planes_rows_cols(v, 2, 4, 3);	//	2 плоскости х 4 строки х 3 столбца
	
    // обновляем каждое значение в первых столбцах обеих плоскостей
    v[std::gslice{0, {2, 4}, {4 * 3, 3}}] = 1; 	// 	два шага первого уровня по 12 элементов
												// 	затем четыре уровня по два шага по 3 элемента
	print_planes_rows_cols(v, 2, 4, 3);
	
    // вычитаем третий столбец из второго столбца в 1-й плоскости
	#if defined (__clang__)  
	// 	clang не хочет признавать составную операцию умножения; 
	v[std::gslice(1, {1, 4}, {4 * 3, 3})] 	= 	std::valarray<int>(v[std::gslice(1, {1, 4}, {4 * 3, 3})])
											-	std::valarray<int>(v[std::gslice(2, {1, 4}, {4 * 3, 3})]);
	#elif defined (__GNUC__) 	|| 	defined (__GNUG__) || defined (_MSC_VER) || defined (_MSC_FULL_VER)
	v[std::gslice(1, {1, 4}, {4 * 3, 3})] -= v[std::gslice(2, {1, 4}, {4 * 3, 3})];
	#endif
 
    cout << "\n" "After column operations:\n";
    print_planes_rows_cols(v, 2, 4, 3);
}
/*
	Дополнительные значения позволяют std::gslice определить соответствие 
	между 'n' целыми числами и индексом, которым адресуются элементы массива. 
	
	Например, мы можем описать размещение матрицы 2 x 3 двумя парами (длина, шаг) целых чисел. 
	
	При расположении элементов в стиле языка Fortran 
	длина 2 и шаг 4 описывают два элемента строки матрицы 3 x 4. 
	
	Аналогично, длина 3 и шаг 1 описывают три элемента столбца. 
	
	В совокупности они описывают элементы подматрицы 2 x 3. 
	
	Для перечисления элементов мы можем написать:
*/ 
size_t 	gslice_index	(	const std::gslice& 	gslc
					, 	size_t				i
					, 	size_t				j
					) 	{
		//cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;				
		return 		gslc.start() 
				+ 	i * gslc.stride()[0] 
				+ 	j * gslc.stride()[1]
				;
}
void 	second_gslice_test()	{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;	
	
	//	Вспомогательные массивы для последующей иницилизации std::valarray<size_t> lengths и strides
	size_t 					len[] 	= 	{2, 3}; 		// 	(len[0], str[0]) описывают строку
	size_t 					str[] 	= 	{4, 1}; 		// 	(len[1], str[1]) описывают столбец
	
	//	std::valarray(const T* vals, std::size_t count);	//	count элементов co значениями p[0], p[1], ...
	
	std::valarray<size_t> 	lengths {len, 2};
	for (size_t idx = 0; idx < lengths.size(); ++idx)		cout << lengths[idx] << " ";	cout << endl;	
	
	std::valarray<size_t> 	strides {str, 2};
	for (size_t idx = 0; idx < strides.size(); ++idx)		cout << strides[idx] << " ";	cout << endl;	

	/*
		std::gslice		( 	std::size_t 							start
						, 	const std::valarray<std::size_t>& 	sizes
						,   const std::valarray<std::size_t>& 	strides 
						);
	*/
	std::gslice 				gslc 	{	0			//	start
									, 	lengths		//	sizes
									, 	strides		//	strides
									};
	
	for (int i = 0; i < gslc.size()[0]; i++) 
		cout << gslice_index (gslc, i, 0) << " "; 	//	строка 
	cout		<< ", ";
	
	for	(int j = 0; j < gslc.size()[1]; j++) 
		cout << gslice_index (gslc, 0, j) << " "; 	//	столбец
	cout << endl;
}
static void		test_7()	{	
	cout << "\n-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	first_gslice_test();
	
	second_gslice_test();
}	
/*
	***************************		main()		******************************************
*/
int main()
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << " of Ch_22_4_Vector_Arithmetic"  << endl;
	
    try {
		//test_1();
		//test_2();
		//test_3();
		//test_4();
		//test_5();
		//test_6();
		test_7();
				
		system("pause");
    }
    catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return  EXIT_FAILURE;
    }
	system("pause");
}
