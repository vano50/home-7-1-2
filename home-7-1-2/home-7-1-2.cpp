#include <iostream>
#include <vector>
#include <chrono>
#include <thread>


void generateRandomVector(std::vector<int>& v, int size)
{
    for (int i = 0; i < size; ++i)
        v.push_back(rand() % 10000);
}

void sum_vectors(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        result[i] = a[i] + b[i];
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::cout << "Количество аппаратных ядер - " << std::thread::hardware_concurrency() << std::endl;


    int size_vector[] = { 1000, 10000, 100000, 1000000 };
    std::cout << "             " << size_vector[0] << "          " << size_vector[1] << "          " << size_vector[2] << "          " << size_vector[3] << std::endl;
    int num_threads[] = { 2, 4, 8, 16 };

    for (auto num_thread : num_threads) {
        std::cout << "Потоки: " << num_thread << "    ";

        for (const auto size : size_vector) {


            std::vector<int> vectorA;
            std::vector<int> vectorB;
            std::vector<std::thread> threads;

            generateRandomVector(vectorA, size);
            generateRandomVector(vectorB, size);

            std::vector<int> vector_result(size);

            if (num_thread > size) {
                num_thread = size;
            }

            size_t part_of_size = size / num_thread;

            for (size_t i = 0; i < num_thread; ++i) {
                size_t start = i * part_of_size;
                size_t end = (i == num_thread - 1) ? size : start + part_of_size;
                threads.emplace_back(sum_vectors, std::ref(vectorA), std::ref(vectorB), std::ref(vector_result), start, end);
            }

            auto start = std::chrono::steady_clock::now();

            for (auto& thread : threads) {
                thread.join();
            }

            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> elapsed_milliseconds = end - start;
            std::cout << elapsed_milliseconds.count() << " мс" << "    ";
        }
        std::cout << std::endl;
    }

    return 0;
}
