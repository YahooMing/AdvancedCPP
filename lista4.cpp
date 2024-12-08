#include  <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <utility>

//ZADANIE 1
namespace cpplab{
    template <typename T>
    class vector{
        public:
            vector(): _size(0), _data(nullptr), _capacity(0){};

            ~vector() { delete[] _data; }


        // Konstruktor kopiujący
        vector(const vector& other) : _size(other._size), _capacity(other._capacity) {
            _data = new T[_capacity];
            for (size_t i = 0; i < _size; ++i) {
                _data[i] = other._data[i];
            }
        }

        // Kopiujący operator kopiujący
        vector& operator=(const vector& other) {
            if (this != &other) {
                delete[] _data;
                _size = other._size;
                _capacity = other._capacity;
                _data = new T[_capacity];
                for (size_t i = 0; i < _size; ++i) {
                    _data[i] = other._data[i];
                }
            }
            return *this;
        }

        // Konstruktor przenoszący
        vector(vector&& other) : _size(other._size), _capacity(other._capacity), _data(other._data) {
            other._size = 0;
            other._capacity = 0;
            other._data = nullptr;
        }

        // Przenoszący operator
        vector& operator=(vector&& other) {
            if (this != &other) {
                delete[] _data;
                _size = other._size;
                _capacity = other._capacity;
                _data = other._data;
                other._size = 0;
                other._capacity = 0;
                other._data = nullptr;
            }
            return *this;
        }

            const size_t size(){
                return _size;
            }

            const size_t cap(){
                return _capacity;
            }

            void alloc(size_t new_cap){
                if(new_cap > _capacity){
                    T* new_data = new T[new_cap];
                    for(size_t i=0; i<_size; i++){
                        new_data[i]=_data[i];
                    }
                    delete[] _data;
                    _data = new_data;
                    _capacity = new_cap;
                }else return;
            }

            void push(const T& v){
                if(_size == _capacity){
                    alloc(_capacity == 0 ? 1 : _capacity * 2);
                }
                _data[_size++] = v;
            }

            void pop(){
                if(_size > 0){
                    _size=_size-1;
                }
            }

            void new_size(size_t new_size) {
                if(new_size > _capacity) {
                    alloc(new_size);
                }
                _size = new_size;
            }

            T& operator[](size_t index) {
                if (index >= _size) {
                    throw std::out_of_range("Poza zasięgiem");
                }
                return _data[index];
            }

            //ZADANIE 2
            template<typename... Args>
            void emplace_back(Args&&... args) {
                if (_size == _capacity) {
                    alloc(_capacity == 0 ? 1 : _capacity * 2);
                }
                new (&_data[_size++]) T(std::forward<Args>(args)...);
            }

        private:
            T _data_type;
            T* _data;
            size_t _size;
            size_t _capacity;
    };

}

enum class pixel {
    r, g, b
};


int main(){
    //ZADANIE 1
	cpplab::vector<int> v1;
    v1.push(1);
    v1.push(2);
    v1.push(3);

	cpplab::vector<int> v2(std::move(v1));
	std::cout << "--------------v1 przeniesiony na v2--------------";
	for (int i = 0; i < v1.size(); i++){
      std::cout << v1[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Rozmiar: " << v1.size() << std::endl;
    std::cout << "Pojemność: " << v1.cap() << std::endl;


	std::cout << "--------------v2 z v1-------------- \n";
	for (int i = 0; i < v2.size(); i++){
      std::cout << v2[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Rozmiar: " << v2.size() << std::endl;
    std::cout << "Pojemność: " << v2.cap() << std::endl;


	v1 = std::move(v2);
	std::cout << "--------------Operator przenoszacy/v2 na v1-------------- \n";
	for (int i = 0; i < v1.size(); i++){
      std::cout << v1[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Rozmiar: " << v1.size() << std::endl;
    std::cout << "Pojemność: " << v1.cap() << std::endl;

	cpplab::vector<int> v3(v1);
	std::cout << "--------------Skopiowanie z v1 na v3-------------- \n";
	for (int i = 0; i < v3.size(); i++){
      std::cout << v3[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Rozmiar: " << v3.size() << std::endl;
    std::cout << "Pojemność: " << v3.cap() << std::endl;

	std::cout << "--------------Skopiowanie z v1 na v3-------------- \n";
	for (int i = 0; i < v2.size(); i++){
      std::cout << v1[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Rozmiar: " << v2.size() << std::endl;
    std::cout << "Pojemność: " << v2.cap() << std::endl;

	v2 = v3;
	std::cout << "--------------Operator kopiujacy/z v3 na v2-------------- \n";
	for (int i = 0; i < v2.size(); i++){
      std::cout << v2[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Rozmiar: " << v2.size() << std::endl;
    std::cout << "Pojemność: " << v2.cap() << std::endl;

	std::cout << "--------------Operator kopiujacy/z v3 na v2-------------- \n";
		for (int i = 0; i < v3.size(); i++){
      std::cout << v3[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Rozmiar: " << v3.size() << std::endl;
    std::cout << "Pojemność: " << v3.cap() << std::endl;



    //ZADANIE 2
    cpplab::vector<pixel> v4;
    v4.emplace_back(pixel::r);
    v4.emplace_back(pixel::g);
    v4.emplace_back(pixel::b);

    std::cout << "--------------Wektor v4 | emplace_back--------------" << std::endl;
    for (int i = 0; i < v4.size(); i++) {
        std::cout << static_cast<int>(v4[i]) << " ";
    }
    std::cout << std::endl;

    return 0;
}