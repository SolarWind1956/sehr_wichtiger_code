/* 
	Coroutines_And_Heliostat_Field.cpp
*/ 
#ifdef __GNUC__
	#pragma GCC diagnostic ignored "-Wanalyzer-malloc-leak"
	#pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
	#pragma GCC optimize ("O2")  // normal optimization
	#pragma GCC diagnostic ignored "-Wanalyzer-all" // suppress analyzer warnings
#endif	
/*
*/
#include <iostream>
using std::cout; using std::endl; using std::flush;  using std::cerr;
#include <coroutine>
#include <queue>
#include <locale>
#include <thread>
#include <chrono>
/*
	Постановка новой задачи
	
	1.	В контуре управления имется десять объектов (гелиостатов) с `heliostats_num` датчиками и
		`heliostats_num` парами приводов.
		
	1. 	Есть диспетчер циклов управления, который:
		
		1.1 	Запускает задачу опроса датчиков, обработки информации с этих датчиков
			и расчитывает величины управляющих воздействий для каждой пары приводов.
			
			Время опроса и обработки информации от оптических датчиков 
			в целом по полю гелиостатов составляет `poll_time` секунд.
			
		1.2.	Запускает внутренний диспетчер управления приводами.
			
			Для каждого из приводов может быть до `max_control_actions` заданий на управление,
			выполнение каждого из которых длится не более `control_action_time` секунд. 
			
			Во время `control_action_time` периода отработки привода, драйвер засыпает,
			возвращая управление внутреннему диспетчеру, который переходит к следующей паре
			приводов.
			
			Когда все `steppers_num` шаговых двигателей из очереди получили свои задания,
			внутренний диспетчер передает управление внешнему диспетчеру, 
			который запускает задачу опроса датчиков 
			и расчета следующих порций управляющих воздействий 
			для всех `steppers_num` приводов.
			
			После этого управление передается снова внутреннему диспетчеру.
			
		1.3. 	Драйвер привода реализован как сопрограмма	
			
*/
/*
	================================================================
	Структуры и функции для механизмов сопрограмм
	================================================================
*/
struct Task {
    struct promise_type {
		// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
        auto get_return_object() -> Task 
			{ return Task{std::coroutine_handle<promise_type>::from_promise(*this)}; }
		// NOLINTNEXTLINE(readability-convert-member-functions-to-static)	
        auto  initial_suspend() -> std::suspend_always
			{ return {}; }
        
        // ИСПРАВЛЕНИЕ 1: Говорим корутине автоматически очищать память после co_return
		// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
        auto  final_suspend() noexcept -> std::suspend_never 
			{ return {}; }
        // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
        void  unhandled_exception() 
			{ std::terminate(); }
        void return_void() 
			{}
    };
    std::coroutine_handle<promise_type> 	corout_hndl;
	
    Task(std::coroutine_handle<promise_type> handle) : corout_hndl(handle) 
		{}
    Task(const Task&) 	= default;
	auto operator=(const Task&)  -> Task&  = default;
	
	Task(const Task&&) 	= delete;
	auto operator=(const Task&&) -> Task&  = delete;
		
    // ИСПРАВЛЕНИЕ 2: Убираем принудительный destroy(). 
    // Теперь деструктор временной обертки не трогает живую память корутины в очереди!
    ~Task() = default;
};
//	Очередь задач сопрограмм
struct OS_Task_Queue{
	static std::queue<std::coroutine_handle<>> 	task_queue;
};
std::queue<std::coroutine_handle<>> 	OS_Task_Queue::task_queue{};
// 	------------------------------------------------------------------------------------------------------------------------------------------------------------
// 	НАШ КАСТОМНЫЙ ДРАЙВЕР ПЕРЕКЛЮЧЕНИЯ (Аналог системного вызова YIELD)
// 	------------------------------------------------------------------------------------------------------------------------------------------------------------
struct OsYield {
    // 	1. Нужно ли засыпать? (Ложь = всегда приостанавливаем выполнение)
	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    auto await_ready() -> bool 
		{ return false; }

    // 	2. В момент приостановки компилятор передает сюда дескриптор корутины
	// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void await_suspend(std::coroutine_handle<> handle) {
        cout << "[Драйвер] Сопрограмма вызвала YIELD. Регистрируем её в Очереди ОС...\n";
        
