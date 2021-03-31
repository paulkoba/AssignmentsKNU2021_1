//
// Created by fail on 2/24/21.
//

#ifndef LAB3_CUSTOMUNORDEREDMAP_H
#define LAB3_CUSTOMUNORDEREDMAP_H

#include "customStack.h"
#include <algorithm>

static const std::size_t DEFAULT_CAPACITY = 16;
static const std::size_t RESIZE_FACTOR = 2;
static const std::size_t RESIZE_TRIGGER = 2;

/*
 * The sole reason for the creation of this is that we are
 * prohibited from using standard containers by assigment rules.
 */

template<typename Key, typename Value, typename Hasher = std::hash<Key>>
struct customUnorderedMap {
    customStack<customStack<std::pair<Key, Value>>> data = customStack<customStack<std::pair<Key, Value>>>(DEFAULT_CAPACITY);
    std::size_t currentCapacity = DEFAULT_CAPACITY;
    std::size_t currentSize = 0;

    customUnorderedMap() noexcept = default;
    ~customUnorderedMap() noexcept = default;
    customUnorderedMap(const customUnorderedMap<Key, Value, Hasher>& other) noexcept = default;
    customUnorderedMap(customUnorderedMap<Key, Value, Hasher>&& other) noexcept = default;
    customUnorderedMap& operator=(const customUnorderedMap<Key, Value, Hasher>&) noexcept = default;

    [[nodiscard]] std::size_t size() const noexcept { return currentSize; }
    [[nodiscard]] std::size_t resizeCapacity() const noexcept { return currentCapacity * RESIZE_TRIGGER; }

    void resize() noexcept {
        customStack<customStack<std::pair<Key, Value>>> newData =
                customStack<customStack<std::pair<Key, Value>>>(currentCapacity * RESIZE_FACTOR);

        currentCapacity = currentCapacity * RESIZE_FACTOR;
        for(auto& node : data) {
            for(auto& p : node) {
                size_t h = Hasher()(p.first) % currentCapacity;
                newData[h].push(p);
            }
        }

        data = newData;
    }

    Value& operator[](Key key) noexcept {
        std::size_t h = Hasher()(key) % currentCapacity;

        for(std::pair<Key, Value>& out : data[h]) {
            if(out.first == key) return out.second;
        }

        ++currentSize;

        data[h].push({key, Value()});

        if(currentSize >= currentCapacity * RESIZE_TRIGGER) resize();

        return this->operator[](key);
    }

    bool contains(Key key) noexcept {
        std::size_t h = Hasher()(key) % currentCapacity;

        for(std::pair<Key, Value>& out : data[h]) {
            if(out.first == key) return true;
        }

        return false;
    }

    /*
     *  Const iterators not implemented.
     */

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::pair<Key, Value>;
        using pointer           = std::pair<Key, Value>*;
        using reference         = std::pair<Key, Value>&;

        explicit iterator(int _h, int _i, customStack<customStack<std::pair<Key, Value>>>& _data) noexcept
                : h(_h), i(_i), data(_data) {}

        std::pair<Key, Value>& operator*() const noexcept { return data[h][i]; }
        std::pair<Key, Value>* operator->() noexcept { return &data[h][i]; }
        explicit operator std::pair<Key, Value>*() noexcept { return &data[h][i]; };

        iterator& operator++() noexcept {
            ++i;
            while(data.size() > h && i >= data[h].currentSize()) {
                ++h;
                i = 0;
            }
            return *this;
        }

        friend bool operator== (const iterator& a, const iterator& b) noexcept {
            return a.i == b.i && a.h == b.h && &a.data == &b.data;
        };

        friend bool operator!= (const iterator& a, const iterator& b) noexcept { return !(a == b); };

        int h = 0, i = 0;
        customStack<customStack<std::pair<Key, Value>>>& data;
    };

    iterator begin() noexcept {
        auto iter = iterator(0, 0, data);

        while(data.size() > iter.h && iter.i >= data[iter.h].currentSize()) {
            ++iter.h;
        }

        return iter;
    }
    iterator end() noexcept { return iterator(currentCapacity, 0, data); }
};

#endif //LAB3_CUSTOMUNORDEREDMAP_H