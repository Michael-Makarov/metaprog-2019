#include <iostream>

struct NullType {};

template<class T, class U>
struct TypeList
{
    typedef T head;
    typedef U tail;
};

template <class L, int N>
struct TypeAt {
    typedef typename TypeAt<typename L::tail, N - 1>::type type;
};

template <class T1, class T2>
struct TypeAt<TypeList<T1, T2>, 0> {
    typedef T1 type;
};

template <class T>
struct Holder {
    T value;
};

template <class TList, template <class> class Unit>
class GenScatterHierarchy;

template <class T1, class T2, template <class> class Unit>
class GenScatterHierarchy<TypeList<T1, T2>, Unit> :
    public GenScatterHierarchy<T1, Unit>,
    public GenScatterHierarchy<T2, Unit> {
public:
    typedef GenScatterHierarchy<TypeList<T1, T2>, Unit> CurType;
    typedef TypeList<T1, T2> TList;
    typedef GenScatterHierarchy<T1, Unit> LeftBase;
    typedef GenScatterHierarchy<T2, Unit> RightBase;

    template <class S>
    S& get_value() {
        return static_cast<Unit<S>*>(this)->value;
    }

    template <int N, class Dummy=NullType>
    struct IndexValueGetter {
        typename TypeAt<TList, N>::type& operator()(CurType* obj) {
            return static_cast<RightBase*>(obj)->template get_value<N - 1>();
        }
    };

    template <class Dummy>
    struct IndexValueGetter<0, Dummy> {
        typename TypeAt<TList, 0>::type& operator()(CurType* obj) {
            return static_cast<Unit<typename TypeAt<TList, 0>::type>* >(static_cast<LeftBase*>(obj))->value;
        }
    };

    template <int N>
    typename TypeAt<TList, N>::type& get_value() {
        return IndexValueGetter<N>()(this);
    }
};

template <class AtomicType, template <class> class Unit>
class GenScatterHierarchy : public Unit<AtomicType> {
public:
    typedef Unit<AtomicType> LeftBase;
};

template <template <class> class Unit>
class GenScatterHierarchy<NullType, Unit> {};

int main() {
    typedef GenScatterHierarchy<TypeList<int, TypeList<std::string, TypeList<double, NullType> > >, Holder> T;
    T a;
    a.get_value<int>() = 10;
    a.get_value<std::string>() = "asd";
    a.get_value<double>() = 3.14;
    std::cout << a.get_value<int>() << std::endl;
    std::cout << a.get_value<std::string>() << std::endl;
    std::cout << a.get_value<double>() << std::endl;
    std::cout << a.get_value<1>() << std::endl;
    std::cout << a.get_value<2>() << std::endl;
    std::cout << a.get_value<0>() << std::endl;
    return 0;
}