        // 	Помещаем контекст корутины в системную очередь задач
        OS_Task_Queue::task_queue.push(handle); 
    }

    // 	3. Что вернуть внутрь корутины после просыпания (ничего)
    void await_resume() {}
};
// 	------------------------------------------------------------------------------------------------------------------------------------------------------------
// 	СУЩНОСТЬ СИСТЕМЫ: КОРУТИНА, УПРАВЛЯЮЩАЯ СВОИМ ПЛАНИРОВАНИЕМ
// 	------------------------------------------------------------------------------------------------------------------------------------------------------------
auto stepper_coroutine(int stepper_id) -> Task {
    cout << "[Привод " << stepper_id << "] Шаг 1. Начинаем движение.\n";
    
    co_await OsYield{}; 
    
    cout << "[Привод отработал успешно " << stepper_id << "] Шаг 2. Завершаем по сигналу\n";
    // 	Память корутины очистится сама сразу после этой строчки благодаря suspend_never
    co_return; 
}
/*
	****************************************************************************************************
	*	Технологические структуры и фукции
	****************************************************************************************************
*/
struct	Heliostat{
	struct optic_sensor{
		
	};  
		
	static constexpr int	max_control_actions{4};
	static constexpr int	control_action_time{2};
	
	int	control_action_num{0};
	
	int 	Zenith_stepper_control_action_num{0};
	int 	Azimuth_stepper_control_action_num{0};
};
struct	Heliostat_Field{
	//	Количество гелиостатов на поле гелиостатов
	static constexpr int		heliostats_num{10};			
	//	Общее количество шаговых двигателей
	static constexpr int		steppers_num{heliostats_num * 2};	
	//	Обще время опроса и обработки информации от оптических датчиков (в секундах)
	static constexpr int		poll_time{5};	
	//	Флаг останова процесса управления: true - завершение работы	
	static bool				stop_flag;	
};
//	Инициализация статических переменных
bool Heliostat_Field::stop_flag{true};

//	Функция опроса и обработки показаний оптических датчиков
void		optic_sensors_poll(){}
//	Функция расчета управляющих воздействий на приводы гелиостатов
void		control_computation(){
	
}
void		heliostat_field_control(){
	cout << "\nФункция heliostat_field_control() в потоке с ID == " << std::this_thread::get_id() << '\n';
	//	Цикл управления полем гелиостатов
	while (!Heliostat_Field::stop_flag){
		//	Функция опроса и обработки показаний оптических датчиков
		optic_sensors_poll();
		//	Функция расчета управляющих воздействий на приводы гелиостатов
		control_computation();
		
		//	Цикл формирования задач для приводов гелиостатов
		// 	Первичный запуск: толкаем их до первой точки ожидания (до OsYield)
		cout << "\n=== [Внешний диспетчер] Формируем новые задачи для приводов ===" << '\n';
		for (auto i = 1; i <= Heliostat_Field::steppers_num; ++i){
			// Создаем и пинаем до первого yield
			stepper_coroutine(i).corout_hndl.resume(); 
		}
		cout << "\n=== [Внутренний диспетчер] Запуск главного цикла обработки очереди ОС ===" << '\n';
		while (!OS_Task_Queue::task_queue.empty()) {
			std::coroutine_handle<> next_task = OS_Task_Queue::task_queue.front();
			OS_Task_Queue::task_queue.pop();

			cout << "[Диспетчер] Извлекли задачу из очереди. Выделяем квант времени...\n";
			next_task.resume(); // Корутина просыпается, делает Шаг 2 и умирает
			
			std::this_thread::sleep_for(std::chrono::seconds(2)); 
			// Немного ускорил шаг для наглядности
		}
		cout << "[Ядро ОС] Очередь пуста. Все приводы отработали. Остановка.\n";
	}
}

auto main() -> int
{  
	cout << "\n------------------ I'am Coroutines_by_Gemini_AI_Final.cpp" << '\n';
	// 	Настройка кодировки UTF-8 для корректного вывода в Windows
    std::setlocale(LC_ALL, "UTF-8");
#ifdef _WIN32
    std::system("chcp 65001 > nul");
#endif
	cout << "\nФункция main() в потоке с ID == " << std::this_thread::get_id() << '\n';
	
	Heliostat_Field::stop_flag = false;				//	Разрешаем работу с полем гелиостатов
	
	// 	Создаем объект класса потока, инициализируя его именем функции,
	// 	которая будет в этом потоке выполняться
	std::thread thread_heliostat_field_control(heliostat_field_control);
		
	std::this_thread::sleep_for(std::chrono::seconds(10));	//	Работаем какое-то время
	
	Heliostat_Field::stop_flag = true;				//	Останавливаем работу поля гелиостатов
	
	// Ждем, пока отработает поток с функцией приветствия
	thread_heliostat_field_control.join();

	system("pause");
	
    return 0;
};		
