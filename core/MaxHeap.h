#pragma once
#include <vector>
#include <utility>

class MaxHeap {
private:
    std::vector<std::pair<double, int>> heap; // (média, id_projeto)

    void heapifyDown(int i);
    void heapifyUp(int i);

public:
    void insert(double media, int projeto_id);
    bool empty() const;
    std::pair<double, int> extractMax();
    std::vector<std::pair<double, int>> getSorted() const;
};
