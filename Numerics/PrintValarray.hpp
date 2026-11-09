//: C07:PrintValarray.hpp
#ifndef INCLUDE_GUARDS_PRINTVALARRAY_H
#define INCLUDE_GUARDS_PRINTVALARRAY_H
#include <valarray>
#include <iostream>
using std::cout; using std::endl; using std::boolalpha; using std::cin; using std::cerr;
#include <iomanip>
using std::setw;
#include <cstddef>
#include <cassert>

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
void 	print_3D		(	std::valarray<int>& 	v
					, 	int 					planes	//	Плоскости
					, 	int 					rows		//	Строки в плоскости
					, 	int 					cols		//	Столбцы в плоскости
					)		{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
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
//	--------------------------------------------------------------------------------------------------
// Вывод объекта valarray в виде матрицы
template<class T>
void 	print_Matrix	(	const valarray<T>& 	a
					, 	size_t 				n
					) 
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	size_t 	siz 	= 	n * n;
	assert	(siz <= a.size());
	for	(size_t  i = 0; i < siz; ++i) {
		cout << setw(5) << a[i];
		cout << ((i + 1) % n ? ' ' : '\n');
	}
	cout << endl;
}
//	------------------------------------------------------------------
/*
	--------------------------------------------------------------------------------------------------
*/
#endif // INCLUDE_GUARDS_PRINTVALARRAY_H