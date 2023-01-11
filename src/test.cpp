#include "headers/new_array.hpp"
#include <iostream>
#include <vector>
#include <alloca.h>
#include <memory_resource>
#include <memory>
#include <cstring>
#include <algorithm>
#include <stdlib.h>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <bitset>
#include <thread>
#include <memory_resource>
#include <atomic>
// super fast and optimized array manipulation functions
// inspired by the incredible std::vector, but way faster
// and using a power of two instead of a generic size

template <typename T>
struct array
{
private:
    [[nodiscard]] std::allocator<T> _alloc;
    int _capacity = 64;
    int _size = 0;
    T *_data = _alloc.allocate(_capacity);

public:
    /// @brief Default capacity of the array

    array()
    {
        //_data = _alloc.allocate(_capacity);
    }
    /*array(T *d)
    {
        _data = new T[sizeof(T) * 5];

       _data = std::move(d);
       delete d;
    }*/
    constexpr inline T *data() const
    {
        return this->_data;
    }

    array(std::allocator_arg_t args)
    {
    }

    array(int size)
    {
        this->_capacity = size;
        // _data = _alloc.allocate(_capacity);
    }

    array(const array<T> &&__data)
    {
        _size = __data._size;
        this->_capacity = __data._capacity;
        _data = _alloc.allocate(_capacity);
        memcpy(this->_data, __data.data(), this->_capacity * sizeof(T));
    }

    array(array<T> &rhs)
    {
        this->_size = rhs._size;
        this->_capacity = rhs._capacity;
        _data = _alloc.allocate(_capacity);
        memcpy(this->_data, rhs.data(), this->_capacity * sizeof(T));
    }

    array(std::initializer_list<T> __data)
    {
        this->_size = __data.size();
        _data = _alloc.allocate(_capacity);
        memcpy(this->_data, __data.begin(), __data.end() - __data.begin());
    }

    ~array()
    {
    }

    void set_capacity(int cap)
    {
        T *cpy_data = new T[this->_capacity];
        // memcpy(cpy_data, this->_data, this->_capacity * sizeof(T));
        _alloc.deallocate(_data, 1);
        this->_capacity = cap;
        _data = _alloc.allocate(_capacity);
        this->_data = std::ref(cpy_data);
        delete[] cpy_data;
    }
    /*
        array<T> &operator=(const array<T> &other) noexcept
        {
            // Guard self assignment
            if (this == &other)
                return *this;

            // assume *this manages a reusable resource, such as a heap-allocated buffer mArray
            if (this->_capacity != other._capacity) // resource in *this cannot be reused
            {
                _alloc.deallocate(this->_data, this->_capacity); // release resource in *this
                _data = nullptr;
                // size = 0;                     // preserve invariants in case next line throws
                _data = _alloc.allocate(other._capacity); // allocate resource in *this
                _capacity = other._capacity;
                _size = other._size;
            }

            std::copy(other._data, other._data + sizeof(other), this->_data);
            return *this;
        }

        array<T> &operator=(array<T> &&other) noexcept
        {
            // Guard self assignment
            if (this == &other)
                return *this; // delete[]/size=0 would also be ok
            delete[] _data;
            this->_data = std::exchange(other._data, nullptr); // leave other in valid state
            this->_capacity = std::exchange(other._capacity, 0);
            this->_size = std::exchange(other._size, 0);
            return *this;
        }*/

    // copy assignment (copy-and-swap idiom)
    array<T> &operator=(array<T> other) noexcept // call copy or move constructor to construct other
    {
        std::swap(this->_size, other._size); // exchange resources between *this and other
        std::swap(this->_data, other._data);
        std::swap(this->_capacity, other._capacity);
        return *this;
    } // destructor of other is called to release the resources formerly managed by *this

    T &operator[](int idx)
    {
        std::cout << "called index" << std::endl;
        if (this->_data == nullptr || idx > this->size())
            throw std::invalid_argument("[array] data, or data index not valid");
        return this->_data[idx * sizeof(T)];
    }

    constexpr int &size()
    {
        return this->_size;
    }

    void push_back(const T &value)
    {
        if (this->_size > this->_capacity)
        {
            set_capacity(this->_capacity + 1024);
        }

        _data[this->_size] = value;
        _size++;
    }
    void push_back(T &&value)
    {
        if (this->_size + 1 > this->_capacity)
        {
            set_capacity(this->_capacity + 1024);
        }
        //*(this->_data + (this->_size * sizeof(T))) = value;
        // this->_data[this->_size] = value;
        _alloc.construct(this->_data[this->_size], value);

        //
        _size++;
    }

    void push_range(array<T> value)
    {
        if (this->_size + value.size() > this->_capacity)
        {
            set_capacity(this->_capacity + value.size() + 1024); // allocate the expected size plus 5 more
            memcpy(this->_data[this->_size], value.data(), value.size() * sizeof(T));
        }
        else
            throw std::runtime_error("[array]Push range fail.");
    }
};


std::atomic<bool> sw = false;
void test_my_array()
{
        check:
    while(!sw.load())
        goto check;
    new_array<new_array<double>> test(UINT16_MAX);
    for (auto i = 0; i < UINT16_MAX; i++)
    {
        test[i] = {1.};
    }

    std::cout << test[0] << std::endl; // 0.001
    test[0] = 0.25;
    std::cout << test[0] << std::endl; // 0.002

    std::cout << "finished my vec" << std::endl;
}

void test_std_array()
{
    check:
    while(!sw.load())
        goto check;

    std::vector<std::vector<double>> test{};
    for (auto i = 0; i < UINT64_MAX; i++)
    {
        test.push_back(1.);
    }

    std::cout << test[0] << std::endl; // 0.001
    test[0] = 0.25;
    std::cout << test[0] << std::endl; // 0.002

    std::cout << "finished std vec" << std::endl;
}

int main()
{
    sw.store(true);
    std::thread{test_std_array}.detach(); // 0.012s on UINT64
    //std::thread{test_my_array}.detach(); // 0.008s on UINT64
    //sw.store(true);
}
