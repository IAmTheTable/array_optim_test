#include <iostream>
#include <vector>
#include <memory>
#include <memory.h>
#include <algorithm>
// super fast and optimized array manipulation functions
// inspired by the incredible std::vector, but way faster
// and using a power of two instead of a generic size

template <typename T>
class array : public std::iterator_traits<T>
{
    const int _capacity = 64;
    T* data = new T[_capacity];
    int _size = 0;
    public:
    /// @brief Default capacity of the array

    array()
    {
        auto a = std::realloc(data, sizeof(T) * _capacity);
        auto d = reinterpret_cast<T*>(a);
        data = d;
    }

    array(std::initializer_list<T> _data)
    {
        data = reinterpret_cast<T*>(std::realloc(&data, sizeof(T) * _data.size()));
    }
    ~array()
    {
        std::free(data);
    }
    array<T> operator=(array<T>& val)
    {
        this->data = val.data;
        this->_size = val.size();
    }

    // mem cleanup
    void operator delete[](void* data)
    {
        std::free(data);
    }

    constexpr int& size()
    {
        return this->_size;
    }

    void push_back(T&& value)
    {
        if(this->_size > this->size())
            throw std::runtime_error("[array] Invalid size at push_back");
        
        data[this->_size] = value;
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