#ifndef GUARD_threadsafe_queue_aw_hpp
#define GUARD_threadsafe_queue_aw_hpp
/*
Энтони Уильямс. C++. Практика многопоточного программирования. 2-е издание. 2020 год.
Anthony Williams. C++ Concurrency in Action. Second Edition. 2020.

"Глава 6. Проектирование параллельных структур данных с блокировками"
Chapter 6. Designing lock-based concurrent data structures
 */
 // Стандартная библиотека
#include "my_std_library.h"
#include "my_process_of_exeptions.h"
#include "Polygon.h"

namespace ns_threadsafe_queue_aw
{
	/*
	*************************************************************************************************
	"Глава 6. Проектирование параллельных структур данных с блокировками"
	Chapter 6. Designing lock-based concurrent data structures
	***********************************************************************************************

	*************************************************************************************************
	Раздел 6.2. Параллельные структуры данных с блокировками
	*************************************************************************************************
	Проектирование параллельных структур данных с блокировками сводится к тому,
чтобы захватить нужный мьютекс при доступе к данным и удерживать его минимально возможное время.

	*************************************************************************************************
	Параграф 6.2.2. Потокобезопасная очередь с блокировками и условными переменными
	*************************************************************************************************
	Если стек построен по образцу std::stack<>, то очередь – по образцу std::queue<>.
Но ее интерфейс также отличается от стандартного адаптера контейнера, потому что запись в структуру данных
должна быть безопасной относительно одновременного доступа из нескольких потоков.
	*************************************************************************************************
	/*
	Листинг 6.7.   	Потокобезопасная очередь с блокировкой и ожиданием : внутренние данные и интерфейс
	Листинг 6.8.   	Потокобезопасная очередь с блокировкой и ожиданием : добавление новых значений
	Листинг 6.9.   	Потокобезопасная очередь с блокировкой и ожиданием : wait_and_pop()
	Листинг 6.10. 	Потокобезопасная очередь с блокировкой и ожиданием : try_pop() и empty()
	*/
	/*	Чтобы предотвратить гонку данных при вталкивании и выталкивании элементов,
		заранее создаем фиктивный узел, не содержащий данных, и тем самым гарантируем,
		что в очереди всегда есть хотя бы один узел, отделяющий голову от хвоста.

		Данная очередь неограниченная, то есть в нее можно помещать и помещать данные,
		ничего не удаляя, пока не кончится память.
	*/

	template<typename T>
	class threadsafe_queue_aw
	{
	private:
		// Узел очереди 
		struct node
		{
			std::shared_ptr<T>		data_sh_ptr;			// Разделяемый указатель на данные
			std::unique_ptr<node>	next_u_ptr;			// Уникальный указатель на следующий элемент   
		};

		std::mutex					head_mutex;
		std::unique_ptr<node>		head_u_ptr;			// Уникальный указатель на голову очереди-списка

		std::mutex					tail_mutex;
		node* tail_ptr;										// Простой указатель на хвост очереди

		std::condition_variable		data_cond;

		node* get_tail_ptr()
		{
			std::lock_guard<std::mutex> tail_lock(tail_mutex);

			return tail_ptr;
		}
		// Mодификация очереди в результате удаления головного элемента
		// Фиктивный узел остается в хвосте очереди.
		// pop_head() используется также в функции try_pop()
		std::unique_ptr<node> pop_head()
		{
			std::unique_ptr<node> 		old_head_u_ptr = std::move(head_u_ptr);

			head_u_ptr = std::move(old_head_u_ptr->next_u_ptr);
			// Возвращаем указатель на вытолкнутый узел, чтобы можно было извлечь из него данные
			return std::move(old_head_u_ptr);
		}
		// Oжидание появления данных в очереди
		// Oна не только ждет условную переменную, используя лямбда-функцию в качестве предиката, 
		// но и возвращает объект блокировки вызывающей программе.
		// Тем самым мы гарантируем, что та же самая блокировка удерживается, 
		// пока данные модифицируются в соответствующем перегруженном варианте wait_pop_head()
		std::unique_lock<std::mutex> wait_for_data()
		{
			std::unique_lock<std::mutex> head_lock(head_mutex);

			node* ptr = head_u_ptr.get();
			data_cond.wait(head_lock, [&]() { return (ptr != get_tail_ptr()); });

			return std::move(head_lock);
		}

