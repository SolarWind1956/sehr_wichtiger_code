
/*
	*************************************************
	Раздел 22.3. Интерфейс моста
	*************************************************
'Шаблон класса' FunctorBridge отвечает за владение 'базовым функциональным объектом' и манипуляции с ним.
Он реализован как 'абстрактный базовый класс',
формирующий основу для 'динамического полиморфизма' FunctionPtr:						*/
	template<typename R, typename... Args>
	class FunctorBridge
	{
	public:
		virtual ~FunctorBridge() {
		}
		virtual FunctorBridge*	clone()								const = 0;
		virtual R					invoke(Args... args)					const = 0;
		virtual bool				equals(FunctorBridge const* fb)		const = 0;
	};
/*
	FunctorBridge предоставляет основные операции, 
необходимые для работы с 'хранимым функциональным объектом' посредством 'виртуальных функций': 
	-	деструктор, 
	-	операция clone() для копирования и
	-	invoke() для вызова базового функционального объекта. 
Не забудьте определить clone() и invoke() как 'константные функции-члены'.					
	Константность invoke() - это 'ремень безопасности' 
против вызова неконстантных перегрузок operator () посредством константных объектов FunctionPtr, 
что нарушает ожидания программистов.															
	С помощью этих виртуальных функций можно реализовать 'копирующий конструктор' и 'оператор вызова'
функции FunctionPtr. Смотри ниже.																		*/

/*
	*********************************************************************************************
	Это 'наш собственный' 'шаблон класса' 'обобщенного указателя на функцию', FunctionPtr, 
	обеспечивающий те же основные операции и возможности, 
	и могущий использоваться вместо std::function<>
	*********************************************************************************************
Интерфейс FunctionPtr довольно простой, обеспечивающий:
	-	создание, 
	-	копирование, 
	-	перемещение, 
	-	уничтожение, 
	-	инициализацию, 
	-	присваивание произвольных функциональных объектов, 
	а также 
	-	вызов базового функционального объекта. 
Наиболее интересной частью интерфейса является то,
как все это описано полностью в рамках 'частичной специализации' 'шаблона класса', 
служащей для 'разделения аргумента шаблона' (типа функции) 
на его составные части (типы результата и аргументов):
*/
// primary template:
template<typename Signature>
class FunctionPtr;

// partial specialization:
template<typename R, typename... Args>
class FunctionPtr<R(Args...)>
{
private:
	//	
	FunctorBridge<R, Args...>* bridge;									
public:
	// constructors:
	FunctionPtr() : bridge(nullptr) {
	}
	// Копирующие конструкторы
	FunctionPtr(FunctionPtr const& other);									// see functionptr-cpinv.hpp ниже

	FunctionPtr(FunctionPtr& other)
		: FunctionPtr(static_cast<FunctionPtr const&>(other)) {
	}
	// Перемещающий конструктор
	FunctionPtr(FunctionPtr&& other) : bridge(other.bridge) {
		other.bridge = nullptr;
	}

	 // Построение из произвольного функционального объекта:
	// construction from arbitrary function object:
	template<typename F> 
	FunctionPtr(F&& f);													// see functionptr-init.hpp ниже

	// assignment operators:
	FunctionPtr& operator=(FunctionPtr const& other) {
		FunctionPtr tmp(other);
		swap(*this, tmp);
		return *this;
	}
	FunctionPtr& operator=(FunctionPtr&& other) {
		delete bridge;
		bridge			= other.bridge;
		other.bridge		= nullptr;
		return *this;
	}
	// Построение и присваивание произвольного функционального объекта:
	// construction and assignment from arbitrary function objects:
	template<typename F> FunctionPtr& operator=(F&& f) {
		FunctionPtr tmp(std::forward<F>(f));
		swap(*this, tmp);
		return *this;
	}

	// destructor:
	~FunctionPtr() {
		delete bridge;
	}

	friend void swap(FunctionPtr& fp1, FunctionPtr& fp2) {
		std::swap(fp1.bridge, fp2.bridge);
	}
	explicit operator bool() const {
		return bridge == nullptr;
	}

	//  Pеализуем оператор operator== для FunctionPtr, 
	// который сначала проверяет функторы на нулевое значение, 
	// а затем выполняет делегирование в FunctorBridge:
	friend bool operator==(FunctionPtr const& f1, FunctionPtr const& f2)
	{
		if (!f1 || !f2)
		{
			return !f1 && !f2;
		}
		return f1.bridge->equals(f2.bridge);
	}

