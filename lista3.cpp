
#include  <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>


//ZADANIE 1
template <typename T>
concept koncept = requires (T v){
  std::is_arithmetic_v<T>;
  v[0];
  v.size();
};

template <koncept v1, koncept v2> 
auto operator* (const v1 &w1, const v2 &w2){
	using ResultType = decltype (w1[0]*w2[0]);
	ResultType output = ResultType{};
	for (size_t i = 0; i < w1.size (); i++){
		output += w1[i]*w2[i];
	}
	return output;
}


//ZADANIE 2
template <typename T>
std::vector<T*>
as_sorted_view (std::vector<T> &vec)
{
  std::vector<T*> ptrVec;
  for(auto &el: vec)
    ptrVec.push_back(&el);
  std::sort(ptrVec.begin(), ptrVec.end(), [](const T* a, const T* b) { return *a < *b;});
  return ptrVec;
}

//ZADANIE 3

template <typename T>
class Element {
public:
    Element(T data) : value(data), nextElement(nullptr) {}
    T value;
    std::unique_ptr<Element> nextElement;
};

template <typename T>
class ForwardList {
public:
    std::unique_ptr<Element<T>> firstElement;

    ForwardList() : firstElement(nullptr) {}
    ForwardList(T data) : firstElement(std::make_unique<Element<T>>(data)) {}

    void addElement(T data) {
        if (!firstElement) {
            firstElement = std::make_unique<Element<T>>(data);
        } else {
            Element<T> *current = firstElement.get();
            while (current->nextElement) {
                current = current->nextElement.get();
            }
            current->nextElement = std::make_unique<Element<T>>(data);
        }
    }

    friend std::ostream& operator<<(std::ostream &out, const ForwardList<T> &list) {
        Element<T> *current = list.firstElement.get();
        while (current) {
            out << current->value;
            current = current->nextElement.get();
            if (current) {
                out << " -> ";
            }
        }
        return out;
    }

    void reverse() {
        std::unique_ptr<Element<T>> previous = nullptr;
        std::unique_ptr<Element<T>> current = std::move(firstElement);
        
        while (current) {
            std::unique_ptr<Element<T>> nextNode = std::move(current->nextElement);
            current->nextElement = std::move(previous);
            previous = std::move(current);
            current = std::move(nextNode);
        }
        firstElement = std::move(previous);
    }
};

int main(){

    //ZADANIE 1

    std::vector<int> v1;
    v1.push_back(5);
    v1.push_back(8);
    std::vector<int> v2;
    v2.push_back(2);
    v2.push_back(4);
    std::cout << "ZADANIE 1" << std::endl;

    std::cout << v1*v2 << std::endl;

    //ZADANIE 2

    std::cout << "ZADANIE 2" << std::endl;
    
    std::vector<std::string> z1 = {"zupa","kura","jajo","arbuz","babilon"};
    std::cout << "Nieposortowane: ";
    for(auto &k: z1){
        std::cout << k << ", ";
    }
    std::cout << std::endl << "Posortowane: ";

    std::vector<std::string*> z2 = as_sorted_view(z1);
    for(auto &k: z2){
        std::cout << *k << ", ";
    }
    std::cout << std::endl;


    //ZADANIE 3

    std::cout << "ZADANIE 3" << std::endl;
    ForwardList<int> z3(1);
    z3.addElement(2);
    z3.addElement(3);
    std::cout << "Lista: " << z3 << std::endl;;
    z3.reverse();
    std::cout << "Po odwróceniu: " << z3 << std::endl;

    return 0;
}