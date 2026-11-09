/* 
	BRUCE_ECKEL_Numerics.cpp
*/
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <iomanip>
using std::setw;

#include <memory>
#include <iterator>
#include <map>
#include <list>
#include <exception>
#include <vector>

#include <numeric>
#include <limits>
#include <climits>
#include <cmath> 
#include <math.h>
#include <cerrno>
#include <valarray>
#include <cassert>

using namespace std;
#include "PrintValarray.hpp"
#include "BRUCE_ECKEL_Numerics.hpp"

#include "..\type_name_as_str.h"
#include "..\separate_number_with_apostrophe.hpp"
#include "..\integ_to_bin_str.hpp"
/*
	**************************************************************************************
	Библиотека Basic Linear Algebra Subprograms (BLAS) 
	содержит основные средства для реализации операций линейной алгебры, 
	таких как умножение матриц, решение треугольных систем и простые векторные операции.
	https://ru.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms
	**************************************************************************************
	Шаблон std::valarray<T> представляет собой аналог контейнера vector, 
	оптимизированный для эффективных математических вычислений. 
	
	Oн не поддерживает итераторов. 
	
	Хотя std::valarray<T> может специализироваться для нечисловых типов, 
	этот шаблон содержит математические функции, 
	ориентированные на работу с числовыми данными: sin, cos, tan и т.д.
 
	Определим вспомогательный шаблон для вывода элементов std::valarray<T>:	---------> .hpp
	
	Большинство функций и операторов valarray ориентировано на работу не с отдельными элементами, 
	а с массивом valarray в целом, как показывает следующий пример:
*/ 
double f_any_expr	(double x) 	{ 
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;	
	return 2.0 * x - 1.0; 
}

