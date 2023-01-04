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
struct array
{
private:
    T *_data = reinterpret_cast<T *>(malloc(sizeof(T) * _capacity));
    int _capacity = 64;
    int _size = 0;

public:
    /// @brief Default capacity of the array

    array()
    {
        _data = reinterpret_cast<T *>(malloc(sizeof(T) * _capacity));
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
        delete[] this->_data;
    }

    void set_capacity(int cap)
    {
        T *cpy_data = new T[this->_capacity];
        memcpy(cpy_data, this->_data, this->_capacity * sizeof(T));
        this->_capacity = cap;
        this->_data = reinterpret_cast<T *>(realloc(this->_data, this->_capacity * sizeof(T)));
        memcpy(this->_data, cpy_data, this->_capacity * sizeof(T));
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
        if(!this->_data)
            this->_data = malloc(other._capacity);
        std::cout << "called set" << std::endl;
        memcpy(this->_data, other.data(), other.size() * sizeof(T));
        // std::swap(_data, other._data);
        return *this;
    } // destructor of other is called to release the resources formerly managed by *this

    T &operator[](int idx)
    {
        std::cout << "called index" << std::endl;
        if (this->_data == nullptr || idx > this->size())
            throw std::invalid_argument("[array] data, or data index not valid");
        return this->_data[idx];
    }

    constexpr int &size()
    {
        return this->_size;
    }

    void push_back(T value)
    {
        if (this->_size + 1 > this->_capacity)
        {
            set_capacity(this->_capacity + 1024);
        }
        _data[this->_size] = value;
        _size++;
    }

    void push_back(T &value)
    {
        if (this->_size > this->_capacity)
        {
            set_capacity(this->_capacity + 1024);
        }

        _data[this->_size] = value;
        _size++;
    }

    void push_range(array<T> value)
    {
        if (this->_size + value.size() > this->_capacity)
        {
            set_capacity(this->_capacity + value.size() + 1024); // allocate the expected size plus 5 more
            memcpy(this->_data + (sizeof(T) * this->_size), value.data(), value.size() * sizeof(T));
        }
        else
            throw std::runtime_error("[array]Push range fail.");
    }
};

int main()
{
    array<array<double>> test{};
    std::cout << "array size: " << sizeof(test) << std::endl;
    std::vector<std::vector<double>> test2{};

    for (auto i = 0; i < UINT16_MAX; i++)
    {
        test.push_back({1});
        // test2.push_back({i * 0.25});
    }

    std::cout << test[0][0] << std::endl; // 0.001
    // std::cout << test2[0][0] << std::endl; // 0.002

    test[0] = {0.25};

    std::cout << test[0][0] << std::endl; // 0.002
}
