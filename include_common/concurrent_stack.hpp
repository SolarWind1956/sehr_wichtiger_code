#pragma once

/*
	************************ 2.3.3.1.7. Потокобезопасный односвязный список
	https://www.modernescpp.com/index.php/atomic-smart-pointers/
*/
#include <memory>
using std::shared_ptr;
using std::make_shared;

// #include <experimental/atomic> using std::atomic_shared_ptr;

template<typename T> 
class 	concurrent_stack 
{
    struct 	Node 
	{ 
		T 					t; 
		shared_ptr<Node> 	next; 
	};
	
	//std::atomic<shared_ptr<Node>>	head{nullptr};
    shared_ptr<Node> 		head;	
									/* 
										In C++11: remove “atomic_” 
										and remember to use the special functions 
										every time you touch the variable.
										
										В стандате C++11: удалите префикс «atomic_» 
										и не забывайте использовать специальные функции каждый раз, 
										когда вы касаетесь переменной.
									*/
	// Копировать и присваивать копированием нельзя!!!								
    concurrent_stack(concurrent_stack &) 	=	delete;
    void operator=(concurrent_stack&) 		=	delete;
public:
    concurrent_stack()
	{
		cout << "I'm default CTOR concurrent_stack()" << endl;
	}
    ~concurrent_stack()		=	default;
	
    class reference {
       shared_ptr<Node> 	sh_ptr;
    public:
       reference(shared_ptr<Node> sh_ptr_) : sh_ptr{sh_ptr_} 
	   {}
       T& operator* () { return sh_ptr->t; }
       T* operator->() { return &sh_ptr->t; }
    };

    auto find( T t ) const {
        //auto sh_ptr = head.load();  			// in C++11: atomic_load(&head)
		auto sh_ptr = atomic_load(&head);  		// in C++11: atomic_load(&head)
		
        while(sh_ptr && sh_ptr->t != t)
            sh_ptr = sh_ptr->next;
        return reference(move(sh_ptr));
    }
    auto front() const {
		//return reference(head); 				// in C++11: atomic_load(&head)
		return reference(atomic_load(&head)); 	// in C++11: atomic_load(&head)
    }
    void push_front( T t ) 
	{
		auto sh_ptr 	= make_shared<Node>();
		sh_ptr->t 	= t;
		// sh_ptr->next 	= head;         	// in C++11: atomic_load(&head)
		sh_ptr->next 	= atomic_load(&head);   // in C++11: atomic_load(&head)
		//while( !head.compare_exchange_weak(sh_ptr->next, sh_ptr) ){ }
		while( !atomic_compare_exchange_weak(&head, &sh_ptr->next, sh_ptr) ){ }
											// in C++11: atomic_compare_exchange_weak(&head, &sh_ptr->next, sh_ptr);
    }
    void pop_front() {
       auto sh_ptr = head.load();
       // while( sh_ptr && !head.compare_exchange_weak(sh_ptr, sh_ptr->next) ){ }
	   while( sh_ptr && !atomic_compare_exchange_weak(&head, &sh_ptr, sh_ptr->next) ){ }
											// in C++11: atomic_compare_exchange_weak(&head, &sh_ptr, sh_ptr->next);
    }
};
/*	
	https://github.com/flexxxxer/concurrent-stack/blob/master/source/concurrent-stack.h
	

#include <atomic>
#include <optional>

template <typename  T>
class concurrent_stack
{
public:
	concurrent_stack() : top_(nullptr) {}

	void push(const T& value) noexcept
	{
		stack_node<T>* new_node = new stack_node<T>(value, this->top_.load());

		while (!this->top_.compare_exchange_weak(new_node->next, new_node));
	}
	std::optional<T> pop() noexcept
	{
		stack_node<T>* top = this->top_.load();

		if (top == nullptr)
			return std::optional<T>();

		stack_node<T>* new_front = top->next;

		while (!this->top_.compare_exchange_weak(top, new_front))
		{
			if (top == nullptr)
				return std::optional<T>();

			new_front = top->next;
		}

		T value = top->value;
		delete top;

		return std::optional<T>(value);
	}
	std::optional<T> top() const noexcept
	{
		stack_node<T>* top = this->top_.load(std::memory_order_acquire);

		if (top == nullptr)
			return std::optional<T>();

		return std::optional<T>(top->value);
	}

	[[nodiscard]] bool is_empty() const noexcept
	{
		return this->top_.load() == nullptr;
	}

private:

	template <typename X>
	struct stack_node
	{
		X value;
		stack_node<X>* next;

		explicit stack_node(const X& value) : value(value), next(nullptr) {}
		explicit stack_node(const X& value, stack_node<X>* next) : value(value), next(next) {}
	};

	std::atomic<stack_node<T>*> top_;
};
*/