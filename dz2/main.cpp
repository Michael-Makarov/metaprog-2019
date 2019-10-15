#include <iostream>

struct NullType {};

template<class T, class U>
struct TypeList
{
    typedef T head;
    typedef U tail;
};

template <class T, class L>
struct IndexOf;

template <class T, class U>
struct IndexOf<T, TypeList<T, U> > {
    static const int value = 0;
};

template <class T>
struct IndexOf<T, NullType> {
    static const int value = -1;
};

template <class T1, class T2, class U>
struct IndexOf<T1, TypeList<T2, U> > {
    typedef IndexOf<T1, U> Next;
    static const int value = Next::value == -1 ? -1 : (Next::value + 1);
};

int main() {
    typedef TypeList<int, TypeList<double, TypeList<int, TypeList<char, NullType> > > > T;
    std::cout << IndexOf<int, T>::value << std::endl;
    std::cout << IndexOf<double, T>::value << std::endl;
    std::cout << IndexOf<char, T>::value << std::endl;
    std::cout << IndexOf<unsigned int, T>::value << std::endl;
    std::cout << IndexOf<NullType, T>::value << std::endl;
    return 0;
}