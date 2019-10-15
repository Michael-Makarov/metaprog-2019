#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <sstream>

template <class T>
std::tuple<T> ReadLine(std::istream& in) {
    T value;
    in >> value;
    return std::make_tuple(value);
}

template <class T, class T1, class ...U>
std::tuple<T, T1, U...> ReadLine(std::istream& in) {
    T value;
    in >> value;
    char c;
    in >> c;
    return std::tuple_cat(std::make_tuple(value), ReadLine<T1, U...>(in));
}

template <class ...U>
std::vector<std::tuple<U...>> ReadCSV(const std::string& filename) {
    std::ifstream in(filename);
    std::vector<std::tuple<U...>> res;
    std::string s;
    while (std::getline(in, s)) {
        std::stringstream sin(s);
        res.push_back(ReadLine<U...>(sin));
    }
    return res;
}

// helper function to print a tuple of any size
template<class Tuple, std::size_t N>
struct TuplePrinter {
    static void print(const Tuple& t)
    {
        TuplePrinter<Tuple, N-1>::print(t);
        std::cout << ", " << std::get<N-1>(t);
    }
};

template<class Tuple>
struct TuplePrinter<Tuple, 1> {
    static void print(const Tuple& t)
    {
        std::cout << std::get<0>(t);
    }
};

template<class... Args>
void print(const std::tuple<Args...>& t)
{
    TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
}

int main() {
    auto v = ReadCSV<int, double, char>("data.csv");
    for (auto& w : v) {
        print(w);
        std::cout << std::endl;
    }
    return 0;
}
