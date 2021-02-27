//
// Created by fail on 2/26/21.
//

#ifndef LAB3_CUSTOMSTRING_H
#define LAB3_CUSTOMSTRING_H

#include <iostream>
#include <cstring>

/*
 * The sole reason for the creation of this is that we are
 * prohibited from using standard containers by assigment rules.
 */

struct customString {
private:
    static const std::size_t DEFAULT_CAPACITY = 16;
    static const std::size_t RESIZE_FACTOR = 2;

    char* m_data = nullptr;
    std::size_t m_size;
    std::size_t m_capacity;

public:
    char* data() { return m_data; }

    [[nodiscard]] std::size_t size() const noexcept { return m_size; }
    [[nodiscard]] std::size_t capacity() const noexcept { return m_size; }

    customString(const char* text) {
        m_capacity = strlen(text);
        m_size = m_capacity;
        m_data = new char[m_capacity];

        for (std::size_t i = 0; i < m_capacity; ++i)
            m_data[i] = text[i];
    }

    customString& operator=(const customString& other) {
        if(this == &other) return *this;
        delete[] m_data;

        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_data = new char[m_capacity];

        for (std::size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];

        return *this;
    }

    customString(const customString& other) {
        m_capacity = other.m_capacity;
        m_size = other.m_size;
        m_data = new char[m_capacity];
        for (std::size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];
    }

    customString(int a) {
        m_capacity = DEFAULT_CAPACITY;
        m_size = 0;

        m_data = new char[m_capacity];

        bool sign = true;
        if (a < 0) {
            sign = false;
            a = -a;
        }

        while (a != 0) {
            char remainder = a % 10;

            m_data[m_size++] = remainder + '0';

            a = a / 10;
        }

        for (std::size_t i = 0; i < m_size / 2; ++i) {
            std::swap(m_data[i], m_data[m_size - i - 1]);
        }

        std::size_t idx = m_size;

        if (!sign) {
            while (idx != 0) {
                m_data[idx + 1] = m_data[idx];
                idx--;
            }
            m_data[1] = m_data[0];

            m_data[0] = '-';
            m_size++;
        }
    }

    [[maybe_unused]] int stoi() const noexcept {
        if (m_size == 0) return 0;

        int out = 0;
        if (m_data[0] == '-') {
            for (std::size_t i = 1; i < m_size; ++i) {
                out = out * 10 + m_data[i] - '0';
            }

            return -out;
        }
        else {
            for (std::size_t i = 0; i < m_size; ++i) {
                out = out * 10 + m_data[i] - '0';
            }

            return out;
        }
    }

    ~customString() noexcept {
        delete[] m_data;
    }

    customString() {
        m_capacity = DEFAULT_CAPACITY;
        m_size = 0;

        m_data = new char[m_capacity];
    }

    char* view() noexcept {
        return m_data;
    }

    bool operator==(const customString& other) const noexcept {
        if (other.m_data == m_data) return true;
        if (!other.m_data && !m_data) return true;
        if (!other.m_data || !m_data) return false;
        if (other.m_size != m_size) return false;

        for (std::size_t i = 0; i < m_size; ++i) {
            if (m_data[i] != other.m_data[i]) return false;
        }

        return true;
    }

    bool operator!=(const customString& other) const noexcept {
        return !(*this == other);
    }

    customString& append(char ch) {
        if (m_size == m_capacity) {
            std::size_t capacity = RESIZE_FACTOR * m_capacity + 1;
            char* data = new char[capacity];

            for (std::size_t i = 0; i < m_size; ++i)
                data[i] = m_data[i];

            delete[] m_data;
            m_data = data;
            m_capacity = capacity;
        }

        m_data[m_size++] = ch;

        return *this;
    }

    char operator[](std::size_t idx) const noexcept { return m_data[idx]; }
    char& operator[](std::size_t idx) noexcept { return m_data[idx]; }

    customString& append(customString str) {
        for (std::size_t i = 0; i < str.size(); ++i) {
            append(str[i]);
        }

        return *this;
    }

    friend std::ostream& operator<< (std::ostream& out, const customString& a) {
        for (std::size_t i = 0; i < a.m_size; ++i)
            out << a.m_data[i];

        return out;
    }

    friend std::istream& operator>> (std::istream& in, customString& a) {
        customString output = "";
        char ch;

        in >> ch;

        while (ch != '\n' && ch != -1) {
            output.append((char)ch);
            ch = in.get();
        }
        a = output;

        return in;
    }

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = char;
        using pointer           = char*;
        using reference         = char&;

        iterator(char* ptr) : m_ptr(ptr) {}

        char& operator*() const { return *m_ptr; }
        char* operator->() { return m_ptr; }

        iterator& operator++() { m_ptr++; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const iterator& a, const iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const iterator& a, const iterator& b) { return a.m_ptr != b.m_ptr; };

    private:

        char* m_ptr;
    };

    iterator begin() { return iterator(&m_data[0]); }
    iterator end() { return iterator(&m_data[m_size]); }
};

namespace std {
    template <>
    struct hash<customString> {
        std::size_t operator() (const customString& other) const {
            std::size_t sz = other.size();
            std::size_t output = 0;
            for(std::size_t i = 0; i < sz; ++i) {
                output = ((output << 5) + output ) ^ other[i];
            }

            return output;
        }
    };
}

#endif //LAB3_CUSTOMSTRING_H
