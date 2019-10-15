#include <iostream>

struct NullType {};

template<class T, class U>
struct TypeList
{
    typedef T head;
    typedef U tail;
};

template<int N, class U>
struct IntList {
    static const int value = N;
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

// List of indices

template <class T, class L, int N>
struct ListIndexOfHelper;

template <class T, int N>
struct ListIndexOfHelper<T, NullType, N> {
    typedef NullType value;
};

template <class T, class U, int N>
struct ListIndexOfHelper<T, TypeList<T, U>, N> {
    typedef IntList<N, typename ListIndexOfHelper<T, U, N + 1>::value> value;
};

template <class T1, class T2, class U, int N>
struct ListIndexOfHelper<T1, TypeList<T2, U>, N> {
    typedef typename ListIndexOfHelper<T1, U, N + 1>::value value;
};

template <class T, class L>
struct ListIndexOf {
    typedef typename ListIndexOfHelper<T, L, 0>::value value;
};

int main() {
    typedef TypeList<int, TypeList<double, TypeList<int, TypeList<char, NullType> > > > T;
    std::cout << IndexOf<int, T>::value << std::endl;
    std::cout << IndexOf<double, T>::value << std::endl;
    std::cout << IndexOf<char, T>::value << std::endl;
    std::cout << IndexOf<unsigned int, T>::value << std::endl;
    std::cout << IndexOf<NullType, T>::value << std::endl;

    std::cout << "Lists" << std::endl;
    typedef ListIndexOf<int, T>::value IntInd;
    std::cout << IntInd::value << std::endl;
    std::cout << IntInd::tail::value << std::endl;
    typedef ListIndexOf<char, T>::value CharInd;
    std::cout << CharInd::value << std::endl;
    return 0;
}