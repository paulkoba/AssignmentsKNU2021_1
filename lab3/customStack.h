//
// Created by fail on 2/24/21.
//

#ifndef LAB3_CUSTOMSTACK_H
#define LAB3_CUSTOMSTACK_H

#pragma once

#include <iterator>
#include <cstddef>

/*
 * The sole reason for the creation of this is that we are
 * prohibited from using standard containers by assigment rules.
 */

template<typename T>
struct customStack {
private:
    static const std::size_t DEFAULT_CAPACITY = 16;
    static const std::size_t RESIZE_FACTOR = 2;

    T* m_data = nullptr;
    std::size_t m_size;
    std::size_t m_capacity;

public:
    [[nodiscard]] std::size_t size() const noexcept { return m_size; }
    [[nodiscard]] std::size_t capacity() const noexcept { return m_size; }

    explicit customStack(std::size_t capacity) noexcept {
        m_capacity = capacity;
        m_size = capacity;

        m_data = new T[m_capacity]();
    }

    customStack(const customStack& other) noexcept {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_data = new T[m_capacity];

        for (std::size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];
    }

    customStack& operator=(const customStack& other) noexcept {
        if(this == &other) return *this;

        delete[] m_data;

        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_data = new T[m_capacity];

        for (size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];

        return *this;
    }

    customStack() noexcept {
        m_capacity = DEFAULT_CAPACITY;
        m_size = 0;
        m_data = new T[m_capacity];
    }

    ~customStack() noexcept {
        delete[] m_data;
    }

    void push(T elem) noexcept {
        if (m_size == m_capacity) {
            std::size_t capacity = RESIZE_FACTOR * m_capacity + 1;
            T* data = new T[capacity];

            for (std::size_t i = 0; i < m_size; ++i)
                data[i] = m_data[i];

            delete[] m_data;
            m_data = data;
            m_capacity = capacity;
        }

        m_data[m_size++] = elem;
    }

    T pop() noexcept {
        return m_data[--m_size];
    }

    void swapRemove(std::size_t i) noexcept {
        --m_size;
        std::swap(m_data[m_size], m_data[i]);
    }

    T peek() noexcept {
        return m_data[m_size - 1];
    }

    T* view() noexcept {
        return m_data;
    }

    T& operator[](std::size_t idx) noexcept { return m_data[idx]; }

    [[nodiscard]] bool empty() const noexcept {
        return m_size == 0;
    }

    /*
     *  Const iterators not implemented.
     */

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        explicit iterator(T* ptr) : m_ptr(ptr) {}

        T& operator*() const noexcept { return *m_ptr; }
        T* operator->() noexcept { return m_ptr; }
        explicit operator T*() noexcept { return m_ptr; };

        iterator& operator++() noexcept { m_ptr++; return *this; }
        iterator operator++(int) noexcept { iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; };

    private:

        T* m_ptr;
    };

    iterator begin() noexcept { return iterator(&m_data[0]); }
    iterator end() noexcept { return iterator(&m_data[m_size]); }
};

#endif //LAB3_CUSTOMSTACK_H