static void		test_1()	
{
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
	
	double 				n[] = {1.0, 2.0, 3.0, 4.0};
	
	valarray<double> 	v	{	n
							, 	sizeof n / sizeof n[0]
							};
	print_valarray("v", v);
	
	valarray<double> 	sh	{	v.shift(1)
							};
	print_valarray("shift 1", sh);
	
	valarray<double> 	acc	{	v + sh
							};
	print_valarray("sum", acc);
	
	valarray<double> 	trig	{	sin(v) + cos(acc)
							};
	print_valarray("trig", trig);
	
	valarray<double> 	p	{	pow(v, 3.0)
							};
	print_valarray("3rd power", p);
	
	valarray<double> 	app	{	v.apply(f_any_expr)
							};
	print_valarray("f_any_expr(v)", app);
	
	valarray<bool> 		eq	{	v == app
							};
	print_valarray("v == app?", eq);
	
	double 				x 	= 	v.min();
	double 				y 	= 	v.max();
	double 				z 	= 	v.sum();
	cout << "x = v.min() == " << x << ", y = v.max() == " << y << ", z = v.sum() == " << z	<< endl;
}
/*
	Однако самая необычная операция с valarray — 
	выделение подмножеств элементов, 
	причем не только для получения информации, 
	но и для ее обновления. 
	
	Подмножество элементов valarray называется срезом. 
	
	Срезы используются некоторыми операторами. 
	
	Следующий пример демонстрирует работу со срезами:
	----------------------------------------------------------------------------------------------------
*/
static void		test_2()	
{
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
	
	int 	data[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
	valarray<int> 	v	(data, 12);
	valarray<int> 	r1	(v[slice(0, 4, 3)]);
	print_valarray("slice(0, 4, 3) - выборка четырех элементов - каждый третий с нулевого: ", r1);
	
	cout << "\n------------------------- Выделение элементов по условию" << endl;
	valarray<int> 	r2	(v[v > 60]);
	print_valarray("elements > 60", r2);
	
	cout << "\n------------------------- Возведение в квадрат первого столбца" << endl;
	v[slice(0, 4, 3)] *= valarray<int>(v[slice(0, 4, 3)]);
	print_valarray("after squaring first column", v);
	
	cout << "\n------------------------- Восстановление исходных значений" << endl;
	int 				idx[] = {10, 40, 70, 100};
	valarray<int> 	save		{idx, 4};
	v[slice(0, 4, 3)] = save;
	print_valarray("v restored", v);
	
	cout << "\n------------------------- Выделение двухмерного подмножества: {{10,  30,  50}, {70,  90, 110}}" << endl;
	
	//	-----------------------------------------------------------------------------------------------
	#if 0	//	Многострочный комментарий для кода - меняем 0 на 1, и фрагмент будет компилироваться
	valarray<size_t> 	siz	{2};
	siz[0] = 2;						//	Количество измерений	
	siz[1] = 3;						//	Количество элементов
	valarray<size_t> 	gap	{2};
	gap[0] = 6;						//	Шаг от начала первого подмножества к началу второго
	gap[1] = 2;						//	Шаг между элементами каждого подмножества
	valarray<int> 		r3	{	v	[std::gslice	{	0
												, 	const_cast<const valarray<size_t>&>(siz)
												, 	const_cast<const valarray<size_t>&>(gap)
												}
									]
							};
	#endif	//	Конец фрагмента закомментированного кода						
	//	-----------------------------------------------------------------------------------------------
	valarray<int> 		r3	{v[std::gslice(0, {2, 3}, {6, 2})]};
	printValarray_line_by_line(r3, 3);
	print_valarray("2-d slice", r3);
	print_3D(r3, 1, 2, 3);
	
	cout << "\n------------------------- Выделение подмножества по логической маске (элементы bool)" << endl;
	valarray<bool> 	mask(false, 5);
	mask[1] = mask[2] = mask[4] = true;
	valarray<int> 	r4(v[mask]);
	print_valarray("v[mask]", 	r4);
	
	cout << "\n------------------------- Выделение подмножества по индексной маске (элементы size_t)" << endl;
	size_t 				idx2[] = 	{2, 2, 3, 6};
	valarray<size_t> 	mask2		{idx2, 4};
	valarray<int> 		r5			{v[mask2]};
	print_valarray("v[mask2]", r5);
	
	cout << "\n------------------------- Использование индексной маски при присваивании" << endl;
	valarray<char> 		text	{"now is the time", 15};
	valarray<char> 		caps	{"NITT", 4};
	valarray<size_t> 	idx3	(4);
	idx3[0] 		= 0;
	idx3[1] 		= 4;
	idx3[2] 		= 7;
	idx3[3] 		= 11;
	
	text[idx3] 	= caps;
		
	print_valarray("capitalized", text);
}
/*
	******************************************************************************************
	Одним из примеров практического применения срезов является умножение матриц. 
	
	Посмотрим, как должна выглядеть функция для умножения двух целочисленных матриц 
	при использовании обычных массивов:
*/	
const	size_t	MAXCOLS{10};
void 	matrix_mult	(	const int 	mtrix_а		[][MAXCOLS]
					, 	size_t 		m_a
					, 	size_t 		n_a
					, 	const int 	mtrix_b		[][MAXCOLS]
					, 	size_t 		m_b
					,	size_t 		n_b
					, 	int 			result_mtrix	[][MAXCOLS]
					);
/*
	Функция умножает 
	матрицу mtrix_а с размерами m_a x n_a 
	на 
	матрицу mtrix_b с размерами m_b x n_b; 

	предполагается, что n_a и n_b равны. 
	
	Как видите, без применения объекта valarray или его аналога 
	приходится фиксировать максимальную вторую размерность 
	в каждой матрице. 
	
	Кроме того, такой подход затрудняет возвращение массива-результата по значению, 
	поэтому вызывающая сторона обычно передает его в дополнительном аргументе.
	
	Объекты valarray не только позволяют передавать матрицы произвольного размера, 
	но и упрощают обработку матриц любого типа и возврат результата по значению. 
	
	Вот как это делается:
*/
// Умножение совместимых матриц в формате valarray
template<class T>
valarray<T>		matr_mult	(	const valarray<T>& 	a
							, 	size_t 				arows
							, 	size_t 				acols
							,	const valarray<T>& 	b
							, 	size_t 				brows
							, 	size_t 				bcols
							) 
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	assert(acols == brows);
	
	valarray<T> 	result	{static_cast<std::valarray<T>>(arows * bcols)};
	
	for	(size_t i = 0; i < arows; ++i)
		for	(size_t j = 0; j < bcols; ++j) {
			// Внутреннее произведение строки a[i] и столбца b[j]
			valarray<T> row 			= 	a[std::slice(acols * i, 	acols, 	1)];
			valarray<T> col 			= 	b[std::slice(j, 			brows, 	bcols)];
			result[i * bcols + j] 	= 	(row * col).sum();
		}
	return 	result;
}

static void		test_3()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
	const int 		n{3};
	int 				adata[n * n] 	= 	{	 1,   0,  -1, 
											 2,   2,  -3, 
											 3,   4,   0
										};
	int 				bdata[n * n] 	= 	{	 3,   4,  -1, 
											 1,  -3,   0,  
											-1,   1,   2
										};
	
	valarray<int> 	a_matrix		{adata, n * n};
	valarray<int> 	b_matrix		{bdata, n * n};
	
	valarray<int> 	c_matrix		{matr_mult	(	a_matrix, 	n, 	n
											, 	b_matrix, 	n, 	n
											)
								};
	print_Matrix		(c_matrix, n);
}
static void		test_4()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}

static void		test_5()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}
static void		test_6()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}
static void		test_7()	{	
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< endl;
}

/*
	---------------------------------------------------------------------------------------
*/
/*
	***************************		main()		******************************************
*/
int main()
{
	cout << "-------------- I'm " <<	__PRETTY_FUNCTION__	<< " of BRUCE_ECKEL_Numerics"	<< endl;
	
		try {
		//test_1();
		//test_2();
		test_3();
		//test_4();
		//test_5();
		//test_6();
		//test_7();		
		
		}
		catch (const std::exception& e) {
				cerr << "Exception: " << e.what() << endl;
				return	EXIT_FAILURE;
		}
	system("pause");
}
