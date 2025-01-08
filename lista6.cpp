#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>


class thread_pool {
private:
    std::vector<std::thread> slaves; // wektor wątków
    std::queue<std::function<double()>> tasks; //zadanka
    std::mutex queue_mutex; //kolejka żeby nie zepsuły sie zasoby dzielone między wątkami
    std::condition_variable condition; //słuzy do tego, żeby wątek wiedział kiedy są nowe zadania albo kiedy koniec
    std::atomic<bool> stop_flag; //zatrzymuje wątki
    std::vector<double> results; // wyniki
    std::mutex results_mutex; //wektor wyników

public:
    explicit thread_pool(size_t threads) : stop_flag(false) { //konstruktor / od razu mówimy że rozpoczeta praca
        for (size_t i = 0; i < threads; ++i) { //tworzymy wątki
            slaves.emplace_back([this] {
                while (true) { //każdy wątek działa w pętli i czeka na nowe zadanka w kolejce
                    std::function<double()> task;

                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { return stop_flag || !tasks.empty(); }); //czekanko na zadanie
                        if (stop_flag && tasks.empty())
                            return;

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    double result = task();

                    {
                        std::lock_guard<std::mutex> lock(results_mutex);
                        results.push_back(result);
                    }
                }
            });
        }
    }

    ~thread_pool() {
        stop();
    }

    void add_task(std::function<double()> task) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex); //dodawanko taska
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

    double average() {
        std::lock_guard<std::mutex> lock(results_mutex);
        if (results.empty()) {
            return 0.0;
        }

        double sum = 0.0;
        for (double result : results) {
            sum += result;
        }
        return sum / results.size();
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            stop_flag = true;
        }
        condition.notify_all(); //powiadamianie że trzeba konczyć thready

        for (std::thread &worker : slaves) { //czekanko na zakończenie pracy
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
};

int main(){
    // ZADANIE 1
    std::cout << "-------------ZADANIE 1-------------" << std::endl;
    std::vector<int> v1(10);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "Wektor v1 przed posortowaniem: " << std::endl;
    for (int& val : v1) {
        val = std::rand() % 100;
        std::cout << val << " ";
    }
    std::cout << std::endl;

    auto srd = v1.begin() + v1.size() / 2;

    std::thread p1(std::bind(std::sort<std::vector<int>::iterator>, v1.begin(), srd));
    std::thread p2(std::bind(std::sort<std::vector<int>::iterator>, srd, v1.end()));

    p1.join();
    p2.join();

    // Jak zrobie inplace merga to wtedy sie posortuje idealnie ale bez niego nie ma szans na posortowanie dając każdemu wątkowi po jednej połowie
    //std::inplace_merge(v1.begin(), srd, v1.end());

    std::cout << "Wektor v1 po posortowaniu: " << std::endl;
    for (int& val : v1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;


    std::cout << "-------------ZADANIE 2-------------" << std::endl;
    thread_pool pool(4);

    pool.add_task([]() { return 42.0; });
    pool.add_task([]() { return 13.0; });
    pool.add_task([]() { return 7.0; });
    pool.add_task([]() { return 21.0; });

    std::this_thread::sleep_for(std::chrono::seconds(1)); //trzeba dać czas na zakończenie zadania

    std::cout << "Średnia: " << pool.average() << std::endl;

    pool.stop();


    return 0;
}
