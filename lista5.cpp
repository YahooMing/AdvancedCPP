#include  <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <utility>
#include <concepts>

//ZADANIE 1
template <typename T>
class BST{
    struct node{
        T data;
        std::unique_ptr<node> left;
        std::unique_ptr<node> right;
        node* parent;

        node(T val, node* par=nullptr) : data(val), parent(par){}
    };

    std::unique_ptr<node> root;

    void print_in_order(const std::unique_ptr<node>& n) const {
        if (n) {
            print_in_order(n->left);
            std::cout << n->data << " ";
            print_in_order(n->right);
        }
    }
public:
    void insert(T val) {
        if (!root) {
            root = std::make_unique<node>(val);
        } else {
            insert(val, root.get());
        }
    }

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

    void print_in_order() const {
        print_in_order(root);
        std::cout << std::endl;
    }

};

//ZADANIE 2
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
//ZADANIE 3
// Funkcja obliczająca maksymalną wartość skarbów, które można zabrać
int maxTreasureValue(int N, int W, const std::vector<int>& weights, const std::vector<int>& values) {
    // Tworzymy 2-wymiarową tablicę do przechowywania maksymalnej wartości dla każdego podproblemu
    std::vector<std::vector<int>> dp(N + 1, std::vector<int>(W + 1, 0));

    // Wypełniamy tablicę dp
    for (int i = 1; i <= N; ++i) {
        for (int w = 1; w <= W; ++w) {
            // Sprawdzamy, czy możemy dodać bieżący skarb do plecaka
            if (weights[i - 1] <= w) {
                // Maksymalna wartość to maksimum z:
                // 1. Wartości bez bieżącego skarbu
                // 2. Wartości z bieżącym skarbem (dodajemy jego wartość do wartości dla pozostałej wagi)
                dp[i][w] = std::max(dp[i - 1][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            } else {
                // Jeśli nie możemy dodać bieżącego skarbu, to wartość pozostaje taka sama jak bez niego
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Maksymalna wartość, jaką można uzyskać przy N skarbach i limicie wagi W
    return dp[N][W];
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

        //ZADANIE 2
    std::cout << "----------ZADANIE 2------------" << std::endl;
    //Ale jazda, dopiero sie dowiedziałem że w C i C++ można definiować scope za pomocą {}. Polecam do testów i od teraz będę tego używał często bo przydatne
    // Test konstrukcji i destrukcji
    {
        cpplab::unique_ptr<int> ptr(new int(5));
        std::cout << "Wartość ptr: " << *ptr << std::endl;
    }

    //Test reset
    {
        cpplab::unique_ptr<int> ptr(new int(5));
        std::cout << "Wartość ptr przed reset: " << *ptr << std::endl;
        ptr.reset();
        std::cout << "ptr po reset: " << (ptr == nullptr ? "nullptr" : "nie nullptr") << std::endl;
    }

    //Test przenoszenia
    {
        cpplab::unique_ptr<int> ptr1(new int(5));
        cpplab::unique_ptr<int> ptr2 = std::move(ptr1);
        std::cout << "ptr1 po przeniesieniu: " << (ptr1 == nullptr ? "nullptr" : "nie nullptr") << std::endl;
        std::cout << "Wartość ptr2: " << *ptr2 << std::endl;
    }

    //Test przypisania przenoszącego
    {
        cpplab::unique_ptr<int> ptr1(new int(5));
        cpplab::unique_ptr<int> ptr2;
        ptr2 = std::move(ptr1);
        std::cout << "ptr1 po przeniesieniu: " << (ptr1 == nullptr ? "nullptr" : "nie nullptr") << std::endl;
        std::cout << "Wartość ptr2: " << *ptr2 << std::endl;
    }


    //ZADANIE 3
    int N, W;
    N = 10;
    W = 10;
    std::cout << "----------ZADANIE 3------------" << std::endl << "Liczba skarbów: " << N << std::endl << "Maks. waga: " << W << std::endl;

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