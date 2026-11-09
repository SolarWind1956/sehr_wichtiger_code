#ifndef GUARD_AW_chapter_4_hpp
#define GUARD_AW_chapter_4_hpp
/*
	Энтони Уильямс. C++. Практика многопоточного программирования. 2-е издание. 2020 год.
	Anthony Williams. C++ Concurrency in Action. Second Edition. 2020.

	Глава 4.	Синхронизация конкурентных операций
	Chapter 4.	Synchronizing concurrent operations
*/
// Стандартная библиотека
#include "my_std_library.h"
#include "my_process_of_exeptions.h"
#include "Polygon.h"

namespace ns_AW_chapter_4
{
	/*
		*********************************************************************************************
		Глава 4. Синхронизация конкурентных операций

		Hаиболее предпочтительным вариантом
		является использование средств из стандартной библиотеки C++,
		предназначенных для ожидания наступления самого события.

		Наиболее основательным механизмом ожидания события,
		инициируемого другим потоком является "условная переменная".

		Концептуально она связана с событием или другим условием,
		и один или несколько потоков могут ожидать выполнения условия.

		Когда поток обнаружит, что условие выполнено,
		он может известить об этом один или несколько потоков,
		ожидающих условную переменную,
		чтобы разбудить их и позволить продолжить работу.

		********************************************************************************************
		В большинстве случаев какому-нибудь потоку
		следует дожидаться наступления конкретного 'события'
		или создания определенного 'условия'.

		B стандартной библиотеке C++ имеются соответствующие средства
		в форме 'условных переменных' и 'будущих результатов' ('фьючерсов'),
		позволяющие справиться с данной задачей,
		а также новые средства синхронизации в форме 'защелок' (latches) и 'барьеров' (barriers).
		*****************************************************************************************
		Раздел 4.1. Ожидание наступления события или создания другого условия
		*****************************************************************************************
	*/
	bool			flag_4_1;
	std::mutex		mtx_4_1;

	void wait_for_flagmtx_4_1()
	{
		std::unique_lock<std::mutex>	lk(mtx_4_1);
		while (!flag_4_1)
		{
			// Разблокировка мьютекса - раз собираемся поспать, пусть попытаются другие потоки
			lk.unlock();
			// Здесь другой поток получает шанс заблокировать мьютекс для установки флага.
			std::this_thread::sleep_for(std::chrono::milliseconds(100));// Вхождение в спячку на 100 мс
			lk.lock(); 			// Повторная блокировка мьютекса
			// Возможно, здесь и повисим, если мьютекс заблокировал какой другой поток
		}
	};
	/*
		Наиболее основательным механизмом ожидания события,
		инициируемого другим потоком, является 'условная переменная'.

		Концептуально она связана с 'событием' или 'другим условием',
		и один или несколько потоков могут ожидать 'выполнения условия'.

		Когда поток обнаружит, что 'условие выполнено',
		он может известить об этом один или несколько потоков,
		ожидающих 'условную переменную', чтобы разбудить их и позволить продолжить работу.
		**********************************************************************************************
		Параграф 4.1.1. Ожидание 'выполнения условий' с применением 'условных переменных'
		***********************************************************************************************
		Листинг 4.1.  Ожидание завершения обработки данных с помощью std::condition_variable
	 */
	bool more_data_to_prepare_4_1() {
		cout << "\nI'm more_data_to_prepare_4_1() в потоке ID == "
			<< std::this_thread::get_id() << endl;
		return true;
	};
	// datachunk - блок данных
	struct data_chunk_4_1 {};

	data_chunk_4_1 prepare_data_4_1() {
		cout << "\nI'm prepare_data_4_1() в потоке ID == " << std::this_thread::get_id() << endl;
		return data_chunk_4_1();
	};

	void process_4_1(data_chunk_4_1&) {
		cout << "\nI'm process_4_1(data_chunk_4_1&) в потоке ID == "
			<< std::this_thread::get_id() << endl;
	};

	bool is_last_chunk_4_1(data_chunk_4_1&) {
		cout << "\nI'm is_last_chunk_4_1(data_chunk_4_1&) - поток ID == "
			<< std::this_thread::get_id() << endl;
		return true;
	};
	// Oчередь, используемая для передачи данных между двумя потоками.
	std::queue<data_chunk_4_1>		data_queue_4_1;
	std::mutex						mut_4_1;				// Мьютекс, защищающий эту очередь
	// Условная переменная, сигнализирующая, что данные подготовлены и помещены в очередь
	std::condition_variable			data_cond_4_1;

	void data_preparation_thread_4_1() {
		cout << "\nI'm data_preparation_thread_4_1() - поток ID == "
			<< std::this_thread::get_id() << endl;
		// До тех пор, пока еще есть данные для подготовки к обрабоке 
		// (об этом свидетельствует функция more_data_to_prepare_4_1())
		while (more_data_to_prepare_4_1())
		{
			// Готовим данные для помещения в очередь к обработке данных
			data_chunk_4_1 const		data = prepare_data_4_1();
			// Наличие этого блока {} ниже здесь принципиально важно!!!, 
			// чтобы при выходе из него сработала RAII на удаление блокировки
			{
				// Когда данные готовы, поток, подготавливавший данные, 
				// блокирует мьютекс, защищающий очередь, 
				// с помощью std::lock_guard 
				std::lock_guard<std::mutex>		lk(mut_4_1);
				// и помещает данные в очередь
				data_queue_4_1.push(data);
			}
			// Затем он вызывает в отношении экземпляра std::condition_variable
			// компонентную функцию notify_one() для оповещения ожидающего потока, 
			// который ждет выполнения этого условия, определяемого переменной data_cond_4_1,
			// что условие выполено: данные подготовлены и помещены в очередь
			data_cond_4_1.notify_one();
			break;
			// Kод для помещения данных в очередь находится в более узкой области видимости блока {},
			// поэтому условную переменную уведомляют после разблокировки мьютекса 
			// (при выходе из блока), 
			// чтобы при немедленном пробуждении ожидающего потока ему 
			// не пришлось бы снова быть заблокированным, 
			// ожидая разблокировки мьютекса.
		}
	};
	void data_processing_thread_4_1()
	{
		cout << "\nI'm data_processing_thread_4_1() в потоке ID == "
			<< std::this_thread::get_id() << endl;

		while (true) {
			// Oбрабатывающий поток первым делом блокирует мьютекс, 
			// но теперь уже с помощью std::unique_lock, а не std::lock_guard
			std::unique_lock<std::mutex>	lk(mut_4_1);
			// Затем поток вызывает в отношении std::condition_variable функцию wait(), 
			// передавая ей объект - блокировку и лямбда-функцию, которая выражает ожидаемое условие.
			data_cond_4_1.wait(lk, [] { return !data_queue_4_1.empty(); });
			/*
				Функция wait(),
				проснувшись по сигналу от условной переменной data_cond_4_1.notify_one(),
				проверяет условие, вызывая предоставленную лямбда - функцию,
				и возвращает управление, если условие соблюдено(лямбда - функция вернула true).

				Если условие не соблюдено (лямбда - функция вернула false),
				и значит пробуджение было ложным,
				функция wait() !!! снимает блокировку с мьютекса
				и вводит поток в состояние заблокированности или ожидания.

				Когда условная переменная уведомлена путем вызова функции notify_one() из потока,
				занимающегося подготовкой данных, поток пробуждается (разблокируется),
				повторно !!! получает блокировку мьютекса и снова проверяет условие,
				возвращаясь из wait() со все еще заблокированным мьютексом, если условие выполнено.

				Если условие не выполнено, поток снимает блокировку с мьютекса и возобновляет ожидание.

				Именно поэтому нужен std::unique_lock, а не std::lock_guard -
				ожидающий поток должен разблокировать мьютекс во время ожидания
				и после него заблокировать его снова,
				а std::lock_guard этой гибкости не обеспечивает.

				Если мьютекс остается заблокированным на период спячки потока,
				поток подготовки данных не сможет заблокировать мьютекс,
				чтобы добавить элемент в очередь,
				и ожидающий поток никогда не сможет увидеть его условие выполненным.
			*/
			// Считываем из очереди порцию данных для обрабоки
			data_chunk_4_1 data = data_queue_4_1.front();
			data_queue_4_1.pop();		// Продвигаем очередь

			lk.unlock();				// Снимаем блокировку мьютекса защиты очереди

			// Запускаем функцию обработки считанных из очереди данных
			process_4_1(data);

			// Если это была последняя порция данных, выходим из цикла
			if (is_last_chunk_4_1(data))
				break;
		}
	};
	void	StartAndJoinThreads_4_1() {
		cout << "\nI'm StartAndJoinThreads_4_1() - поток ID == " << std::this_thread::get_id() << endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		std::thread		t1_4_1(data_preparation_thread_4_1);
		std::thread		t2_4_1(data_processing_thread_4_1);

		t1_4_1.join();
		t2_4_1.join();
	};
	/*
		Когда ожидающий поток повторно получает блокировку мьютекса
		и проверяет условие не в ответ на извещение от другого потока notify_one(),
		а по каким-либо другим причинам,
		это действие называется 'ложным пробуждением' (spurious wake).

		Еще может иметь место ситуация, которая называется "утерянным пробуждением".

		1)	ложное пробуждение		–	ситуация, когда ожидающий поток просыпается,
										хотя никакого оповещения отправитель не присылал;

		2)	утерянное пробуждение	–	ситуация, когда отправитель посылает оповещение до того,
										как поток-получатель переходит в состояние ожидания.

		Наличие "условия" в операции wait() позволяет справиться с обеими проблемами.

		Поскольку количество и частота любых ложных пробуждений не регламентируются по определению,
		задействовать для проверки условия функцию с побочными эффектами не рекомендуется.

		По этой же причине возможности 'ложных пробуждений',
		в функции wait() имеет место дополнительная, уже функциональная проверка.

		**********************************************************************************************
		Параграф 4.1.2. Создание потокобезопасной очереди с условными переменными
		***********************************************************************************************
		Листинг 4.2. Интерфейс std::queue_4_2 "двунаправленная очередь"
	*/
	template <class T, class Container = std::deque<T>>
	class queue_4_2
	{
	public:
		explicit queue_4_2(const Container&) {};
		explicit queue_4_2(Container && = Container()) {};

