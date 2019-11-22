#include <iostream>

struct NullType {};

template<class T, class... U>
struct TypeList {
    using head = T;
    using tail = TypeList<U...>;
};

template <class T>
struct TypeList<T> {
    using head = T;
    using tail = NullType;
};

template <class... T>
class ArgsPack;

template <>
class ArgsPack<> {
public:
    ArgsPack() {}

    template <class F, class... Args>
    auto Call(const F& func, Args&&... args) -> decltype(func(std::forward<Args>(args)...)) {
        return func(std::forward<Args>(args)...);
    }
};

template<typename T>
T* ConvertToPtr(T& t) {
    return &t;
}
template<typename T>
T* ConvertToPtr(T* t) {
    return t;
}

template<typename T>
T& ConvertToRef(T& t) {
    return t;
}
template<typename T>
T& ConvertToRef(T* t) {
    return *t;
}

template <class Head, class... Tail>
class ArgsPack<Head, Tail...> : public ArgsPack<Tail...> {
private:
    Head value_;
public:
    using Base = ArgsPack<Tail...>;
    template <class T, class... U>
    ArgsPack(T&& head, U&&... tail) : Base(std::forward<U>(tail)...), value_(std::forward<T>(head)) {}

    template <class F, class... Args>
    auto Call(const F& func, Args&&... args) -> decltype(this->Base::Call(func, std::forward<Args>(args)..., ConvertToRef(value_))) {
        return this->Base::Call(func, std::forward<Args>(args)..., ConvertToRef(value_));
    }

    template <class F, class... Args>
    auto Call(const F& func, Args&&... args) -> decltype(this->Base::Call(func, std::forward<Args>(args)..., ConvertToPtr(value_))) {
        return this->Base::Call(func, std::forward<Args>(args)..., ConvertToPtr(value_));
    }
};

template<class TObject, typename TCE>
struct SimpleDelegate {
private:
    TObject& m_Object;
    TCE m_CallableEntity;
public:
    SimpleDelegate(TObject& object, const TCE& ptr)
        : m_Object(object), m_CallableEntity(ptr) {}
    template<typename... Params2>
    auto operator()(Params2... args) const
    -> decltype((m_Object.*m_CallableEntity) (args...)) {
        return (m_Object.*m_CallableEntity) (args...);
    }
};

template <class TCE>
struct CEWrapper {
private:
    TCE m_CallableEntity;
public:
    CEWrapper(const TCE& ptr) : m_CallableEntity(ptr) {}
    template<typename... Params2>
    auto operator()(Params2... args) const -> decltype(m_CallableEntity(args...)) {
        return m_CallableEntity(args...);
    }
};

template<class TCE, class... Params>
class Functor {
private:
    ArgsPack<Params...> params_;
    TCE callable_entity_;
public:
    Functor(const TCE& ce, Params&&... params) : callable_entity_(ce), params_(std::forward<Params>(params)...) {}

    template <class TObject>
    auto Call(TObject& obj) -> decltype(params_.Call(SimpleDelegate<TObject, TCE>(obj, callable_entity_))) const {
        return params_.Call(SimpleDelegate<TObject, TCE>(obj, callable_entity_));
    }

    template <class TCallableEntity = TCE>
    auto Call() -> decltype(params_.Call(CEWrapper<TCallableEntity>(callable_entity_))) const {
        return params_.Call(callable_entity_);
    }
};

template<class TCallableEntity, typename...Params>
Functor<TCallableEntity, Params...> make_functor(const TCallableEntity& callable_entity, Params&&... given_params) {
    return Functor<TCallableEntity, Params...>(callable_entity, std::forward<Params>(given_params)...);
}

int function(int a, float b, short c) {
    return a * b + c;
}

int procedure(int a, int b) {
    return 123 + a + b;
}

int foo(int a) {
    return 123 + a;
}

int bar() {
    return 123;
}

class Y {
public:
    int value_;
};

class X {
public:
    float process(Y& y) {
        return y.value_ / 2.0;
    }
    short func() {
        return 12;
    }
};

int main() {
// обычные функции
    auto functor0 = make_functor(&procedure, 1, 2);
    int x0 = functor0.Call();
    std::cout << x0 << std::endl;
    auto functor1 = make_functor(&function, 1, 2, 3);
    int x1 = functor1.Call();
    std::cout << x1 << std::endl;
// функции-члены
    auto functor2 = make_functor(&X::process, Y());
    X x;
    float y0 = functor2.Call(x);
    std::cout << y0 << std::endl;
// лямбды
    Y y;
    auto functor3 = make_functor([](Y*) {return 32;}, y);
    int x2 = functor3.Call();
    std::cout << x2 << std::endl;
    return 0;
}
