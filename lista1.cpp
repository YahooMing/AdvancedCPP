#include <iostream>
#include <vector>

using namespace std;

//w komentarzach postaram się opisać jak wszystko rozumiem
template <typename T, typename C> //szablon jest po to, żeby pisać funkcje lub klasy dla różnych typów danych.
//T i C to parametry szablonu, gdzie T to typ danych a C to funkcja używana do porównywania
T my_max(T a, T b, C comp){ //funkcja zwracająca wartości typu T
    return comp(a, b) ? a : b; //zwraca i wywołuje komparator, jeśli prawda zwraca A jeśli fałsz zwraca B
}

template <typename T>
vector<T> insertion_sort(vector<T> wektor) { //trzeba pamiętać że funkcja zwraca kopie wektora, więc ten wektor którego "wkładamy" bedzie niezmieniony
    if(wektor.size() > 1){ //jeśli ma więcej niż jeden element
        for(int i = 1; i < wektor.size(); i++) { //przejeżdżamy po wszystkich elementach od drugiego
            T tmp = wektor[i]; //tmp jest do przechowywania bieżącego elementu który zostanie wstawiony na odpowiednie miejsce
            int j = i - 1; //j jest do kolejnej iteracji przez wektor, zawsze mniejszy niż tmp

            while(j >= 0 && wektor[j] > tmp) {
                wektor[j + 1] = wektor[j]; //przesuwamy elemenmty coraz wyżej aż nie dojdziemy do tmp
                j--;
            }
            wektor[j + 1] = tmp;
        }
    }
    return wektor;
}

namespace cpplab{ //przestrzeń nazw
    template <typename T>
    class vector{
        public:
            vector(): _size(0), _data(nullptr), _capacity(0){}; //konstruktor który nie alokuje żadnej pamięci (chyba), skoro wszystkie prywatne zmienne są wyzerowane. Nie jestem pewien co do size i capacity ale data na bank jest wyzerowana

            ~vector() { delete[] _data; } //destruktor

            const size_t size(){
                return _size;
            }

            void alloc(size_t new_cap){ //żeby "zwiększyć limity"
                if(new_cap > _capacity){ //sprawdzamy czy nowa pojemność jest większa od starej
                    T* new_data = new T[new_cap]; //nowa tablica danych powinna mieć rozmiar nowej pojemności
                    for(size_t i=0; i<_size; i++){ //for odpowiadający za kopiowanie danych do nowej tablicy danych
                        new_data[i]=_data[i];
                    }
                    delete[] _data; //usuwam stare dane bo po co mi to
                    _data = new_data; //nadpisuje dane jako nowe dane
                    _capacity = new_cap; //i nadpisuje starą pojemność do nowej
                }else return; //nic nie zwracam bo alloc ma typ void
            }
            //w treści zadania jest napisane "Klasa powinna wspierać dynamiczne dodawaniei i usuwanie elementów oraz zmiane długości" i to teraz robie
            void push(const T& v){ //do dodawania
                if(_size == _capacity){ //czy aktualny rozmiar jest wielkości max?
                    alloc(_capacity == 0 ? 1 : _capacity * 2); //jeśli tak to tworze nowe miejsce z funkcji alloc, po prostu jeśli cap jest 0 to powiększam do 1 a jeśli jest inne to powiększam dwa razy
                }
                _data[_size++] = v; //dodaje nowy element do danych
            }

            void pop(){
                if(_size > 0){ //zwyczajnie usuwam ostatni element poprzez ograniczenie długosci wektora
                    _size=_size-1;
                }
            }

            void new_size(size_t new_size) {
                if(new_size > _capacity) { //jeśli jest taka chęć to powiększam długość wektora
                    alloc(new_size);
                }
                _size = new_size;
            }

            T& operator[](size_t index) { //przeciążam operator []
                if (index >= _size) {
                    throw out_of_range("Poza zasięgiem");
                }
                return _data[index];
            }




        private:
            T _data_type;
            T* _data;
            size_t _size;
            size_t _capacity;
    };

}
            //zadanie3c
            template <typename V, typename W>
            auto operator*(W& pierwszy_wektor ,V& inny_wektor){
                if(pierwszy_wektor.size() != inny_wektor.size()){
                    throw out_of_range("Oba wektory muszą mieć te samą długość");
                }

                auto ret = pierwszy_wektor[0] * inny_wektor[0];
                for (size_t i=1; i< pierwszy_wektor.size(); i++){
                    ret = ret + pierwszy_wektor[i] * inny_wektor[i];
                }
                return ret;
            }

int main(){
    //zadanie1
    cout << "Zadanie 1" << endl;
    auto comp = [](auto a, auto b){ return a > b; };
    cout << my_max(1, 2, comp) << endl;
    cout << my_max(22, 103, comp) << endl;
    cout << my_max("a", "b", comp) << endl;
    cout << my_max(2.1, 2.2 , comp) << endl;
    float num1 = 3E-5f;
    float num2 = 3E-6f;
    cout << my_max(num1, num2 , comp) << endl;

    //zadanie2
    cout << "Zadanie 2" << endl;
    vector<double> v1({ 1.2, 1.5, 1.1, 0.9});
    vector<char> v2({'j','c','k','e','b'});

    auto t1 = insertion_sort(v1);
    for(auto &x : t1){
        cout << x ;
    }

    //zadanie3
    cout << endl << "Zadanie 3" << endl;
    cpplab::vector<int> v;
    cpplab::vector<char> w8;
    v.push(1);
    v.push(2);
    v[1]=4;
    w8.push('a');
    w8.push('b');
    w8.push('c');

    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
    v.pop();
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
    for (size_t i = 0; i < w8.size(); ++i) {
        cout << w8[i] << " ";
    }
    cout << endl;

    //zadanie3c
    cout << "Zadanie 3c" << endl;
    v.push(3);
    vector<int> v3;
    v3.push_back(5);
    v3.push_back(8);

    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
    for (size_t i = 0; i < v3.size(); ++i) {
        cout << v3[i] << " ";
    }
    cout << endl;
    cout << v * v3 << endl;
    cout << v3 * v << endl;


    return 0;

}

