//#include <memory>
//
// struct test {
//    int A;
//    float B;
//};
//
// template <unsigned int N>
// struct dosomething {
//    std::unique_ptr<test[]> arr = std::make_unique<test[]>(N);
//
//    void increase_capacity(size_t size) {
//        auto temp = std::make_unique<test[]>(size);
//        for (auto i = 0; i < size; ++i) {
//            temp[i] = arr[i];
//        }
//        arr = std::move(temp);
//    }
//};
//

#include <algorithm>
#include <memory>
#include <type_traits>

struct test {
    int A;
    float B;
};

template <unsigned int N>
struct dosomething {
    std::unique_ptr<test[]> arr = std::make_unique<test[]>(N);

    void increase_capacity(size_t size) {
        auto temp = std::make_unique<test[]>(size);
        if constexpr (std::is_nothrow_move_assignable_v<test>) {
            std::move(arr.get(), arr.get() + size, temp.get());
        } else {
            std::copy(arr.get(), arr.get() + size, temp.get());
        }

        arr = std::move(temp);
    }
};

int main() { dosomething<3>().increase_capacity(3); }

/*
Hi! I have a unique pointer of an array of struct, and I am writing a function
to increase the size of struct array the pointer is pointing to. The way I do it
is create a new unique pointer, copy all the elements, and then move it to
original one
*/
