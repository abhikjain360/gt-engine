#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <optional>

template <typename T>
class cached_array;

class index_keeper;

template <typename T>
class resizeable_array {
public:
    /* Constructors */
    constexpr resizeable_array() noexcept
        : deg(0), cap(2), ptr(std::make_unique<T[]>(2)) {}

    constexpr resizeable_array(const size_t capacity) noexcept
        : deg(0), cap(capacity), ptr(std::make_unique<T[]>(cap)) {
        assert(capacity > 0);
    }

    resizeable_array(std::unique_ptr<T[]> p, const size_t size)
        : deg(size), cap(size), ptr(std::move(p)) {
        assert(ptr != nullptr && size > 0);
    }

    resizeable_array(resizeable_array<T>& arr) {
        assert(arr.ptr != nullptr);

        deg = arr.deg;

        // cap is currently a random number as uninitalized, need to set 0
        cap = 0;
        this->resize(arr.cap);

        for (size_t i = 0; i < arr.cap; ++i) ptr[i] = arr.ptr[i];
    }

    /* Move Contuctor */
    resizeable_array(resizeable_array<T>&& arr)
        : deg(arr.deg), cap(arr.cap), ptr(std::move(arr.ptr)) {
        assert(ptr != nullptr);
    }

    /* Getters */
    constexpr size_t degree() const noexcept { return deg; }
    constexpr size_t capacity() const noexcept { return cap; }

    /* to manage only with pop & push */
    void push(const T& t) {
        if (deg >= cap) this->resize(cap + 1);
        ptr[deg++] = t;
    }

    std::optional<T> pop() {
        if (deg > 0) return ptr[--deg];
        return {};
    }

    /* editting the array values */
    void put(const size_t index, const T& t) {
        if (index >= cap) this->resize(index + 1);
        ptr[index] = t;
        ++deg;
    }

    void remove(const size_t index, const T& t = 0) {
        assert(index < cap);
        ptr[index] = t;
        --deg;
    }

    const T& operator[](const size_t index) const noexcept {
        assert(index < cap);
        return ptr[index];
    }

    T& operator[](const size_t index) noexcept {
        assert(index < cap);
        return ptr[index];
    }

    void empty() noexcept { deg = 0; }

    /* for assignments */
    resizeable_array<T>& operator=(resizeable_array<T>&& arr) {
        // setting values of the LHS
        deg = arr.deg;
        cap = arr.cap;
        ptr = std::move(arr.ptr);

        return *this;
    }

    resizeable_array<T>& operator=(const resizeable_array<T>& arr) {
        deg = arr.deg;
        cap = arr.cap;
        for (size_t i = 0; i < cap; ++i) ptr[i] = arr.ptr[i];

        return *this;
    }

    constexpr bool operator==(const resizeable_array<T>& arr) const noexcept {
        return arr.ptr.get() == ptr.get();
    }
    constexpr bool operator==(const T* p) const noexcept { return p == ptr.get(); }

    constexpr bool operator!=(const resizeable_array<T>& arr) const noexcept {
        return arr.ptr.get() != ptr.get();
    }
    constexpr bool operator!=(const T* p) const noexcept { return p != ptr.get(); }

    template <typename Compare = bool(const T&, const T&)>
    void sort(Compare compare = [](const T& a, const T& b) -> bool { return a > b; }) {
        std::sort(ptr.get(), ptr.get() + cap, compare);
    }

private:
    size_t deg, cap;
    std::unique_ptr<T[]> ptr;

    /* utility functon */
    constexpr size_t next2(size_t size) const noexcept {
        size_t lt = 2;
        while (lt <= size) lt <<= 1;
        return lt;
    }

    constexpr size_t prev2(size_t size) const noexcept {
        size_t lt = 2;
        while (lt <= size) lt <<= 1;
        lt >>= 1;
        return lt;
    }

    /* change the size of the array */
    void resize(const size_t size) {
        // resize should not delete existing info
        if (size <= cap) return;

        size_t lt = next2(size);

        this->trim(lt);
    }

    void trim(const size_t size) {
        // reallocation most thread safe
        auto temp = std::make_unique<T[]>(size);

        if constexpr (std::is_nothrow_move_assignable_v<std::unique_ptr<T[]>>) {
            std::move(ptr.get(), ptr.get() + cap, temp.get());
        } else {
            std::copy(ptr.get(), ptr.get() + cap, temp.get());
        }

        ptr = std::move(temp);
        cap = size;

        return;
    }

    // these need access to
    friend class cached_array<T>;
    friend class index_keeper;
};