		// Конструктор перемещения
		queue_4_2(queue_4_2&& q) {};

		template <class Alloc> explicit queue_4_2(const Alloc&) {};

		template <class Alloc> queue_4_2(const Container&, const Alloc&) {};
		template <class Alloc> queue_4_2(Container&&, const Alloc&) {};
		template <class Alloc> queue_4_2(queue_4_2&&, const Alloc&) {};

		queue_4_2& operator=(queue_4_2&& q) {};

		void			swap(queue_4_2&& q) {};

		bool			empty() const {};

		typedef typename std::deque<T>::size_type size_type;
		size_type		size() const {};

		T& front() {};
		const T& front() const {};

		T& back() {};
		const T& back() const {};

		void			push(const T& x) {};
		void			push(T&& x) {};

		void			pop() {};

		//template <class ... Args> void emplace(Args&& … args);
	};
	// Листинг 4.3. Интерфейс потокобезопасной очереди threadsafe_queue будет возвращено текущее время
	// Листинг 4.4. Извлечение push() и wait_and_pop() из листинга 4.1
	// Листинг 4.5. Полное определение потокобезопасной очереди, использующей условные переменные

	// Этот класс является некоторой потокобезопасной оберткой для библиотечной очереди
	template<typename T>
	class threadsafe_queue_4_3
	{
	private:
		// Cобственно классическая библиотечная "однонаправленная" очередь FIFO
		std::queue<T>				data_queue;
		// Мьютекс, защищающий эту очередь
		mutable std::mutex			mut;
		// Условная переменная, сигнализирующая, что данные подготовлены и помещены в очередь
		std::condition_variable		data_cond;
	public:
		threadsafe_queue_4_3() {};
		threadsafe_queue_4_3(threadsafe_queue_4_3 const& other) {
			std::lock_guard<std::mutex>		lk(other.mut);
			data_queue = other.data_queue;
		}
		threadsafe_queue_4_3& operator=(const threadsafe_queue_4_3&) = delete;

		void	 push(T new_value) {
			std::lock_guard<std::mutex>		lk(mut);
			data_queue.push(new_value);
			data_cond.notify_one();
		};

		// try_pop() будет пытаться извлечь значение из очереди, 
		// но всегда тут же возвращать управление(с признаком сбоя), 
		// даже если значения для извлечения не было
		// ???
		bool	try_pop(T& value) {
			std::lock_guard<std::mutex>		lk(mut);
			if (data_queue.empty)
				return false;
			value = data_queue.front();
			data_queue.pop();
			return true;
		}

		std::shared_ptr<T> try_pop() {
			std::lock_guard<std::mutex>		lk(mut);
			if (data_queue.empty())
				return std::shared_ptr<T>();
			std::shared_ptr<T>				result(std::make_shared<T>(data_queue.front()));
			data_queue.pop();
			return result;
		}
		// wait_and_pop() станет ждать, пока не появится значение для извлечения
		void	 wait_and_pop(T& value) {
			if (empty())
				return;
			std::unique_lock<std::mutex>	lk(mut);
			data_cond.wait(lk, [this] { return !data_queue.empty(); });
			value = data_queue.front();
			data_queue.pop();
		};
		std::shared_ptr<T> wait_and_pop() {
			std::unique_lock<std::mutex>	lk(mut);
			data_cond.wait(lk, [this] { return !data_queue.empty(); });
			std::shared_ptr<T>				result(std::make_shared<T>(data_queue.front()));
			data_queue.pop();
			return result;
		}
		bool	empty() const {
			std::lock_guard<std::mutex>		lk(mut);
			return data_queue.empty();
		}
	};

	struct data_chunk_4_4
	{};
	bool			StopFlag{ false };
	bool			LastChunkFlag{ false };
	data_chunk_4_4	prepare_data_4_4() { return data_chunk_4_4(); };
	bool			more_data_to_prepare_4_4() { return StopFlag; };
	void			process_4_4(data_chunk_4_4)
	{
		// Вхождение в спячку на 1000 мс - не работаем, а обозначаем
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	};
	bool			is_last_chunk_4_4(data_chunk_4_4) { return LastChunkFlag; };

	threadsafe_queue_4_3<data_chunk_4_4>	data_queue_4_4;

	void data_preparation_thread_4_4() {
		cout << "\nI'm void data_preparation_thread_4_4() в потоке ID == "
			<< std::this_thread::get_id() << endl;
		while (!more_data_to_prepare_4_4())
		{
			data_chunk_4_4 const data = prepare_data_4_4();
			data_queue_4_4.push(data);
			StopFlag = true;
		}
		cout << "\nI'm void data_preparation_thread_4_4() - конец работы" << endl;
	}

	void data_processing_thread_4_4() {
		cout << "\nI'm void data_processing_thread_4_4() в потоке ID == "
			<< std::this_thread::get_id() << endl;
		while (true)
		{
			data_chunk_4_4		data;
			data_queue_4_4.wait_and_pop(data);
			process_4_4(data);
			if (is_last_chunk_4_4(data))
				break;
			LastChunkFlag = true;
		}
		cout << "\nI'm void data_processing_thread_4_4() - конец работы" << endl;
	}

	void threadsafe_queue_4_3_class_test()
	{
		cout << "\nI'm void threadsafe_queue_4_3_class_test() в потоке ID == "
			<< std::this_thread::get_id() << endl;

		std::thread		data_preparation(data_preparation_thread_4_4);
		std::thread		data_processing(data_processing_thread_4_4);

		data_processing.join();
		data_preparation.join();
	};
	/*
		******************************************************************************************
		Раздел 4.2. Ожидание единичных событий с помощью фьючерсов
		******************************************************************************************
		Параграф 4.2.1.Возвращение значений из фоновых задач
		******************************************************************************************
		Листинг 4.6. Использование std::future для получения значения,
		возвращаемого задачей, выполняемой в асинхронном режиме
	*/
	int find_the_answer_to_ltuae_4_6() {
		cout << "\nI'm find_the_answer_to_ltuae() в потоке ID == "
			<< std::this_thread::get_id() << endl;
		cout << "Посплю пять секунд" << endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		cout << "I'm <find_the_answer_to_ltuae()> Уже проснулась и завершаю работу" << endl;
		/*
			В книге «Автостопом по Галактике» описывалось создание компьютера Deep Thought для
			получения ответа на вопрос о сути Жизни,
			Вселенной и всего остального.
			Ответом стало число 42.
		*/
		return 42;
	}

