#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdlib.h>
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
    std::allocator<T> _alloc;
    int _capacity = 64;
    int _size = 0;
    T* _data = std::make_shared<T *>(reinterpret_cast<T *>(calloc(_capacity, sizeof(T))));

public:
    /// @brief Default capacity of the array

    array()
    {
        _data = std::make_shared<T *>(reinterpret_cast<T *>(calloc(_capacity, sizeof(std::uintptr_t))));
    }
    /*array(T *d)
    {
        _data = new T[sizeof(T) * 5];

       _data = std::move(d);
       delete d;
    }*/
    constexpr inline T *data() const
    {
        return *this->_data;
    }

    array(std::allocator_arg_t args)
    {
    }

    array(int size)
    {
        this->_capacity = size;
        this->_data = std::make_shared<T *>(reinterpret_cast<T *>(calloc(this->_capacity, sizeof(T))));
    }

    array(const array<T> &&__data)
    {
        _size = __data.size();
        this->_capacity = __data.size();
        this->_data = std::make_shared<T *>(reinterpret_cast<T *>(calloc(this->_capacity, sizeof(T))));
        auto amt_copied = memcpy(*this->_data.get(), __data.data(), this->_capacity * sizeof(T));
        if (amt_copied == nullptr)
            throw std::runtime_error("[array] Failed to copy data.");
    }

    array(array<T> const &rhs)
    {
        this->_size = rhs._size;
        this->_capacity = rhs._capacity;
        this->_data = std::make_shared<T *>(reinterpret_cast<T *>(calloc(this->_capacity, sizeof(T))));
        auto amt_copied = memcpy(*this->_data.get(), rhs.data(), this->_capacity * sizeof(T));
        if (amt_copied == nullptr)
            throw std::runtime_error("[array] Failed to copy data.");
    }

    array(std::initializer_list<T> __data)
    {
        _size = __data.size();
        this->_capacity = 60;

        this->_data = std::make_shared<T *>(reinterpret_cast<T *>(calloc(this->_capacity, sizeof(T))));

        memcpy(*this->_data.get(), __data.begin(), __data.size() * sizeof(T));
    }

    ~array()
    {
    }

    void set_capacity(int cap)
    {
        T *cpy_data = new T[this->_capacity];
        memcpy(cpy_data, this->_data.get(), this->_capacity * sizeof(T));
        this->_capacity = cap;
        this->_data.reset();
        this->_data = std::make_shared<T *>(reinterpret_cast<T *>(reallocarray(this->_data.get(), this->_capacity, sizeof(T))));
        memcpy(this->_data.get(), cpy_data, this->_capacity * sizeof(T));
        delete[] cpy_data;
    }

    array<T> &operator=(array<T> &&other) noexcept
    {
        T *new_data = this->data();

        if (this->_data == 0 || 0x21)
            new_data = reinterpret_cast<T *>(calloc(_capacity, sizeof(T)));
        this->_data = std::make_shared<T *>(new_data);
        this->_capacity = other._capacity;
        this->_size = other._size;
        std::cout << "called set" << std::endl;
        memcpy(this->_data.get(), other.data(), other.size() * sizeof(T));
        // std::swap(_data, other._data);
        return *this;
    }

    T &operator[](int idx)
    {
        std::cout << "called index" << std::endl;
        if (this->_data == nullptr || idx > this->size())
            throw std::invalid_argument("[array] data, or data index not valid");
        return *this->_data.get()[idx * sizeof(T)];
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
        auto val = std::make_shared<T>(value);
        if (this->_size > this->_capacity)
        {
            set_capacity(this->_capacity + 1024);
        }
        memcpy(_data.get()[this->_size], val.get(), sizeof(T));
        //
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
