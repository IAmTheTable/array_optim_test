#include <iostream>
#include <vector>
#include <memory>
#include <memory.h>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <bit>
// super fast and optimized array manipulation functions
// inspired by the incredible std::vector, but way faster
// and using a power of two instead of a generic size

template <typename T>
class array
{
private:
    T *_data = new T[64];
    int _capacity = 64;
    int _size = 0;

public:
    /// @brief Default capacity of the array

    array()
    {
        _data = new T[_capacity];
    }
    /*array(T *d)
    {
        _data = new T[sizeof(T) * 5];
       _data = std::move(d);
       delete d;
    }*/
    constexpr T *data()
    {
        return this->_data;
    }

    array(int size)
    {
        this->_capacity = size;
        this->_data = malloc(this->_capacity * sizeof(T));
    }

    array(const array<T> &&__data)
    {
        _size = __data.size();
        this->_capacity = __data.size();
        this->_data = reinterpret_cast<T *>(malloc(this->_capacity * sizeof(T)));
        auto amt_copied = memcpy(this->_data, __data.data(), this->_capacity * sizeof(T));
        if (amt_copied == nullptr)
            throw std::runtime_error("[array] Failed to copy data.");
    }
    array(std::initializer_list<T> __data)
    {
        _size = __data.size();
        this->_capacity = __data.size();

        this->_data = reinterpret_cast<T *>(malloc(this->_capacity * sizeof(T)));
        memcpy(this->_data, __data.begin(), this->_capacity * sizeof(T));
    }

    ~array()
    {
        std::free(this->_data);
    }

    void set_capacity(int cap)
    {
        this->_capacity = cap;
        auto cpy_data = this->_data;
        auto new_data = reinterpret_cast<T *>(realloc(this->_data, this->_capacity * sizeof(T)));
        this->_data = new_data;
        memcpy(this->_data, &cpy_data, this->_capacity * sizeof(T));
    }

    void set_size(int amt)
    {
        this->_size = amt;
    }
    array<T> &operator=(array<T> &&val) noexcept
    {
        if (this == &val)
            return *this;

        memcpy(this->_data, val.data(), sizeof(T) * val.size()); // leave other in valid state
        return *this;
    }
    // copy assignment (copy-and-swap idiom)
    array<T> &operator=(array<T> &other) noexcept // call copy or move constructor to construct other
    {
        std::swap(_data, other._data);
        return *this;
    } // destructor of other is called to release the resources formerly managed by *this

    T &operator[](int idx)
    {
        if (this->_data == nullptr || idx > this->size())
            throw std::invalid_argument("[array] data, or data index not valid");
        return this->_data[idx];
    }

    constexpr int &size()
    {
        return this->_capacity;
    }

    void push_back(T value)
    {
        if (this->_size > this->_capacity)
        {
            set_capacity(this->_capacity);
        }
        _data[this->_size] = value;
        _size++;
    }

    void push_back(T &value)
    {
        if (this->_size > this->_capacity)
        {
            set_capacity(this->_capacity);
        }

        _data[this->_size] = value;
        _size++;
    }

    void push_range(array<T> value)
    {
        if (this->_size + value.size() > this->_capacity)
        {
            set_capacity(this->_capacity + value.size() + (sizeof(value) * 5)); // allocate the expected size plus 5 more
            memcpy(this->_data, value.data(), value.size() * sizeof(T));
        }
        else
            throw std::runtime_error("[array]Push range fail.");
    }
};

int main()
{
    array<array<double>> test{};
    std::vector<std::vector<double>> test2{};

    for (auto i = 0; i < 10; i++)
    {
        test.push_back({1});
        // test2.push_back({i * 0.25});
    }

    std::cout << test[0][0] << std::endl; // 0.001
    // std::cout << test2[0][0] << std::endl; // 0.002

    test[0] = {0.25};

    std::cout << test[0][0] << std::endl; // 0.002
}