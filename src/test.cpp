#include <iostream>
#include <vector>
#include <memory>
#include <memory.h>
#include <algorithm>
#include <stdexcept>
#include <utility>
// super fast and optimized array manipulation functions
// inspired by the incredible std::vector, but way faster
// and using a power of two instead of a generic size

template <typename T>
class array
{
private:
    T *_data;
    int _size = 0;
    int _capacity = 0;

public:
    /// @brief Default capacity of the array

    array()
    {
        _data = new T[_capacity];
    }
    array(T *d)
    {
        _data = new T[sizeof(T) * 5];
    }
    constexpr array<T> data()
    {
        return array(this->_data);
    }

    array(int size)
    {
        this->_data = new T[size]{};
        this->_capacity = size;
    }

    array(const array<T> &&__data)
    {
        this->_capacity = sizeof(T) + (sizeof(T) * __data.size());
        _data = new T[this->_capacity]{__data._data};
        memset(&_data, 1, _capacity);
    }
    array(std::initializer_list<T> __data)
    {
        array<T> &&__local(__data.size());
        for (auto i = 0; i < __data.size(); i++)
        {
            __local[i] = *(__data.begin() + i);
        }
        array(__local._data);
    }

    ~array()
    {
        if (_data != nullptr)
            std::free(_data);
    }

    void set_capacity(int cap)
    {
        this->_capacity = cap;
        auto& cpy_data = _data;
        _data = reinterpret_cast<T*>(malloc(_capacity));
        memcpy(_data, cpy_data, this->_capacity);
    }

    void set_size(int amt)
    {
        this->_size = amt;
    }
    array<T> &operator=(array<T> &&val) noexcept
    {
        if (this == &val)
            return *this;

        delete[] this->data;
        data = std::exchange(val.data, nullptr); // leave other in valid state
        _size = std::exchange(val._size, 0);
        return *this;
    }
    // copy assignment (copy-and-swap idiom)
    array<T> &operator=(array<T> &other) noexcept // call copy or move constructor to construct other
    {
        std::swap(_size, other._size); // exchange resources between *this and other
        std::swap(_data, other._data);
        return *this;
    } // destructor of other is called to release the resources formerly managed by *this

    // mem cleanup
    void operator delete[](void *data)
    {
        std::free(data);
        data = nullptr;
    }

    T &operator[](int idx)
    {
        return this->_data[idx];
    }

    constexpr int &size()
    {
        return this->_size;
    }

    void push_back(T &value)
    {
        if (this->_size + sizeof(value) > this->_capacity)
        {
            set_capacity(this->_capacity + sizeof(value));
        }

        data[this->_size] = value;
        this->_size++;
    }

    void push_back(T value)
    {
        if (this->_size + value.size() > this->_capacity)
        {
            set_capacity(this->_capacity + value.size() + (sizeof(value) * 5)); // allocate the expected size plus 5 more
        }

        _data[this->_size] = value;
        this->_size++;
    }
};

int main()
{
    array<array<double>> test{};
    std::vector<double> test2{};

    for (auto i = 0; i < 10; i++)
    {
        test.push_back({i * 0.25});
    }
}