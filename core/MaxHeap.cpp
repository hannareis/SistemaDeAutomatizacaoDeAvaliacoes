#include "MaxHeap.h"
#include <algorithm>

void MaxHeap::heapifyDown(int i) {
    int maior = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < (int)heap.size() && heap[esq].first > heap[maior].first)
        maior = esq;
    if (dir < (int)heap.size() && heap[dir].first > heap[maior].first)
        maior = dir;

    if (maior != i) {
        std::swap(heap[i], heap[maior]);
        heapifyDown(maior);
    }
}

void MaxHeap::heapifyUp(int i) {
    if (i == 0) return;
    int pai = (i - 1) / 2;
    if (heap[i].first > heap[pai].first) {
        std::swap(heap[i], heap[pai]);
        heapifyUp(pai);
    }
}

void MaxHeap::insert(double media, int projeto_id) {
    heap.push_back({media, projeto_id});
    heapifyUp(heap.size() - 1);
}

bool MaxHeap::empty() const {
    return heap.empty();
}

std::pair<double, int> MaxHeap::extractMax() {
    if (heap.empty()) return {0.0, -1};
    auto max = heap.front();
    heap[0] = heap.back();
    heap.pop_back();
    heapifyDown(0);
    return max;
}

std::vector<std::pair<double, int>> MaxHeap::getSorted() const {
    auto copia = heap;
    std::vector<std::pair<double, int>> ordenado;

    MaxHeap temp = *this;
    while (!temp.empty()) {
        ordenado.push_back(temp.extractMax());
    }
    return ordenado;
}
