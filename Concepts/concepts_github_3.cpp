// 	concepts_github_3.cpp
#include <type_traits>
#include <utility>
#include <vector>
#include <iostream>
#include <map>
#include <string>

template <class T>
struct TypeWrapper {
    using type = T;
};

template <class Impl> // Impl --- будет конечным типом, имплементирующим интерфейс
class ISummator {
public:

    static constexpr auto ValueType() {
        return TypeWrapper<typename Impl::value_type>{};
    }

    // потребуем, чтоб Impl предоставлял как минимум константный (!а может и статический!)
    // метод Sum принимающий какие-то два аргумента.
    // используем perfect forwarding для возвращаемого значения (dectype(auto))
    // и аргументов (T&&, std::forward<T>) чтобы все передавалось и возвращалось как есть:
    // копии -- копиями, ссылки -- ссылками (сохранить rvalue/lvalue и прочие *value категории)
    template <class T1, class T2> 
    decltype(auto) Sum(T1&& a, T2&& b) const {
        return AsImpl()->Sum(std::forward<T1>(a), std::forward<T2>(b));
    }
    // аналогично потребуем, чтоб Impl предоставил как минимум константный метод Zero, не принимащий ничего
    decltype(auto) Zero() const {
        return AsImpl()->Zero();
    }
private:
    // не разрешаем никому, кроме имплементирующего типа, конструировать этот базовый класс
    friend Impl; 
    ISummator() = default;
    // мы не используем виртуальный деструктор
    // так что разрушать объект тоже запрещаем,
    // чтоб никто не делал delete через указатель
    // на этот класс
    ~ISummator() = default;

    const Impl* AsImpl() const {
        // Будем требовать, чтоб Impl был нашим наследником
        // иначе static_cast просто не скомпилируется
        // причем мы можем здесь обращаться с Impl как с полным типом. 
        // Потому что методы шаблонов компилируются только тогда, когда к ним будут обращаться вне шаблонов
        return static_cast<const Impl*>(this);
    }
};
// наследуемся от базового класса, указывая параметром шаблона самих себя!
// Внутри шаблона в этот момент DefaultSummator является неполным типом (конечно, мы его только начали описывать!)
// Однако никаких ошибок не возникает -- мы не трогаем методы ISummator, они не компилируеются
// и мы не обращаемся к нашему пока еще не полному типу!
class DefaultSummator : public ISummator<DefaultSummator> {
public:    
    using value_type = float;

    static float Zero() {
        return 0;
    }
    static float Sum(float a, float b) {
        return a + b;
    }
};
template<class T, class Summator>
auto accumulate	(	const std::vector<T>& 		arr
				, 	const ISummator<Summator>& 	op
				) -> 
				std::enable_if_t	<	std::is_same_v	<	T
														, 	typename decltype(op.ValueType())::type
														>
									, 	T
									> 
{
	auto sum = op.Zero();
	for (auto x : arr) 
		sum = op.Sum(sum, x);
	return sum;
}

int main() {
	/*
    std::vector<std::string> v = {"a", "b", "c"};

    auto r = accumulate(v, DefaultSummator());
	std::cout << r << std::endl;
	*/
	std::vector<float>	int_vector{11.1, 21.2, 42.4, 84.8};
	std::cout << accumulate(int_vector, DefaultSummator()) << std::endl;
}