	friend bool operator!=(FunctionPtr const& fl, FunctionPtr const& f2)
	{
		return !(fl == f2);
	}
	/*
	Эта реализация корректна. Однако она имеет достойный сожаления недостаток: 
	если FunctionPtr инициализирован функциональным объектом (или этот объект ему присвоен позже),
	который не имеет подходящего оператора operator== (например, сюда входят лямбда-выражения), 
	то программа не будет компилироваться. 
	Это может быть сюрпризом, потому что operator== из FunctionPtr еще даже не был использован. 
	Многие другие шаблоны классов - например, std::vector - могут быть созданы с типами, 
	которые не имеют оператора operator==, 
	лишь бы этот оператор не был использован в программе для данных типов. 
	Данная проблема с operator== связана со 'стиранием типа': 
	поскольку мы, по сути, теряем тип функционального объекта, 
	при инициализации или присваивании FunctionPtr нужно захватить всю информацию, 
	которую нам необходимо знать о типе, до того, как это присваивание или инициализация завершатся.
	Эта информация включает вызов operator== функционального объекта, 
	потому что мы не можем быть уверены, потребуется ли он. 
	Код вызова operator== инстанцируется потому, что все виртуальные функции шаблона класса 
	(в данном случае SpecificFunctorBridge) 
	обычно инстанцируются при инстанцировании самого шаблона класса.*/
	// Вызов:
	// invocation:
	R operator()(Args... args) const;								// see functionptr-cpinv.hpp
};
/*
Чтобы выяснить, доступен ли operator==, перед тем как его вызывать, 
можно воспользоваться (довольно сложным) свойством (trait) на основе SFINAE					*/
	template<typename T>
	class IsEqualityComparable
	{
	private:
		// Проверка преобразуемости == и != в bool:
		// test convertibility of == and ! == to bool:
		static void* conv(bool);  // Для проверки конвертируемости в bool - to check convertibility to bool
		template<typename U>
		static std::true_type test(	decltype(conv(std::declval<U const&>() == std::declval<U const&>())),
								decltype(conv(!(std::declval<U const&>() == std::declval<U const&>())))
		);
		// Резервный вариант - fallback:
		template<typename U>
		static std::false_type test(...);
	public:
		static constexpr bool value = decltype(test<T>(nullptr, nullptr))::value;
	};
/*
Свойство IsEqualityComparable применяет типичный подход для проверки выражений:
две перегрузки test(), 
	-	одна из которых содержит тестируемое выражение, обернутое в decltype, и
	-	другая, которая содержит многоточие и потому принимает произвольные аргументы. 
Первая функция test() пытается сравнивать два объекта типа T const с использованием оператора ==, 
а затем убеждается, что результат может быть неявно преобразован в bool (для первого параметра) 
и передан оператору логического отрицания operator!() с результатом, преобразуемым в bool. 
Если обе операции корректны, оба типа параметров будут void*. 
С помощью свойства IsEqualityComparable можно построить шаблон класса TryEquals, 
который может либо вызывать == для данного типа (если он доступен), 
либо вызвать исключение, если подходящего оператора == не существует:				*/
	template<typename T, bool EqComparable = IsEqualityComparable<T>::value>
	struct TryEquals
	{
		static bool equals(T const& x1, T const& x2) {
			return x1 == x2;
		}
	};

	class NotEqualityComparable : public std::exception
	{
	};

	template<typename T>
	struct TryEquals<T, false>
	{
		static bool equals(T const& x1, T const& x2) {
			throw NotEqualityComparable();
		}
	};
/*
Наконец, c пoмoщью TryEquals в нaшeйpeaлизaции SpecificFunctorBridge
 мы можем обеспечить поддержку для оператора == в FunctionPtr всякий раз,
 когда типы сохраненных функциональных объектов совпадают 
 и функциональный объект поддерживает оператор ==:
	virtual bool equals(FunctorBridge<R, Args...> const* fb) const override
	{
		if (auto specFb = dynamic_cast<SpecificFunctorBridge const*>(fb))
		{
			return TryEquals<Functor>::equals(functor, specFb->functor);
		}
		// Функторы с разными типами не равны: 
		return false;
}
 */


/*
Этих файлов я пока не нахожу
functionptr-init.hpp
functionptr-cpinv.hpp
*/
/*
С помощью этих 'виртуальных функций' можно реализовать 
'копирующий конструктор' и 'оператор вызова' функции FunctionPtr:					*/
	template<typename R, typename... Args > 
	FunctionPtr< R(Args...)>::FunctionPtr(FunctionPtr const& other) : bridge(nullptr)
	{
		if (other.bridge) { bridge = other.bridge->clone(); }
	};

	template<typename R, typename... Args >
	R FunctionPtr<R(Args...)>::operator()(Args... args) const
	{
		return bridge->invoke(std::forward<Args> (args)...);
	};

