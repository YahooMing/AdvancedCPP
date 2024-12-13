#include  <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>
#include <concepts>

//ZADANIE 1
// nie jestem dobry z tłumaczenia więc staram się nie tłumaczyć ale lubie mieć zapisane to jak coś działa
template <typename T>
class BST{
    struct node{
        T data; // wartość węzła
        std::unique_ptr<node> left; // wskaźnik do lewego węzła (mniejszego)
        std::unique_ptr<node> right; // wskaźnik do prawego węzła (większego)
        node* parent; // wskaźnik do rodzica

        node(T val, node* par=nullptr) : data(val), parent(par){}
    };

    std::unique_ptr<node> root; // wskaźnik na korzeń (sama góra)

    // funkcja pomocnicza do tego żeby wypisać wartości węzłów w kolejności lewy, parent, prawy
    void print_in_order(const std::unique_ptr<node>& n) const {
        if (n) {
            print_in_order(n->left);
            std::cout << n->data << " ";
            print_in_order(n->right);
        }
    }
public:
    // funkcja żeby łatwo wstawić wartość do drzewa, a jeśli wstawiamy pierwszą wartość to to jest root
    void insert(T val) {
        if (!root) {
            root = std::make_unique<node>(val);
        } else {
            insert(val, root.get());
        }
    }

    // insert który rekurencyjnie wstawia wartość do dobrego miejsca na drzewie
    void insert(T val, node* n) {
        if (val < n->data) {
            if (n->left) {
                insert(val, n->left.get());
            } else {
                n->left = std::make_unique<node>(val, n);
            }
        } else {
            if (n->right) {
                insert(val, n->right.get());
            } else {
                n->right = std::make_unique<node>(val, n);
            }
        }
    }

    // publiczne wywołanie tego printa dla korzenia i pozostałcch węzłów
    void print_in_order() const {
        print_in_order(root);
        std::cout << std::endl;
    }

};

//ZADANIE 2a
//robiłem z filmiku tego co jest w src listy zadań, ale tam nie ma wszystkich funkcjonalności (albo ja przegapiłem)
namespace cpplab{
    template <typename Contained>
    class unique_ptr {
    public:
        constexpr void reset() {
            delete data;
            data = nullptr;
        }

        constexpr unique_ptr() = default;
        constexpr ~unique_ptr() { reset(); }
        constexpr unique_ptr(Contained *data_) : data(data_) {}

        //operacja przeniesiena
        constexpr unique_ptr(unique_ptr &&other) : data(std::exchange(other.data, nullptr)) {}
        constexpr unique_ptr &operator=(unique_ptr &&other) {
            if (this != &other) {
                reset();
                data = std::exchange(other.data, nullptr);
            }
            return *this;
        }

        unique_ptr(const unique_ptr &) = delete;
        unique_ptr &operator=(const unique_ptr &) = delete;

        //operator dereferencji
        Contained& operator*() const {
            return *data;
        }

        //operator strzałki
        Contained* operator->() const {
            return data;
        }

        //operatory porównania z nullptr
        bool operator==(std::nullptr_t) const {
            return data == nullptr;
        }

        bool operator!=(std::nullptr_t) const {
            return data != nullptr;
        }

    private:
        Contained *data = nullptr;
    };
}

//ZADANIE 2b

// niestety nie zrobiłem tego używając conceptów ponieważ nie działa niestety :(

template <typename T>
class non0_ptr {
public:
    // raw ptr
    explicit non0_ptr(T* ptr) : ptr_(ptr) {
        if (!ptr_) {
            throw std::invalid_argument("non0_ptr nie analizujemy z nullptr");
        }
    }

    non0_ptr(const non0_ptr& other) : ptr_(other.ptr_) {}