	void do_other_stuff_4_6() {
		cout << "\nI'm do_other_stuff() в потоке ID == " << std::this_thread::get_id() << endl;
		cout << "----- do_other_stuff() Посплю секунду" << endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cout << "----- do_other_stuff() Уже проснулась и завершаю работу" << endl;
	}
	// Листинг 4.7. Передача аргументов функции с помощью std::async
	/*
		Предположим, что имеются какие-то длительные вычисления find_the_answer_to_ltuae_4_6(),
		которые, как ожидается, дадут со временем полезный результат,
		значение которого пока не интересно

		Шаблон std::async применяется для запуска асинхронной задачи,
		результат решения которой пока не нужен.

		Вместо предоставления объекта ожидания std::thread
		шаблон std::async возвращает объект std::future,
		который в конечном итоге будет содержать значение, возвращаемое функцией.

		std::future<int> the_answer = std::async(find_the_answer_to_ltuae_4_6);

		Пока нам не нужен результат функции find_the_answer_to_ltuae_4_6(),
		которая выполняется в другом потоке асинхронно,
		можно запустить в основном потоке другую функцию,
		которая будет делать другую полезную работу

		do_other_stuff_4_6();

		Теперь мы вспомнили, что нужен результат функции find_the_answer_to_ltuae_4_6().

		Этот результат можно получить, используя метод get() шаблона std::future<>

		cout << "The answer is " << the_answer.get() << endl;

		При этом фоновый поток, возможно, уже завершил работу.

		Если нет, то в этой точке придется подождать его завершения.
	*/
	void future_async_4_6()	{
		cout << "I'm void init_async_4_6()" << endl;
		std::future<int> the_answer = std::async(find_the_answer_to_ltuae_4_6);
		do_other_stuff_4_6();
		std::cout << "The answer is " << the_answer.get() << std::endl;
	}
	struct X_4_7 {
   		void			foo_4_7(int iPar1, std::string const& strPar2) {
			cout << "I'm void foo_4_7(int iPar1, std::string const& strPar2) " 
				 << " iPar1 == " << iPar1 << "; strPar2 == " << strPar2 << endl;
		};
		std::string		bar_4_7(std::string const&) { return std::string{}; };
		int				X_4_7var1{};
		double			X_4_7var2{};
	};
	// Структура - функтор
	struct Y_4_7	{
		double operator()(double par) { return 2 * par; };
	};
	X_4_7&		baz_4_7(X_4_7& par) {
		cout << "I'm X_4_7 baz_4_7(X_4_7& par)" << endl;
		par.X_4_7var1 = 4242;
		return par;
	};
	class move_only_4_7 {
	public:
		// Конструктор
		move_only_4_7() {};

		// Конструктор перемещения
		move_only_4_7(move_only_4_7&&) noexcept { };
	
		// Конструктора копирования быть не должно
		move_only_4_7(move_only_4_7 const&) = delete;
		
		// Опреатор присваивания перемещением
		move_only_4_7& operator=(move_only_4_7&&)  noexcept { return *this; };
		
		// Опреатора присваивания копирование  быть не должно
		move_only_4_7& operator=(move_only_4_7 const&) = delete;
		
		// Функтор - перегружен оператор вызова
		void operator()() {
			cout << "\nI'm void operator() of class move_only_4_7 в потоке ID == "
				<< std::this_thread::get_id() << endl;
		};
	};
	void init_async_4_7() 	{
		cout << "I'm void init_async_4_7()" << endl;

		X_4_7		x_4_7;
		// Вызов ptr->foo_4_7(42,"hello"), где рtr - это &x_4_7
		(&x_4_7)->foo_4_7(42, "Hello, Solar Wind! Я и не думал, что так можно...");
		/*
			Если первым аргументом является указатель на компонентную функцию, 
			второй аргумент предоставляет объект, 
			к которому применяется компонентная функция 
			(либо напрямую, либо через указатель, либо заключенным в оболочку std::ref), 
			а остальные аргументы передаются компонентной функции 
			в качестве ее простых аргументов. 
		*/
		auto		f1_4_7	= std::async	(		&X_4_7::foo_4_7
												,	&x_4_7
												,	542
												,	"Hello, Solar Wind! It's Normal"
											);
		// Вызов tmpx.bar_4_7("goodbye"), где tmpx - это копия x_4_7
		auto		f2_4_7	= std::async(&X_4_7::bar_4_7, x_4_7, "Goodbye, Baby!");

		// Вызов временного объекта tmp(), где tmp создается из std::move(move_only())
		// Так как конструктор копирования объявлен удаленным, то был вызван конструктор перемещения
		auto		f5_4_7	= std::async((move_only_4_7()));
		
		// Запуск в новом потоке асинхронно
		auto		f66_4_7	= std::async(std::launch::async, Y_4_7(), 1.2);	

		// Запуск в wait() или get() - а здесь откладвается
		// до того момента, когда будет вызвана функция-член wait() или get() объекта - будущего;
		// а здесь копия объекта не создается, так как передается ссылка на него в обертке std::ref()
		auto		f7_4_7	= std::async(std::launch::deferred, baz_4_7, std::ref(x_4_7));
		// !!! А ТАК НЕ КАТИТ!!! auto	f7_4_7 = std::async(std::launch::deferred, baz_4_7, ((x_4_7)));
		
		// Вызов отложенной функции по усмотрению реализации
		auto		f8_4_7	= std::async	(		std::launch::deferred | std::launch::async
												,	baz_4_7, std::ref(x_4_7)
											);
		// Вызов отложенной функции по усмотрению реализации
		auto		f9_4_7	= std::async(baz_4_7, std::ref(x_4_7));

		Y_4_7		y_4_7;
		// Вызов y_4_7(3.141)
		auto		f3_4_7	= std::async((Y_4_7()), 3.141);

		// Вызов y_4_7(2.718)
		auto		f4_4_7	= std::async(std::ref(y_4_7), 2.718);

		auto		f6_4_7	= std::async(baz_4_7, std::ref(x_4_7));

		auto		var1	= f66_4_7.get();
		cout << "auto var1  = f66_4_7.get(); == " << var1 << endl;

		// Вызов отложенной параметром std::launch::deferred функции - дополнительная гибкость - 
		// иначе придется ее вызывать методом get() и потом ждать, пока она отработает
		f7_4_7.wait();
		// Вызов auto var2 = f7_4_7.get(); не приведет к желаемому результату, 
		// так как auto отбрасывает ссылку
		// Поэтому явно укажем тип возвращаемой величины
		// X_4_7&		var2	= f7_4_7.get();
		// А можно и так:
		// auto	var2 = (f7_4_7.get());		// Компенсируем отброшенную ссылку
		// А можно и так:
		decltype(auto)	var2 = f7_4_7.get();
		cout << "X_4_7&  var2 = f7_4_7.get(); == " << var2.X_4_7var1 << endl;

		var2.X_4_7var1 = 42;
		cout << "После присваивания X_4_7::X_4_7var1 == " << x_4_7.X_4_7var1 << endl;
	};
	/*
		********************************************************************************************
		Связать std::future<> с задачей можно также заключением задачи 
		в экземпляр шаблона класса std::packaged_task<> 
		или созданием кода для явной установки значений с помощью шаблона класса std::promise<>.

		Шаблон класса std::packaged_task<> является абстракцией более высокого уровня, 
		чем std::promise<>.

		Параграф 4.2.2. Связывание задачи с фьючерсом
		*********************************************************************************************
		Шаблон класса std::packaged_task<> привязывает фьючерс к функции или вызываемому объекту.

		Когда вызывается объект std::packaged_task<>, он вызывает связанную функцию или объект
		и приводит фьючерс в состояние готовности с возвращаемым значением, 
		сохраненным в качестве 'связанных данных'.
	
		Если крупную операцию возможно разбить на автономные подзадачи,
		каждую из подзадач можно заключить в экземпляр std::packaged_task<>,
		который затем будет передан диспетчеру задач или пулу потоков.
	
		Тем самым удается абстрагироваться от подробностей задач -
		диспетчер имеет дело только с экземплярами std::packaged_task<>, 
		а не с отдельно взятыми функциями.
	
		Параметром шаблона для шаблона класса std::packaged_task<> является 'сигнатура функции'.

		При создании экземпляра std::packaged_task,
		ему следует передать функцию или вызываемый объект,
		принимающий указанные параметры, а затем возвращающий тип, 
		который можно преобразовать в указанный тип возвращаемого значения.
	
		Тип возвращаемого значения, указанный в сигнатуре функции, 
		определяет тип объекта std::future<>,
		возвращаемого из компонентной функции get_future(), 
		а заданный в сигнатуре 'список аргументов' используется
		для определения сигнатуры оператора вызова в классе упакованной задачи.
		**********************************************************************************************
		Листинг 4.8. Частичное определение класса для специализации std::packaged_task< >
	*/
//  ------------------------------------------------------------------------------------------------
};	// End of the namespace 'ns_AW_chapter_4'
//  ------------------------------------------------------------------------------------------------
// Так как мы работаем в собственном пространстве имен, для определения класса для специализации
// необходимо перейти в пространство имен библиотеки std
// Вообще то это театр, чтобы показать, как примерно устрена нектороая специализация шаблона
/*
namespace std
{
//  ------------------------------------------------------------------------------------------------
	template<>
	class packaged_task<string(vector<char>*, int)>
	{
	public:
		template<typename Callable>
		explicit packaged_task(Callable&& f) {};

		future<string> get_future();

		void operator()(vector<char>*, int) {};
	};
//  ------------------------------------------------------------------------------------------------
};	// End of the namespace 'STD'
*/
//  ------------------------------------------------------------------------------------------------
// Bозвращаемся в родное пространство имен
namespace ns_AW_chapter_4
{
//  ------------------------------------------------------------------------------------------------
	/*
		Таким образом, std::packaged_task – допускающий вызов объект, 
		и, значит, его можно:
		
		-	обернуть объектом std::function, 

		-	передать std::thread в качестве функции потока, 

		-	передать любой другой функции, которая ожидает допускающий вызов объект, 
		
		или даже 
		
		-	вызвать напрямую. 
		
		Если std::packaged_task вызывается как объект-функция, 
		то аргументы, переданные оператору вызова, 
		без изменения передаются обернутой им функции, 
		а возвращенное значение 
		сохраняется в виде асинхронного результата в объекте std::future, 
		полученном от get_future(). 
		
		Следовательно, мы можем обернуть задачу в std::packaged_task 
		и извлечь будущий результат перед тем, 
		как передавать объект std::packaged_task в то место, 
		из которого он будет в свое время вызван. 
		
		Когда результат понадобится, нужно будет подождать готовности будущего результата.
*/
	double	AnyWorkingFunction(int intPar1, double dblPar2) {
		cout << "I'm void AnyWorkingFunction(int intPar1, double dblPar2) в потоке "
			 << std::this_thread::get_id() << endl;
		return	dblPar2 * 1000.0 + double(intPar1) * 10.42;
	}
	void	SimplePackagedTaskExample()
	{
		cout << "I'm void SimplePackagedTaskExample() в потоке "
			 << std::this_thread::get_id() << endl;
		
		// Упакуем выполняемую функцию в задачу с указанной сигнатурой
		std::packaged_task<double(int, double)>		packed_task(AnyWorkingFunction);
		
		// Свяжем упакованную задачу с фьючерсом - будущим результатом
		std::future<double>		future_result	=	packed_task.get_future();
		
		// Запустим функцию в этом же потоке
		packed_task(1, 3.14);
		future_result.wait();				// Так можно делать, можно не делать
		cout << "I'm void SimplePackagedTaskExample() - результат == " << future_result .get() << endl;
		
		/*
		// Еще возможен и такой вариант:
		
		// Свяжем упакованную задачу с  фьючерсом - будущим результатом
		std::future<double>		future_result		=	packed_task.get_future();

		// Запустим функцию в другом потоке параллельно
		std::thread				packed_task_thread(std::move(packed_task), 1, 2.73);
		packed_task_thread .join();
		
		future_result .wait();				// Так можно делать, можно не делать
		cout << "I'm void SimplePackagedTaskExample() - результат == " << future_result .get() << endl;
		*/
	};
	/*
		----------------------- Пункт 4.2.2.1. Передача задач между потоками
		 Во многих каркасах для разработки пользовательского интерфейса требуется, 
		 чтобы интерфейс обновлялся только в специально выделенных потоках. 
		 
		 Если какому-то другому потоку потребуется обновить интерфейс, 
		 то он должен послать сообщение одному из таких выделенных потоков, 
		 чтобы тот выполнил операцию. 
		 
		 Шаблон std::packaged_task позволяет решить эту задачу, 
		 не заводя специальных сообщений 
		 для каждой относящейся к пользовательскому интерфейсу операции.

		Листинг 4.9. Запуск кода в GUI-потоке с помощью std::packaged_task
		
		GUI	-	Graphic User Interface	-	графический интрефейс пользователя
	*/
	std::mutex								mtx_4_9;
	// Очередь упакованных задач - функций с сигнатурами void()
	std::deque<std::packaged_task<int()>>	tasks_4_9;			
	// Флаг завершения работы диспетчера задач
	bool									cond_for_cycle_4_9;	

