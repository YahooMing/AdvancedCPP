#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
#include <type_traits>



template <typename T>
    typename std::enable_if<std::is_same<T, std::string>::value, std::vector<std::string>>::type
insertion_sort(std::vector<T> wektor) {
    // funkcja do konwersji stringa na jego wartość liczbową
    auto to_numeric_value = [](const std::string& str) {
        std::string numeric_string;
        for (char c : str) {
            if (std::isdigit(c)) {
                numeric_string += c; // jeśli to cyfra, dodaj do tymczasowego stringa
            } else {
                numeric_string += std::to_string(static_cast<int>(c));
            }
        }
        return std::stoll(numeric_string);
    };

    std::sort(wektor.begin(), wektor.end(), [&](const T& a, const T& b) {
        return to_numeric_value(a) < to_numeric_value(b);
    });

    return wektor;
}

void print_all(){} //na zakończenie rekurencji

template<typename T, typename... Args>
void print_all(T first, Args... arguments){
    std::cout << first << std::endl;
    print_all(arguments...);
};

class Factorial{
    public:
    template <int T> constexpr int factorial(){
        if constexpr ( T == 0 ){
            return 1;
        } else {
            return T * factorial<T-1>();
        }
    }
};

int main(){
    //zadanie 1
    std::cout << "===========Zadanie 1===========" << std::endl;
    std::vector<std::string> wektor = {"aa23bb", "aa267bb", "aa1bb","bb2aa","aa235bb","ca3cf","aa5bb"};
    std::vector<std::string> posortowany = insertion_sort(wektor);
    for (const auto& str : posortowany) {
        std::cout << str << std::endl;
    }
    //zadanie 2
    std::cout << "===========Zadanie 2===========" << std::endl;
    Factorial z2;
    std::cout << z2.template factorial<5>() << std::endl;

    //zadanie 3
    std::cout << "===========Zadanie 3===========" << std::endl;
    print_all(1, 1.0, 1.0f, "Hello");
    print_all(1, 2.0);
    return 0;
}