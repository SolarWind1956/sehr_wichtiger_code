// conceptClassTemplate.cpp

#include <concepts> 
#include <vector> 
#include <iostream>     
using std::cout; using std::endl;

class Handle	{
 public:
	Handle* clone()	const { 
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		return new Handle(*this); 
	}
};
template <class T>
	requires requires (T t) { t.clone(); }
class test_class	{
 public:
	T*		t_ptr{nullptr};
	T*		h_ptr{nullptr};
	
	test_class(){
		cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
		h_ptr	= t_ptr->clone();	
	}
};
template <class T>
	requires requires (T t) { t.clone(); }
void		test_func(T* t_par)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	T*	h_ptr	= t_par->clone();
}
template 	< 	template <typename> class 	T
			, 	typename 					H
			>
	requires requires (T<H> t, H h) 	{ 			t.t_ptr;		h.clone();	}
void		test_func_2(T<H>& t_ref)
{
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
	H*	h_ptr	= t_ref.t_ptr->clone();
}

template <std::regular T>
class MyVector{};

int main() {
	cout << "-------------- I'm " <<  __PRETTY_FUNCTION__  << endl;
    MyVector<int> myVec1;
    //	MyVector<int&> myVec2;  // ERROR because a reference is not regular
	cout << "debbug marker 0" << endl;
	test_class<Handle>	t_c_h;
	//test_class<int>		t_c_i;
	cout << "debbug marker 1" << endl;
	Handle 	h;
	cout << "debbug marker 2" << endl;
	test_func<Handle>(&h);
	
	test_func_2<test_class, Handle>(t_c_h);
	//	test_func_2<std::vector, Handle>(t_c_h);	std::vector не имеет поля 't_ptr' 
	
}