    non0_ptr(non0_ptr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    non0_ptr& operator=(const non0_ptr& other) {
        if (this != &other) {
            ptr_ = other.ptr_;
        }
        return *this;
    }

    non0_ptr& operator=(non0_ptr&& other) noexcept {
        if (this != &other) {
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T& operator*() const {
        assert(ptr_ != nullptr && "Dereferencing a nullptr");
        return *ptr_;
    }

    T* operator->() const {
        assert(ptr_ != nullptr && "Accessing a nullptr");
        return ptr_;
    }

    T* get() const noexcept {
        return ptr_;
    }


    bool operator==(std::nullptr_t) const = delete;
    bool operator!=(std::nullptr_t) const = delete;

private:
    T* ptr_;
};



//ZADANIE 3
// Knapsack problem to sie chyba nazywało i było na którejś matematyce, postaram się wyjaśnić jak to zostało zrobine
int maxTreasureValue(int N, int W, const std::vector<int>& weights, const std::vector<int>& values) {
    // N = liczba skarbów
    // W = max waga jaką może unieść plecak
    // weights = wektor z wagami rzeczy do podniesienia
    // values = wektor z wartościami przedmiotów
    // wiadomo, indeks w wektorze values i wektorze weights obest_outputowiada temu samemu przedmiotowi
    std::vector<std::vector<int>> best_output(N + 1, std::vector<int>(W + 1, 0));
    // best_output to troche taka tablica dwuwymiarowa, do liczby skarbów (wierszy) dodajemy 1 żeby był uwzględniony przypadek jak nie bierzemy nic
    // do liczby kolumn (W) dodajemy 1 żeby mieć przypadek że plecak jest tak mały że nie możemy nic zmieścić

    // dla każdego skarbu i każdej możliwej wagi robimy po pętli for
    for (int i = 1; i <= N; ++i) {
        for (int w = 1; w <= W; ++w) {
            // jeśli waga skarbu "i" jest mniejsza lub równa W to wtedy dodajemy ten element lub nie dodajemy bierzącego skarbu
            if (weights[i - 1] <= w) {
                best_output[i][w] = std::max(best_output[i - 1][w], best_output[i - 1][w - weights[i - 1]] + values[i - 1]);
            } else {
                // jeśli waga skarbu "i" jest większa to best_output pozostaje taki sam jak poprzednio
                best_output[i][w] = best_output[i - 1][w];
            }
        }
    }
    // zwracamy elegancko output
    return best_output[N][W];
}

int main(){
    //ZADANIE 1
    std::cout << "----------ZADANIE 1------------" << std::endl;
    BST<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);

    tree.print_in_order();

    //ZADANIE 2a
    std::cout << "----------ZADANIE 2a------------" << std::endl;
    //Ale jazda, dopiero sie dowiedziałem że w C i C++ można definiować scope za pomocą {}. Polecam do testów i od teraz będę tego używał często bo przydatne
    // Test konstrukcji i destrukcji
    {
        cpplab::unique_ptr<int> ptr(new int(5));
        std::cout << "wartość ptr: " << *ptr << std::endl;
    }

    // Test resetu
    {
        cpplab::unique_ptr<int> ptr(new int(5));
        std::cout << "wartość ptr przed reset: " << *ptr << std::endl;
        ptr.reset();
        std::cout << "ptr po reset: " << (ptr == nullptr ? "nullptr" : "nie nullptr") << std::endl;
    }

    // Test przenoszenia
    {
        cpplab::unique_ptr<int> ptr1(new int(5));
        cpplab::unique_ptr<int> ptr2 = std::move(ptr1);
        std::cout << "ptr1 po przeniesieniu: " << (ptr1 == nullptr ? "nullptr" : "nie nullptr") << std::endl;
        std::cout << "wartość ptr2: " << *ptr2 << std::endl;
    }

    // Test przypisania przenoszącego
    {
        cpplab::unique_ptr<int> ptr1(new int(5));
        cpplab::unique_ptr<int> ptr2;
        ptr2 = std::move(ptr1);
        std::cout << "ptr1 po przeniesieniu: " << (ptr1 == nullptr ? "nullptr" : "nie nullptr") << std::endl;
        std::cout << "wartość ptr2: " << *ptr2 << std::endl;
    }

    //ZADANIE 2b
    //nie mogłem zrobić nullptr używając conceptów :(
    std::cout << "----------ZADANIE 2b------------" << std::endl;

    int x = 10;
    non0_ptr<int> ptr(&x);

    std::cout << "Wartość: " << *ptr << std::endl;
    *ptr = 20;
    std::cout << "Nowa wartość: " << *ptr << std::endl;
    try {
        non0_ptr<int> ptr222(nullptr);
    } catch (const std::invalid_argument& e) { //można catchować gdy wie sie co sie catchuje, prawda?
        std::cerr << "Błąd: " << e.what() << std::endl;
    }



    //ZADANIE 3
    std::cout << "----------ZADANIE 3------------" << std::endl;
    int N, W;
    N = 10;
    W = 10;
    std::cout << "Liczba skarbów: " << N << std::endl << "Maks. waga: " << W << std::endl;


    std::vector<int> weights(N);
    std::vector<int> values(N);

    std::cout << "Wagi skarbów: ";
    for (int i = 0; i < N; ++i) {
        weights[i] = std::rand() % 5 + 1; //losowo pomiędzy 1 a 5
        std::cout << weights[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Wartości skarbów: ";
    for (int i = 0; i < N; ++i) {
        values[i] = std::rand() % 10 + 1; //losowo pomiędzy 1 a 10
        std::cout << values[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Maksymalna wartość, jaką można uzyskać, to: " << maxTreasureValue(N, W, weights, values) << std::endl;
    //https://augustineaykara.github.io/Knapsack-Calculator/ --- tym sprawdzałem czy wygląda dobrze wynik i zgadzało sie, ale ja sie nie znam, nie ukrywam

    return 0;
}

//jezu wreszcie koniec listy 5, ja chce już pythona zobaczyć na oczy. Na uczelni tylko C++, w pracy C, nie ma nigdzie pythona a to taki piękny język
//wiem że to nie jest niczyja wina że tak jest, ale kurcze no tęsknie za tymi chwilami z pythonem bardziej niż za dziewczyną