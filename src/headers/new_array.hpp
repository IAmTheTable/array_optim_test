// create a memory efficient container class
// inspired by the std::vector

#include <memory>
#include <memory.h>
#include <cstdint>
#include <initializer_list>
template <typename T>
struct new_array
{
    std::uint32_t _capacity = 1;
    T* _data;

    new_array()
    {
        this->_data = new T[_capacity];
    }

    new_array(std::uint32_t capacity)
    {
        this->_capacity = capacity;
        this->_data = new T[_capacity];
    }

    new_array(new_array<T> const& other)
    {
        this->_capacity = other._capacity;
        this->_data = new T[this->_capacity];
        memcpy(this->_data, other._data, sizeof(T) * this->_capacity);
    }

    new_array(new_array<T> &&other)
    {
        this->_capacity = other._capacity;
        this->_data = new T[this->_capacity];
        memcpy(this->_data, other._data, sizeof(T) * this->_capacity);
    }

    new_array(std::initializer_list<T> __data)
    {
        this->_capacity = __data.size();
        this->_data = new T[__data.size()];
        memcpy(this->_data, __data.begin(), sizeof(T) * __data.size());
    }

    ~new_array()
    {
        delete[] this->_data;
    }

    new_array<T>& operator=(new_array<T> const& other)
    {
        if (this == &other)
        {
            return *this;
        }
        this->_capacity = other._capacity;
        this->_data = new T[this->_capacity];
        memcpy(this->_data, other._data, sizeof(T) * this->_capacity);
        return *this;
    }

    new_array<T>& operator=(new_array<T> &&other)
    {
        if (this == &other)
        {
            return *this;
        }
        this->_capacity = other._capacity;
        this->_data = new T[this->_capacity];
        memcpy(this->_data, other._data, sizeof(T) * this->_capacity);
        return *this;
    }

    T& operator[](const std::uint32_t index)
    {
        return this->_data[index];
    }
};