		std::unique_ptr<node> wait_pop_head()
		{
			std::unique_lock<std::mutex> head_lock(wait_for_data());

			return std::move(pop_head());
		}

		std::unique_ptr<node> wait_pop_head(T& value)
		{
			std::unique_lock<std::mutex> head_lock(wait_for_data());

			value = std::move(*head_u_ptr->data_sh_ptr);
			return std::move(pop_head());
		}
		std::unique_ptr<node> try_pop_head()
		{
			std::lock_guard<std::mutex> head_lock(head_mutex);

			if (head_u_ptr.get() == get_tail_ptr())
			{
				return std::unique_ptr<node>();
			}
			return std::move(pop_head());
		}

		std::unique_ptr<node> try_pop_head(T& value)
		{
			std::lock_guard<std::mutex> head_lock(head_mutex);

			if (head_u_ptr.get() == get_tail_ptr())
			{
				return std::unique_ptr<node>();
			}
			value = std::move(*head_u_ptr->data_sh_ptr);
			return std::move(pop_head());
		}

	public:
		// *************** Конструкторы и операторы присваивания или перемещения ******************

		// Kонструктор по умолчанию сразу создает фиктивный узел, и инициализирует
		// уникальный указатель на голову			head_u_ptr
		// и простой указатель на хвост очереди		tail_ptr
		threadsafe_queue_aw() : head_u_ptr(new node), tail_ptr(head_u_ptr.get()) {}
		// Конструктор копирования и оператор копирующего присваивания запрещаем, так как имеются 
		// уникальные указатели
		threadsafe_queue_aw(const threadsafe_queue_aw& other) = delete;
		threadsafe_queue_aw& operator=(const threadsafe_queue_aw& other) = delete;

		std::shared_ptr<T> try_pop()
		{
			std::unique_ptr<node> const old_head_u_ptr = try_pop_head();

			return old_head_u_ptr ? old_head_u_ptr->data_sh_ptr : std::shared_ptr<T>();
		}

		bool try_pop(T& value)
		{
			std::unique_ptr<node> const old_head_u_ptr = try_pop_head(value);
			return old_head_u_ptr;
		}
		// ************************** wait_and_pop ********************* Листинг 6.9 ********************
		std::shared_ptr<T> wait_and_pop()
		{
			std::unique_ptr<node> const old_head_u_ptr = std::move(wait_pop_head());

			return old_head_u_ptr->data_sh_ptr;
		}

		void wait_and_pop(T& value)
		{
			std::unique_ptr<node> const old_head_u_ptr = std::move(wait_pop_head(value));
		}
		// ************************** push ****************************** Листинг 6.8 ********************
		void push(T new_value)
		{
			// Cоздаем разделямый указатель на переданные данные для вталкиваемого узла
			std::shared_ptr<T>		new_data_sh_ptr(std::make_shared<T>(std::move(new_value)));
			// Создаем узел для вталкивания в конец очереди
			std::unique_ptr<node>	u_ptr(new node);
			// Далее каскад операторов под мьютексом в блоке, чтобы мьютекс потом был уничтожен компилятором
			{
				std::lock_guard<std::mutex> tail_lock(tail_mutex);
				// В фиктивном узле в хвосте очереди корректируем укатель на реальные данные
				tail_ptr->data_sh_ptr = new_data_sh_ptr;
				// Создаем простой уазатель на созданный для вставки в очередь узел
				node* const new_tail_ptr = u_ptr.get();
				// Теперь фиктивный элемент будет указывать на созданный элемент, который получит статус фиктивного
				// А фиктивный элемент перестает быть фиктивным
				tail_ptr->next_u_ptr = std::move(u_ptr);
				// И хвост очереди теперь будет указывать на новый фиктивный элемент
				tail_ptr = new_tail_ptr;
			}
			data_cond.notify_one();
		}
		bool empty()
		{
			std::lock_guard<std::mutex> head_lock(head_mutex);
			return (head_u_ptr.get() == get_tail_ptr());
		}
	};

};	// End of the namespace 'ns_threadsafe_queue_aw'
/*
*	*********************************************************************************
*	РЕЗЮМЕ
*	*********************************************************************************

*/
//		#ifndef GUARD_threadsafe_queue_aw_hpp
#endif