	bool gui_shutdown_message_received_4_9()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		cout << "I'm void  gui_shutdown_message_received_4_9() в потоке " 
				<< std::this_thread::get_id() 
				<< " cond_for_cycle_4_9 == " << std::boolalpha << cond_for_cycle_4_9
				<< endl;

		return cond_for_cycle_4_9;
	};

	void get_and_process_gui_message_4_9()
	{
		cout << "I'm get_and_process_gui_message_4_9() в потоке " 
			 << std::this_thread::get_id() << endl;
	};

	// Поток GUI - void gui_thread_4_9() - выполняет цикл, 
	// пока не будет получено сообщение, предписывающее GUI прекратить работу	(2), 
	// многократно проводя опрос на наличие GUI - сообщений на обработку		(3), 
	// например, щелчков кнопкой мыши, и на наличие задач в очереди.
	// 
	// Если в очереди нет задач													(4), 
	// цикл повторяется, 
	// в противном случае задача извлекается из очереди							(5), 
	// с очереди снимается блокировка, после чего запускается задача			(6).
	// 
	// Когда задача будет выполнена, фьючерс, связанный с ней, перейдет в состояние готовности.
	void gui_thread_4_9()
	{
		cout << "I'm void gui_thread_4_9() for GUI в потоке " << std::this_thread::get_id() << endl;
		
		std::this_thread::sleep_for(std::chrono::milliseconds(300));

		while (!gui_shutdown_message_received_4_9())							// 2
		{
			cout << "I'm cycle of the void gui_thread_4_9()" << endl;

			get_and_process_gui_message_4_9();									// 3

			std::packaged_task<int()> task;			// Формируем экземпляр шаблона для последующего
													// связываения с вызываемой (связанной) функцией
													// с сигнатурой void(), 
													// которую мы только что указали
													// в параметре шаблона
			{
				std::lock_guard<std::mutex>		lk(mtx_4_9);
				if ( tasks_4_9.empty() )			//								4 
													// - проверяем, пустая или нет очередь задач
					continue;
				task = std::move(tasks_4_9.front());//								5 
													// - извлекаем из очереди очередную 
													// упакованную задачу,
				tasks_4_9.pop_front();				// используя семантику перемещения
			}
			// Запуск на выполнение (вызов "напрямую") очередной задачи, вынутой из очереди
			//task();																// 6 
			//										- запуск в этом же потоке
			// 
			// Можно и запустить в другом, отдельном потоке
			std::thread		thread_for_task(std::move(task)); 
			thread_for_task.join();

			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}	// End of while
	};
	// Помещение задачи в очередь: предоставленная функция создает новую упакованную задачу		(7), 
	// вызовом компонентной функции get_future() из этой задачи получается фьючерс				(8) 
	// и задача помещается в список																(9)
	// перед тем, как фьючерс возвращается вызывающему коду										(10).
	// Затем код, отправивший сообщение потоку GUI, может ожидать фьючерс, если ему требуется узнать
	// о завершении задачи, или же проигнорировать фьючерс, если эта информация	не нужна.
	template<typename Func>
	std::future<int>	post_task_for_gui_thread_4_9(Func f)
	{
		cout << "\nI'm void post_task_for_gui_thread_4_9() for GUI в потоке " 
			 << std::this_thread::get_id() << endl;

		std::packaged_task<int()> task(f);			// 7	-	создаем новую упакованную задачу
		std::future<int> result = task.get_future();// 8	-	создаем фьючерс 
													//			вызовом компонентной функции
													//			get_future() из этой задачи - 
													//			еще до ее вызова диспетчером
		std::lock_guard<std::mutex>		lk(mtx_4_9);
		tasks_4_9.push_back(std::move(task));		// 9	-	помещаем созданную упакованную задачу 
													//			в очередь задач
		return result;								// 10	-	возвращаем фьючерс как резульат 
	}
	// Функции, которые будут упакованы в задачи std::packaged_task<> 
	// и помещены в очередь задач на выполнение
	// диспетчер задач в потоке void gui_thread_4_9() 
	// будет просматривать очередь и запускать последовательно
	// все находящиеся в ней задачи
	int func_1_4_9() { 
		cout << "I'm void func_1_4_9() for GUI в потоке " << std::this_thread::get_id() << endl; 
		return 149;
	};
	int func_2_4_9() { 
		cout << "I'm void func_2_4_9() for GUI в потоке " << std::this_thread::get_id() << endl; 
		return 249;
	};
	int func_3_4_9() { 
		cout << "I'm void func_3_4_9() for GUI в потоке " << std::this_thread::get_id() << endl; 
		return 349;
	};

	void init_listing_4_9()
	{
		cout << "I'm init_listing_4_9() - начинаю работу в потоке " 
			 << std::this_thread::get_id() << endl;

		cond_for_cycle_4_9 = false;			// Разрешаем работать в цикле диспетчеру задач

		// Запускаем поток с диспетчером задач, в цикле проверяющим очередь задач
		// и запускающий находящиеся в очереди упакованные задачи на выполнение.
		std::thread		gui_bg_thread_4_9(gui_thread_4_9);

		// Передаем функции на упаковку и помещение в очередь задач
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		// Тип result_1 - фьючерс std::future<int>
		auto result_1 = post_task_for_gui_thread_4_9(func_1_4_9);	

		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		// Тип result_2 - фьючерс std::future<int>
		auto result_2 = post_task_for_gui_thread_4_9(func_2_4_9);	

		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		// Тип result_3 - фьючерс std::future<int>
		auto result_3 = post_task_for_gui_thread_4_9(func_3_4_9);	

		cout << "I'm init_listing_4_9() Посплю 200 миллисекунд" << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		cond_for_cycle_4_9 = true;	// Запрещаем работать в цикле диспетчеру задач

		gui_bg_thread_4_9.join();

		// Теперь выведем на консоль результаты работы задач-функций
		cout	<< "   1-я задача: " << result_1.get()
				<< "   2-я задача: " << result_2.get()
				<< "   3-я задача: " << result_3.get()
				<< endl;
	};
	/*
		А как быть с такими задачами, которые нельзя выразить в виде простого вызова функции, 
		или с теми, где результат может приходить из более чем одного места? 

		В таких случаях приходится обращаться к третьему способу создания фьючерса — 
		к использованию std::promise для явного задания значения.

		*******************************************************************************************
		Параграф 4.2.3.Создание промисов std::promise
		*******************************************************************************************
	*/
	double	ProductTwoItems	(		std::promise<double>	promisParam
								,	double					dblParam1
								,	double					dblParam2
							)
	{
		cout << "I'm double	ProductTwoItems"
				"(std::promise promiseParam, double dblParam1, double dblParam2) в потоке " 
			 << std::this_thread::get_id() << endl;

		double result	= dblParam1 * dblParam2;

		// Организуем канал передачи данных и сразу по нему передаем информацию
		promisParam.set_value(result);				// Notify future
		
		return	result;
	}
	void	SimplePromisFutureExample()
	{
		cout << "I'm void SimplePromisFutureExample() в потоке "
			 << std::this_thread::get_id() << endl;
		double		dblVar1 = 3.14;
		double		dblVar2 = 2.73;

		std::promise<double>	double_product_promise;
		// Привязываем к промису будущий результат - точку канала передачи информации с этой стороны
		std::future<double>		double_product_future	= double_product_promise.get_future();
		// Промис в прогу надо передать обязательно - иначе канал не организовать
		std::thread				product_thread	(		ProductTwoItems
													,	std::move(double_product_promise)
													,	dblVar1
													,	dblVar2
												);
		// Можно делать, можно не делать - зависит от алгоритма проги
		double_product_future.wait();
		// Наверное присоединение нужно перенести ниже инструкции получения отложенного резульатат
		// Вдруг функция потока выполнит в какой-то точке promise.set_value(result);
		// и после этого ей нужно будет еще что-то делать. 
		// А мы будет только зря ждать.
		// product_thread	.join();

		cout << "result == " << double_product_future.get() << endl;
		cout << "Результат от промиса получен, теперь можно и присоединить поток..." << endl;
		product_thread .join();
	};
	// *************************** Еще несколько примеров о паре promise - futures
	// Это предварительное объявление - определение функции - ниже в этом же заголовочном файле
	double square_root_4_2_4(double x);

	void	promise_futures_4_2_3() {
		cout << "\nI'm promise_futures_4_2_3() в потоке ID == " << std::this_thread::get_id() << endl;

		std::promise<const char*>		prms_4_2_3;				// Создадим обещание...
		// и связанный с ним фьючерс
		std::future<const char*>		ftrs_4_2_3	= prms_4_2_3.get_future();

		cout << "\n Напоминаем, ID главного потока == " << std::this_thread::get_id() << endl;

		// Функция потока - в нее должна попасть ссылка на созданный промис (обещание)
		auto	thread_func_4_2_3	=	[&prms_4_2_3]() {
						cout << "\n Я лямбда потока с ID == " << std::this_thread::get_id() << endl;
						// Здесь промис устанавливает строковое значение и тем самым
						// приводит фьючерс в состояние готовности
						prms_4_2_3.set_value("С наступающим Новым Годом!");
			};
		// Запустить поток
		std::thread		thrd_4_2_3(thread_func_4_2_3);

		// Ожидать завершения потока и получить результат из фьючерса
		string	str_4_2_3 = ftrs_4_2_3.get();
		cout << "Результат - поздравление: " << str_4_2_3 << endl;

		thrd_4_2_3.join();
	};

	void	promise_futures_4_2_4() {
		cout << "\nI'm promise_futures_4_2_4() в потоке ID == " << std::this_thread::get_id() << endl;

		double	y_4_2_4 = 3.14;
		try {
			// Работа функции в главном потоке
			square_root_4_2_4(16.0);
			// а теперь во вспомогательном

			std::future<double>		f_4_2_4		= std::async(square_root_4_2_4, 1000.314);
			y_4_2_4 = f_4_2_4.get();
			cout << "Результат извлечения корня из положительного числа : " << y_4_2_4 << endl;
			f_4_2_4 = std::async(square_root_4_2_4, -1000.314);
			y_4_2_4 = f_4_2_4.get();
			cout << "Результат  извлечения корня из отрицательного числа: " << y_4_2_4 << endl;
		}
		catch (const	std::out_of_range& e) {
			cout << e.what();
		}
		catch (...) {
			cout << "Не угадал тип исключения :(" << endl;
		}
		cout << y_4_2_4 << endl;

		//init_listing_4_2_4();
		//promis_futures_master_4_2_4();
	};

	void	promise_futures_4_2_5() {
		cout << "\nI'm promise_futures_4_2_5() в потоке ID == " << std::this_thread::get_id() << endl;
		/*
			Экземпляры std::shared_future, ссылающиеся на некое асинхронное состояние,
			составлены из экземпляров std::future,
			ссылающихся на это состояние.

			Поскольку объекты std::future не владеют асинхронным состоянием
			совместно с любым другим объектом,
			право собственности должно быть передано в std::shared_future с помощью std::move(),
			оставляя std::future в пустом состоянии,
			как будто этот объект является конструктором по умолчанию:
		*/
		std::promise<std::map<int, std::string>::iterator>		prms_map_4_2_5;	// Создаем промис
		// Создаем разделяемый фьючерс, привязанный к промису
		auto						sh_ftrs_map_4_2_5		= prms_map_4_2_5.get_future().share();

		std::promise<int>			prms_4_2_5;							// Создаем промис
		// Создаем фьючерс, привязанный к промису
		std::future<int>			ftrs_4_2_5(prms_4_2_5.get_future());
		// Фьючерс ftrs_4_2_5 является допустимым
		assert(ftrs_4_2_5.valid() && "Фьючерс ftrs_4_2_5 не прошел валидацию! :(");
		
		std::shared_future<int>		sh_ftrs_4_2_5(std::move(ftrs_4_2_5));
		
		// ftrs_4_2_5 больше не является допустимым, так как был только что перемещен
		//assert(ftrs_4_2_5.valid() && "Фьючерс ftrs_4_2_5 не прошел валидацию - был перемещен! :(");
		
		// Зато sh_ftrs_4_2_5 является допустимым
		assert(sh_ftrs_4_2_5.valid() && "Фьючерс sh_ftrs_4_2_5 не прошел валидацию! :(");
		/*
			Как и для других перемещаемых объектов, передача владения для r-значений
			происходит подразумеваемо, поэтому std::shared_future можно создавать
			непосредственно из возвращаемого значения компонентной функции get_future()
			объекта std::promise, например:

			std::promise<std::string>			prms_str_4_2_5;
			std::shared_future<std::string>		sh_ftrs_str_4_2_5(prms_str_4_2_5.get_future());
			// Подразумеваемый перенос владения
			assert(sh_ftrs_str_4_2_5.valid());

			В std::future есть компонентная функция share(), 
			создающая новый экземпляр std::shared_future
			и непосредственно ему передающая владение асинхронным состоянием.
			
			Это обстоятельство может сэкономить массу времени при наборе кода
			и упростить внесение в него изменений:
		*/
	};
	//	*******************************************************************************

	// Листинг 4.10. Обработка с помощью промисов сразу нескольких подключений в одном потоке

	/*
		B приложениях с большим количеством сетевых подключений для их обработки 
		принято обходиться небольшим количеством потоков (возможно, только одним потоком), 
		каждый из которых имеет дело с несколькими подключениями.
		
		Шаблон std::promise<T> позволяет устанавливать значение (типа T), которое
		позже можно прочитать через связанный объект std::future<T>. 
		
		Один из возможных механизмов такого рода 
		будет реализован парой std::promise - std::future,
		ожидающий поток может заблокироваться на фьючерсе, 
		а поток, предоставляющий данные, - воспользоваться другой половиной пары, 
		промисом (promise, иногда называют 'обещанием'),
		для установки связанного значения и приведения фьючерса в 'состояние готовности'.
		
		Получить объект фьючерса std::future, связанный с заданным промисом
		std::promise, можно вызовом компонентной функции get_future(), 
		как это было с std::packaged_task. 
		
		Когда значение промиса установлено (с помощью компонентной функции set_value()), 
		фьючерс приводится в состояние готовности 
		и может использоваться для извлечения сохраненного значения. 
		
		Если объект std::promise уничтожить без установки значения, 
		вместо него будет сохранено исключение. 

		**********************************************************************************************
		Листинг 4.10. Обработка с помощью 'промисов' сразу нескольких подключений в одном потоке
	*/
	struct incoming_data_packet_4_10	{
		typedef std::string		payload_type;
		typedef unsigned		id_type;
		// В конструкторе по умолчанию инициализация членов производится по умолчанию
		incoming_data_packet_4_10() :	payload	(payload_type{})
									,	id		(id_type{})
									,	promise	(std::promise<payload_type>{}) 
		{
			futures = promise.get_future();
		};
		// Полезная нагрузка входящего пакета (это и дословный перевод с английского)
		payload_type				payload;		// Полезная нагрузка		
		id_type						id;				// Идентификатора входящего пакета
		// Узлы канала обмена данными между потоками (или между задачами)
		std::promise<payload_type>	promise;		// promise.set_value();
		std::future<payload_type>	futures;		// futures = promise.get_future();
		/*
		~incoming_data_packet_4_10()
		{
			cout << "I'm DSTR of the <struct incoming_data_packet_4_10>" << endl;
		}*/
	};
	struct outgoing_data_packet_4_10
	{
		typedef std::string		payload_type;
		typedef unsigned		id_type;

		outgoing_data_packet_4_10() :	payload	(payload_type{})
									,	id		(id_type{})
									,	promise	(std::promise<bool>{}) 
		{
			futures = promise.get_future();
		};
		// Полезная нагрузка исходящего пакета (это и дословный перевод с английского)
		payload_type		payload;			
		id_type				id;					// Идентификатора исходящего пакета

		std::promise<bool>	promise;
		std::future<bool>	futures;			// futures = promise.get_future();
		/*
		~outgoing_data_packet_4_10()
		{
			cout << "I'm DSTR of the <outgoing_data_packet_4_10>" << endl;
		}*/
	};

	class connection_4_10
	{
	public:
		incoming_data_packet_4_10	incoming()
		{
			incoming_data_packet_4_10		result;
			//return std::move(result);
			return result;
		}
		outgoing_data_packet_4_10  top_of_outgoing_queue()
		{
			outgoing_data_packet_4_10	result;
			//return std::move(result);
			return result;
		}

		std::promise<incoming_data_packet_4_10::payload_type>	
				get_promise(incoming_data_packet_4_10::id_type& par)
		{
			std::promise<incoming_data_packet_4_10::payload_type>	result;
			//return std::move(result);
			return result;
		}
		void	send(outgoing_data_packet_4_10::payload_type&) {}

		bool	has_incoming_data() { return true; }
		bool	has_outgoing_data() { return true; }
	};

	class connection_set_4_10 : public std::vector<connection_4_10>
	{
	public:
		connection_set_4_10() : done_connections(true)	{}

		void operator()()								{}
		
		bool done_connections;
	};

	typedef  connection_set_4_10::iterator			connection_iterator_4_10;

	bool	done_4_10(connection_set_4_10& connections) 
	{ 
		return connections.done_connections; 
	};
	/*
		Функция process_connections() выполняет цикл, 
		пока функция done() не возвратит значение true. 
		
		При каждом проходе цикла поочередно проверяется каждое подключения, 
		извлекаются входящие данные, если они есть, 
		или отправляются любые стоящие в очереди исходящие данные. 
		
		Предполагается, что у пакета входящих данных есть идентификатор, 
		а у него — полезная нагрузка. 
	*/
	void	process_connections_4_10(connection_set_4_10&	connections)
	{
		cout << "I'm process_connections_4_10() в потоке ID == " << std::this_thread::get_id() << endl;

		// 1 - Функция process_connections() выполняет цикл, 
		// пока функция done() не возвратит значение true
		while (!done_4_10(connections))
		{
			// 2 - При каждом проходе цикла поочередно проверяется каждое подключение
			for (connection_iterator_4_10	connection =  connections.begin(), end = connections.end();
											connection != end;
											++connection)
			{
				// 3 - извлекаются входящие данные, если они есть
				if (connection->has_incoming_data())
				{
					incoming_data_packet_4_10	data_in = connection->incoming();
					// 4 -	Предполагается, 
					// что у пакета входящих данных есть идентификатор и полезная нагрузка.
					// Идентификатор отображается на объект std::promise 
					// (возможно, поиском в ассоциативном контейнере), 
					std::promise<incoming_data_packet_4_10::payload_type>	
										prms = connection->get_promise(data_in.id);
					// а в качестве значения устанавливается полезная (информационная) нагрузка пакета.
					prms.set_value(data_in.payload);
				};
				// 5  - отправляются любые стоящие в очереди исходящие данные
				if (connection->has_outgoing_data())
				{
					// Исходящий пакет извлекается из очереди исходящих данных  	
					outgoing_data_packet_4_10	data_out = connection->top_of_outgoing_queue();
					// и отправляется по подключению в сеть.
					connection->send(data_out.payload);
					// 6 - Когда отправка завершится, промис, 
					// связанный с исходящими данными, устанавливается равным true, 
					// что говорит об успешной передаче.
					data_out.promise.set_value(true);
				};
			}
			//cout << "I'm cycle of <process_connections_4_10()> Посплю 20 миллисекунд" << endl;
			//std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	};

	connection_set_4_10							connections;
	// Очередь входящих пакетов данных
	std::queue<incoming_data_packet_4_10>		incoming_queue_4_10;
	// Очередь исходящих пакетов данных
	std::queue<outgoing_data_packet_4_10>		outcoming_queue_4_10;

	void listen_incom()
	{
		cout << "I'm void listen_incom() в потоке ID == " << std::this_thread::get_id() << endl;

		for (int i = 0; i != 10; ++i)
		{
			incoming_data_packet_4_10	incom_packet;
			// Нужна промежуточная переменная, так как объект incom_packet ща будет перемещен!!!     
			auto	inc_pack_payload = incom_packet.payload + std::string("  Sergej Heckel");
			// Здесь и далее необходимо перемещение, так как в объекте класса содержится переменная
			// типа std::promise<bool>, а ее нельзя копировать
			incoming_queue_4_10.push(std::move(incom_packet));

			outgoing_data_packet_4_10		outgoing_packet;
			outgoing_packet.payload		= inc_pack_payload;
			outcoming_queue_4_10.push(std::move(outgoing_packet));
			// Здесь и далее необходимо перемещение, так как в объекте класса содержится переменная
			// типа std::promise<bool>, а ее нельзя копировать
			connection_4_10			connect;
			connections.push_back(std::move(connect));
		};
		/*
		while (!outcoming_queue_4_10.empty()) {
			cout << std::move(outcoming_queue_4_10.front()).payload << " ";
			outcoming_queue_4_10.pop();
		}
		cout << endl;
		*/
	};
	void master_connect_4_10()
	{
		cout << "I'm master_connect_4_10() в потоке ID == " << std::this_thread::get_id() << endl;

		std::thread		thrd_lstn(listen_incom);
		connections .done_connections = false;

		std::thread		thrd(process_connections_4_10, ref(connections));

		cout << "-------------- master_connect_4_10() Посплю 2 секунды" << endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));

		connections .done_connections = true;
		
		thrd_lstn	.join();
		thrd		.join();
	};
	/*
		****************************************************************
		std::promise<>
		****************************************************************
	*/
	struct calc_functor;
	struct print_functor;
	std::mutex						clc_prn_mtx;
	std::vector<calc_functor>		clc_fnctrs;		 // вектор функтуров расчета
	std::vector<double>				results(10, 0.0);// вектор результатов вычисления calc_functor
	std::vector<print_functor>		prn_fnctrs;		 // Вектор функтуров распечатки расчета значения

	std::vector<std::promise<int>>	prms_vect;		 // вектор промисов, передаваемых в calc_functor
	std::vector<std::future<int>>	ftrs_vect;		 // вектор фьючерсов, связанных с помисами
													 // передаваемыми в calc_functor
	
	// Функтор расчитывает некоторое значениие как функцию от индекса index = 0, 1, 2, ... 9
	// и помещает расчитанное значение в вектор результатов
	struct calc_functor	{
		int		c_p_index;

		void operator()(std::promise<int>& prms)
		{
			cout << "I'm functor расчета calc_functor" << endl;

			double result = 3.14 * 1000 * c_p_index / 1956.0;
			
			results[c_p_index] = result;
			// Здесь промис устанавливает свое шаблонное значение и тем самым
			// приводит связанный фьючерс в состояние готовности
			prms.set_value(c_p_index);
		}
	};
	
	// Функтор распечатки результатов расчета функтора calc_functor
	struct print_functor	
	{
		void operator()(std::future<int>&	ftrs)
		{
			cout << "I'm functor печати print_functor " << endl;

			int		p_f_index	= ftrs.get();

			cout << "index	= ftrs.get() == " << p_f_index  
				 << ";  results[p_f_index] ==  "  << results[p_f_index] << endl;
		}
	};
	// Функция инициации функтуров расчета
	void calc_functors_init()
	{
		cout << "I'm void calc_functors_init() потока с ID == " << std::this_thread::get_id() << endl;
		// Запускаем функторы из вектора функтуров расчета
		cout << "---------------------- I'm cycle  calc_functor" << endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		for (int index = 0; index != 10; ++index)
		{
			calc_functor		clc_fnctr{};	// Создаем функтор расчета - пустой
			clc_fnctr.c_p_index = index;		// Устанавливаем у него индекс результата расчета
			// Теперь готовим для него соответствующий промис с таким же индексом
			clc_fnctrs[index].c_p_index = index;

			std::promise<int>						prms = std::move(prms_vect[index]);

			std::function<void(std::promise<int>&)>	functor_for_calc = clc_fnctr;
			std::lock_guard		lk(clc_prn_mtx);
			functor_for_calc(prms);
		};
	};
	// 
	// Функция инициации функтуров печати результатов расчета
	void prnt_functors_init()
	{	
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cout << "I'm void prnt_functors_init() потока с ID == " << std::this_thread::get_id() << endl;
		// Запускаем функторы из вектора функторов, 
		// чтобы инициировать печать соответствующего результата
		cout << "---------------------- I'm cycle  print_functor" << endl;
		for (int index = 0; index != 10; ++index)
		{
			print_functor							prnt_ftrs;	// Создаем функтор печати
			std::future<int>						ftrs = std::move(ftrs_vect[index]);
			std::function<void(std::future<int>&)>	functor_for_prn = prnt_ftrs;
			functor_for_prn(ftrs);	// Запуск функтора печати результата, соответствующего индексу 
		};
	};

	void promis_futures_master()
	{
		cout << "I'm void promis_futures_master() потока с ID == " 
			 << std::this_thread::get_id() << endl;
		// Создадим векторы функтуров расчета и распечатки
		for (int index = 0; index != 10; ++index)
		{
			clc_fnctrs.push_back(std::move(calc_functor()));
			prn_fnctrs.push_back(std::move(print_functor()));
		};
		// Создаем вектор промисов, которые будут по очереди передаваться в функторы расчета 
		for (int index = 0; index != 10; ++index)
		{
			prms_vect.push_back(std::move(std::promise<int>()));
		};
		// Создаем вектор фьючерсов, связанных с только что создаными промисами функторов расчета
		for (int index = 0; index != 10; ++index)
		{
			ftrs_vect.push_back(prms_vect[index] .get_future());
		};
		// Запускаем поток инициализации функторов расчета из вектора функторов расчета 
		std::thread calc_thrd(calc_functors_init);
		// Запускаем поток инициализации функторов печати из вектора функторов печати
		std::thread prnt_thrd(prnt_functors_init);
		
		prnt_thrd.join();
		calc_thrd.join();
	};
	/*
		*******************************************************************************************
		Параграф 4.2.4. Сохранение исключения на будущее
		******************************************************************************************
		std::thread
	*/
	double square_root_4_2_4(double x)
	{
		cout << "\nI'm double square_root_4_2_4(double) в потоке с ID == " 
			 << std::this_thread::get_id() << endl;
		if(x < 0)
		{
			throw std::out_of_range("Операнд отрицательный :( x<0 - повнимательней, Брат! :)\n");
		}
		return std::sqrt(x);
	};

	/*
		******************************************************************************
		Исключения с std::packaged_task<>
		******************************************************************************
	*/
	// Aдаптированный листинг 4.9

	std::mutex									mtx_4_2_4;
	// Очередь функций, заключенных в std::packaged_task<void()>
	std::deque<std::packaged_task<void()> >		tasks_4_2_4;				 

	// Условная переменная для синхронизации
	bool										cond_for_cycle_4_2_4;
	
	// Функция Возвращает глобальную условную переменную, которую диспетчер изменяет так или иначе
	bool	gui_shutdown_message_received_4_2_4()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		cout << "----------- I'm void  gui_shutdown_message_received_4_2_4() в потоке ID == "
			 << std::this_thread::get_id()
			 << " cond_for_cycle_4_2_4 == " << std::boolalpha << cond_for_cycle_4_2_4
			 << " ------------------ "
			 << endl;

		return	cond_for_cycle_4_2_4;
	};
	void get_and_process_gui_message_4_2_4()
	{
		cout << "********** I'm void  get_and_process_gui_message_4_2_4() в потоке ID == " 
			 << std::this_thread::get_id() << endl;
	};
	// Поток GUI О выполняет цикл, пока не будет получено сообщение, 
	// предписывающее GUI прекратить работу									(7), 
	// многократно проводя опрос на наличие GUI - сообщений на обработку	(3), 
	// например, щелчков кнопкой мыши, и на наличие задач в очереди.
	// 
	// Если в очереди нет задач												(4), 
	// цикл повторяется, 
	// в противном случае задача извлекается из очереди						(5), 
	// с очереди снимается блокировка, после чего запускается задача		(6).
	// 
	// Когда задача будет выполнена, фьючерс, связанный с ней, перейдет в состояние готовности.
	void gui_thread_4_2_4()
	{
		cout << "I'm void gui_thread_4_2_4() for GUI в потоке ID == " 
			 << std::this_thread::get_id() << endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(30));

		while (!gui_shutdown_message_received_4_2_4())						// 2
		{
			cout << "I'm cycle of the void gui_thread_4_2_4()" << endl;
			// Ну что-то там делаем
			get_and_process_gui_message_4_2_4();							// 3
		
			// Создаем задачу типа std::packaged_task<void()> 
			std::packaged_task<void()> task;
			//
			{
				std::lock_guard<std::mutex>		lk(mtx_4_2_4);
				if (tasks_4_2_4.empty())									// 4
					continue;
				task = std::move(tasks_4_2_4.front());						// 5
				tasks_4_2_4.pop_front();
			}
			// Запуск на выполнение (вызов "напрямую") очередной задачи, вынутой из очереди
			//task();														// 6
			// Можно и запустить в отдельном потоке
			std::thread thread_for_task(std::move(task)); 

			std::this_thread::sleep_for(std::chrono::milliseconds(30));

			thread_for_task.join();
		}
	};
	// Помещение задачи в очередь: предоставленная функция создает новую упакованную задачу		(7), 
	// вызовом компонентной функции get_future() из этой задачи получается фьючерс				(8) 
	// и задача помещается в список																(9)
	// перед тем, как фьючерс возвращается вызывающему коду										(10).
	// 
	// Затем код, отправивший сообщение потоку GUI, может ожидать фьючерс, если ему требуется узнать
	// о завершении задачи, или же проигнорировать фьючерс, если эта информация	не нужна.
	template<typename Func>
	std::future<void>	post_task_for_gui_thread_4_2_4(Func f)
	{
		cout	<< "\nI'm void post_task_for_gui_thread_4_2_4() for GUI в потоке ID == " 
				<< std::this_thread::get_id() << endl;
		cout	<< "I'm void post_task_for_gui_thread_4_2_4() Запуск функции в блоке try - cach" 
				<< endl;
		// Передаем функции на упаковку и помещение в очередь задач
		std::packaged_task<void()>	task(f);													// 7
		std::future<void>			result;
		/*
			Eсли вызов функции, инициированный как часть std::async, 
			выдает исключение, 
			оно сохраняется во фьючерсе на месте сохраненного значения, 
			фьючерс приводится в состояние готовности 
			и вызов get() повторно выдает сохраненное исключение. 
		*/
		try	{
			result = task.get_future();															// 8
		}
		catch (const std::out_of_range& e) {
			cout << "----------!!!!!!-----I'm void post_task_for_gui_thread_4_2_4() Авария у функции " 
				 << endl; 
			cout << e.what();
		}
		catch (...) {
			cout << "----------!!!!!!-----I'm void init_listing_4_2_4() Не угадал тип исключения :(" 
				 << endl;
		};

		std::lock_guard<std::mutex>		lk(mtx_4_2_4);
		tasks_4_2_4.push_back(std::move(task));													// 9
		
		return result;																			// 10
	}
	// Функции, которые будут упакованы в задачи std::packaged_task<> 
	// и помещены в очередь задач на выполнение
	// диспетчер задач в потоке void gui_thread_4_9() будет просматривать очередь 
	// и запускать последовательно
	// все находящиеся в ней задачи
	void func_1_4_2_4() 	{ 
		cout << "1 +++++++++++++++++I'm void func_1_4_2_4() for GUI в потоке ID == " 
			 << std::this_thread::get_id() << endl; 
	};
	void func_2_4_2_4() 	{ 
		cout << "2 +++++++++++++++++I'm void func_2_4_2_4() for GUI в потоке ID == " 
			 << std::this_thread::get_id() << endl; 
	};
	void func_3_4_2_4() { 
		cout << "3 ++++++++++++++++I'm void func_3_4_2_4() for GUI в потоке ID == " 
			 << std::this_thread::get_id() << endl; 
		cout << "У меня авария SOS!" << endl;
		throw std::out_of_range("Что-то там с границами контейнера! :(\n");
	};

	void init_listing_4_2_4()	{
		cout << "I'm init_listing_4_2_4() - начинаю работу в потоке ID == " 
			 << std::this_thread::get_id() << endl;
		
		cond_for_cycle_4_2_4 = false;
		
		// Запускаем поток с диспетчером задач
		std::thread gui_bg_thread_4_2_4(gui_thread_4_2_4);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		
		auto result_1 = post_task_for_gui_thread_4_2_4(func_1_4_2_4);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		
		auto result_2 = post_task_for_gui_thread_4_2_4(func_2_4_2_4);
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		
		auto result_3 = post_task_for_gui_thread_4_2_4(func_3_4_2_4);
		
		// Здесь все сработало. Искючение перeдается через функцию get()
		try {
			result_3.get();							// 8
		}
		catch (const std::out_of_range& e) {
			cout << "-------!!!!!!----- I'm void post_task_for_gui_thread_4_2_4() Авария у функции "   
				 << endl;
			cout << e.what();
		}
		catch (...) {
			cout << "----------!!!!!!-----I'm void init_listing_4_2_4() Не угадал тип исключения :(" 
				 << endl;
		};
		cout << "I'm init_listing_4_2_4() Посплю 200 миллисекунд" << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

		cond_for_cycle_4_2_4 = true;

		gui_bg_thread_4_2_4.join();
	};
	/*
		******************************************************************************
		Исключения с std::promise<>
		******************************************************************************
	*/
	// Функтор, производящий некоторые вычисления
	struct calc_functor_4_2_4
	{
		int		index;
		void operator()(std::promise<double>& prms)
		{
			cout << "***************** I'm  operator() of calc_functor в потоке с ID == " 
				 << std::this_thread::get_id() << endl;

			// Здесь промис устанавливает свое шаблонное значение и тем самым
			// приводит связанный фьючерс в состояние готовности
			try {
				cout << "I'm calc_functor - пока все хоккей? :), а теперь запустим счеты... " << endl;
				// Компонентный вычислитель calc() 
				// выполняет собственно вычисления 
				// или генерирует исключение throw
				prms.set_value(calc());	
				// Результат вычисления или исключение передается во фьючерс, 
				// но, если calc() выдаст исключение, 
				// видимо, уже из следующей секции --->
			}
			catch(...)
			{
				cout << "I'm calc_functor -------!!!!!!----- что-то пошло не так :( -> set_exception " 
					 << endl;
				prms.set_exception(std::current_exception());
				/*
					Для извлечения выданного исключения здесь используется 
					функция std::current_exception(), чтобы сохранить новое исключение напрямую, 
					не выдавая его, можно было бы в качестве альтернативы 
					воспользоваться функцией std::make_exception_ptr():
				*/
				//	prms.set_exception(std::make_exception_ptr(std::logic_error("-!- что-то не так")));
				/*
					Если тип исключения известен, и следует предпочесть именно его, 
					то такой код выглядит намного понятнее, чем код с применением блока try-catch, 
					это не только упрощает код, 
					но и расширяет возможности компилятора в области оптимизации кода.
				
					Еще один способ сохранения исключения во фьючерсе 
					заключается в уничтоже­нии связанного с фьючерсом 
					объекта std::promise 
					или 
					объекта std::packaged_task
					без вызова каких-либо set-функций в отношении промиса 
					или без обращения к упа­кованной задаче. 
				
					В любом случае деструктор std::promise или std::packaged_task 
					сохранит исключение std::future_error 
					с кодом ошибки std::future_errc::broken_promise в связанном состоянии, 
					если фьючерс еще не перешел	в состояние готовно­сти: 
					созданием фьючерса дается обещание предоставить значение или исключение,
					а уничтожением источника этого значения 
					или исключения без их предоставления 
					это обещание нарушается.
				
					Если бы компилятор в таком случае ничего не сохранял во фьючерсе, 
					ожидающие потоки могли бы ожидать бесконечно.
				*/
			}
		}
		double calc() const
		{
			double	result	= 0.0;
			if (index != 0)
				result = 3.14 * 1000.0 / static_cast<double>(index) / 1956.0;
			else {
				cout << "I'm calc() of calc_functor - что-то пошло не так :( -> throw " 
					 << endl;
				throw std::out_of_range("!!!--Exc. calc() of calc_functor - На ноль не делим!!! :(\n");
			}
			return result;
		}
	};
	// Функтор печати
	struct print_functor_4_2_4
	{
		void operator()(std::future<double>& ftrs)
		{
			cout << "\n*****************  I'm operator() of functor print_functor в потоке с ID == " 
				 << std::this_thread::get_id() << endl;
			
			double result = 0.0;
			
			try {
				result = ftrs.get();
			}
			catch (const std::out_of_range& e) {
				cout << e.what();
			}
			catch (...) {
				cout << "I'm functor print_functor ---!!!!!!--- Не угадал тип исключения :(" << endl;
			};
			cout << "Результат расчета:  " << result << endl;
		}
	};
	void promis_futures_master_4_2_4()
	{
		cout << "\n	I'm promis_futures_master() в потоке ID == " << std::this_thread::get_id() << endl;
		{
			// Создадим промис
			std::promise<double>	prms;
			// ...и получим связанный с ним фьючерс
			std::future<double>		ftrs = prms.get_future();

			// Запускаем функтор расчета
			calc_functor_4_2_4		calc_fnctr{};
			calc_fnctr.index = 10;
			std::thread				thrd_prms(calc_fnctr, ref(prms));
			// Получим результат его работы, то есть этот функтор будет ждать завершения работы
			// другого функтора, в котором промис по завершении взведет в готовность связанный фьючерс
			// компонентной функцией set_value()

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			print_functor_4_2_4		prnt_fnctr;
			std::thread				thrd_ftrs(prnt_fnctr, ref(ftrs));

			thrd_ftrs.join();
			thrd_prms.join();
		}
		cout << "\n\nПроверим, как там с делением на ноль\n" << endl;
		{
			// Создадим промис
			std::promise<double>	prms;
			// ...и получим связанный с ним фьючерс
			std::future<double>		ftrs = prms.get_future();

			// Запускаем функтор расчета
			calc_functor_4_2_4		calc_fnctr{};
			calc_fnctr.index		= 0;
			std::thread				thrd_prms_new(calc_fnctr, ref(prms));

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			print_functor_4_2_4		prnt_fnctr;
			std::thread				thrd_ftrs_new(prnt_fnctr, ref(ftrs));
			thrd_ftrs_new.join();
			thrd_prms_new.join();
		}
	};
	
};	// End of the namespace 'ns_AW_chapter_4'
/*
	*	*********************************************************************************
	*	РЕЗЮМЕ
	*	*********************************************************************************
	Нужен некий способ, чтобы поток мог ожидать завершения задачи, выполняемой другим потоком,
	не расходуя на это время центрального процессора. 

*/
//		#ifndef GUARD_AW_chapter_4_hpp
#endif
