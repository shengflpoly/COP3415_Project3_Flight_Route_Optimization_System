#include "MinHeap.hpp"
#include <iostream>

template <typename T>
MinHeap<T>::MinHeap() {

}

template <typename T>
MinHeap<T>::MinHeap(const std::vector<T>& v) : data(v) {
    heapify();
}

template <typename T>
void MinHeap<T>::insert(const T& val) {
    data.push_back(val);
    percolateUp();
}

template <typename T>
void MinHeap<T>::print() const {
    for (int i = 0; i < data.size(); i++) {
        std::cout << data[i] << " ";
    }
    std::cout << '\n';
}

template <typename T>
void MinHeap<T>::percolateUp() {
    int child_idx = data.size() - 1;
    int parent_idx = (child_idx - 1) / 2;
    while (child_idx > 0 && data[child_idx] < data[parent_idx]) {
        std::swap(data[child_idx], data[parent_idx]);
        child_idx = parent_idx;
        parent_idx = (child_idx - 1) / 2;
    }
}

template <typename T>
int MinHeap<T>::getLeftKidIndex(int i) {
    return i * 2 + 1;
}
template <typename T>
int MinHeap<T>::getRightKidIndex(int i) {
    return i * 2 + 2;
}
template <typename T>
int MinHeap<T>::getParentIndex(int i) {
    return (i - 1) / 2;
}
template <typename T>
int MinHeap<T>::getLastWithKidsIndex() {
    return data.size() / 2 - 1;
}

template <typename T>
T MinHeap<T>::deleteMin() {
    if (data.empty()) {
        throw std::logic_error("deleteMin: empty heap");
    }
    T res = data[0];
    data[0] = data[data.size() - 1]; // Set the last element as the root
    data.pop_back(); // Remove the last element
    
    percolateDown();

    return res;
}

template <typename T>
void MinHeap<T>::percolateDown(int i) {
    int index = i;
    int size = data.size();

    while (getLeftKidIndex(index) < size) {
        int left = getLeftKidIndex(index);
        int right = getRightKidIndex(index);

        int smallest = left;
        // Has 2 kids
        if (right < size && data[right] < data[left]) {
            smallest = right;
        }
        if (data[index] < data[smallest]) {
            break;
        }
        std::swap(data[index], data[smallest]);
        index = smallest;
    }
}

template <typename T>
void MinHeap<T>::heapify() {
    for (int i = getLastWithKidsIndex(); i >= 0; i--) {
        percolateDown(i);
    }
}

template <typename T>
bool MinHeap<T>::empty() const {
    return data.empty();
}