/*
	**************************************************************************
	Раздел 22.4. Стирание типа
	**************************************************************************
Каждое инстанцирование FunctorBridge является 'абстрактным классом', 
поэтому его 'производные классы' 'ответственны'
за 'предоставление' 'фактических реализаций' своих 'виртуальных функций'. 

Для поддержки 'полного спектра' 'потенциальных функциональных объектов'
(т.е. неограниченного их множества) нам потребуется неограниченное количество производных классов. 
К счастью, мы можем достичь этого путем 'параметризации производного класса'
'типом хранимого функционального объекта':						*/
	template<typename Functor, typename R, typename... Args>
	class SpecificFunctorBridge : public FunctorBridge<R, Args...> 
	{
		Functor functor;									// Нужно, чтобы он имел реализацию bool operator==()

	public:
		template<typename FunctorFwd>
		SpecificFunctorBridge(FunctorFwd&& functor)
			: functor(std::forward<FunctorFwd>(functor)) {
		}
		virtual SpecificFunctorBridge* clone() const override {
			return new SpecificFunctorBridge(functor);
		}
		virtual R invoke(Args... args) const override {
			return functor(std::forward<Args>(args)...);
		}
		virtual bool equals(FunctorBridge<R, Args...> const* fb) const override {
			auto specFb = dynamic_cast<SpecificFunctorBridge const*>(fb);
			if (specFb)
			{
				// return (functor == specFb->functor);		// Пока не знаю, как тут быть :(
				// Вот так  смотри выше
				return TryEquals<Functor>::equals(functor, specFb->functor);
			};
			// Функторы с разными типами всегда различны: 
			return false;
		}
	};
/*
Каждый экземпляр SpecificFunctorBridge хранит копию функционального объекта 
(типом которого является Functor), который может быть вызван, 
скопирован (путем клонирования SpecificFunctorBridge) или уничтожен (неявно в деструкторе). 
Экземпляры SpecificFunctorBridge создаются каждый раз, 
когда FunctionPtr инициализируется новым функциональным объектом (что завершает наш пример FunctionPtr):*/
	template<typename R, typename... Args> 
	template<typename F>
	FunctionPtr<R(Args...)>::FunctionPtr(F&& f) : bridge(nullptr)
	{
		using Functor	= std::decay_t<F>;
		using Bridge		= SpecificFunctorBridge<Functor, R, Args...>;

		bridge			= new Bridge(std::forward<F>(f));
	};
/*
Обратите внимание: 
в то время как сам конструктор FunctionPtr шаблонизируется функциональным объектом типа F, 
этот тип известен только определенной специализации SpecificFunctorBridge 
(описанной как псевдоним типа Bridge). 
	После того как вновь выделенный экземпляр Bridge присваивается члену-данному bridge, 
дополнительная информация о конкретном типе F становится потерянной 
из-за преобразования производного типа в базовый - из Bridge* в FunctorBridge<R, Args. . . >*. 
	Хотя этот тип можно запросить (среди прочего) с помощью dynamic_cast, 
класс FunctionPtr делает указатель bridge закрытым, 
так что клиенты FunctionPtr не имеют доступа к самому типу.
	Эта потеря информации о типе объясняет,
почему для описания методики преодоления пропасти между статическим и динамическим полиморфизмом 
часто используется термин стирание типа (type erasure).							
	Одной из особенностей реализации является использование std::decay для получения типа Functor, 
который делает выведенный тип F подходящим для хранения, например, 
преобразуя 'ссылки на функциональные типы' в 'типы указателей на функции'
и удаляя const, volatile и 'ссылочные типы' 'верхнего уровня'.								*/
/*
	*****************************************************************
	Раздел 22.5. Условная передача владения
	*****************************************************************
Шаблон FunctionPtr 'почти полностью' 'заменяет указатель на функцию'. 
Именно “почти” - он пока что не поддерживает одну операцию, предоставляемую указателями на функции: 
проверку, будут ли два объекта FunctionPtr вызывать одну и ту же функцию.
Добавление такой операции требует оснащения FunctorBridge операцией equals:
	virtual bool equals(FunctorBridge const* fb) const = 0;
вместе с реализацией в Specif icFunctorBridge, которая сравнивает хранимые функциональные объекты, 
если они имеют одинаковые типы:


*/