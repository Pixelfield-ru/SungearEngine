#ifndef FLAT_ARRAY_H
#define FLAT_ARRAY_H

#include <SGCore/pch.h>

template<class T>
struct reverse_wrapper
{
    T& p;
    reverse_wrapper(T& x) :p(x)
    {}

    auto begin() { return p.rbegin(); }
    auto end() { return p.rend(); }
};


template<class T>
reverse_wrapper<T> reverse_order(T& x)
{
    return reverse_wrapper<T>(x);
}


template<class Element, bool direction>
class IteratorClass
{
    Element* p;
    std::size_t index;

public:
    typedef std::forward_iterator_tag iterator_category;

    IteratorClass(Element *f, std::size_t index1) : p(f), index(index1)
    {
    }

    IteratorClass& operator++()
    {
        if (direction)
        {
            ++index;
        }
        else
        {
            --index;
        }
        return  *this;
    }

    IteratorClass operator++(int)
    {
        std::size_t index1 = index;
        ++(*this);
        return IteratorClass(p, index1);
    }

    Element& operator*()
    {
        return p[index];
    }

    const Element& operator*() const
    {
        return p[index];
    }

    Element* operator->()
    {
        return &p[index];
    }

    friend bool operator==(const IteratorClass& a, const IteratorClass& b)
    {
        return (&a.p[a.index] == &b.p[b.index]);
    }

    friend bool operator!=(const IteratorClass& a, const IteratorClass& b)
    {
        return (&a.p[a.index] != &b.p[b.index]);
    }

    static IteratorClass begin(Element* x, std::size_t length)
    {
        if (direction)
        {
            return IteratorClass(x, 0);
        }
        else
        {
            return IteratorClass(x, length - 1);
        }
    }

    static IteratorClass end(Element* x, std::size_t length)
    {
        if (direction)
        {
            return IteratorClass(x, length);
        }
        else
        {
            return IteratorClass(x, static_cast<std::size_t>(-1));
        }
    }
};

class FlatArrayException : public std::exception
{
    std::string m_message;
public:
    FlatArrayException(const std::string& message) :m_message("FLAT ARRAY EXCEPTION: " + message)
    {
    }

    virtual char const* what() const noexcept
    {
        return m_message.c_str();
    }
};

template<class ...T>
struct Dimensions
{
    using dimension_index = int;
    using size_type = std::size_t;

    dimension_index m_dim = static_cast<dimension_index>(1 + sizeof...(T));
    size_type *m_dimensions = new size_type[m_dim];
    size_type m_dataSize;

    template<class... T1>
    void SetSizes(dimension_index pos, size_type m1, T1 ...k1)
    {
        if (pos == 0)
        {
            m_dataSize = 1;
        }
        m_dimensions[pos] = m1;
        m_dataSize *= m1;

        if constexpr (sizeof...(k1) > 0)
        {
            SetSizes(pos + 1, k1...);
        }
    }

    Dimensions(std::size_t size0, T... sizes)
    {
        SetSizes(0, size0, sizes ...);
    }

    ~Dimensions()
    {
        delete[] m_dimensions;
    }
};


template <class Elem, int Dim, bool AllocationFlag = true>
class flat_array
{
public:
    using size_type = std::size_t;
    using dimension_index = int;

private:
    friend class flat_array<Elem, Dim - 1, true>;
    friend class flat_array<Elem, Dim - 1, false>;

    friend class flat_array<Elem, Dim + 1, true>;
    friend class flat_array<Elem, Dim + 1, false>;
    friend class flat_array<Elem, Dim, !AllocationFlag>;


    size_type *m_dimensions;
    size_type m_dataSize;
    Elem *m_data;

    int output_width = 10;

    bool m_owner = true;

    template<class... T>
    void SetSizes(dimension_index pos, size_type m1, T...k1)
    {
        if (pos == 0)
        {
            m_dataSize = 1;
        }
        m_dimensions[pos] = m1;
        m_dataSize *= m1;

        if constexpr (sizeof...(k1) > 0)
        {
            SetSizes(pos + 1, k1...);
        }
    }

    void SetSizes(const std::vector<size_type>& sizes1)
    {
        m_dataSize = 1;
        m_dimensions = new size_type[Dim];
        for (dimension_index i = 0; i < Dim; i++)
        {
            size_type m1 = sizes1[i];
            m_dimensions[i] = m1;
        }
    }

    template<class... T>
    size_type GetIndex(dimension_index pos, size_type prevIndex, size_type m1, T...k1) const
    {
        size_type index = (prevIndex* m_dimensions[pos]) + m1;

        if constexpr (sizeof...(k1) > 0)
        {
            return GetIndex(pos + 1, index, k1...);
        }
        else
        {
            return index;
        }
    }

    template<bool AllocationFlag2>
    flat_array(const flat_array<Elem, Dim + 1, AllocationFlag2>& s, size_type p)
    {
        m_dataSize = s.m_dataSize / s.m_dimensions[0];
        m_dimensions = s.m_dimensions + 1;
        size_type m = 1;
        for (dimension_index i = 0; i < Dim; i++)
        {
            m *= m_dimensions[i];
        }
        m_data = s.m_data + p * m;
    }

public:

    flat_array()
    {
        m_dimensions = new size_type[Dim];
        m_dataSize = 0;
        m_data = nullptr;
    }

    ~flat_array()
    {
        if constexpr (AllocationFlag)
        {
            delete[] m_dimensions;
            if (m_owner)
            {
                delete[] m_data;
            }
        }
    }


    flat_array(std::initializer_list<Elem> list)
    {
        m_dimensions = new size_type[Dim];
        m_dataSize = 0;
        m_data = nullptr;

        if (list.size() == 0)
        {
            return;
        }

        m_dataSize = list.size();
        size_type i = 0;
        m_dimensions[0] = m_dataSize;
        m_data = new Elem[m_dataSize];
        for (const Elem& a : list)
        {
            m_data[i++] = a;
        }
    }

    flat_array(const std::vector<Elem>& list)
    {
        m_dimensions = new size_type[Dim];
        m_dataSize = 0;
        m_data = nullptr;

        if (list.size() == 0)
        {
            return;
        }

        m_dataSize = list.size();
        size_type i = 0;
        m_dimensions[0] = m_dataSize;
        m_data = new Elem[m_dataSize];
        for (const Elem& a : list)
        {
            m_data[i++] = a;
        }
    }

    flat_array(std::initializer_list<flat_array<Elem, Dim - 1>> list)
    {
        m_dimensions = new size_type[Dim];
        if (list.size() == 0)
        {
            m_dataSize = 0;
            m_data = nullptr;
            return;
        }

        size_type n = list.size();
        size_type counter = 0;
        for (const flat_array<Elem, Dim - 1>& x : list)
        {
            if (counter == 0)
            {
                m_dimensions[0] = n;
                std::copy(x.m_dimensions, x.m_dimensions + Dim - 1, m_dimensions + 1);
            }
            counter += x.m_dataSize;
        }

        m_dataSize = counter;

#ifdef _DEBUG
        {
            size_type total = 1;
            for (size_type j = 0; j < Dim; j++)
            {
                total *= m_dimensions[j];
            }
            if (total != m_dataSize)
            {
                std::ostringstream ss;
                ss << "Initialization. Dimensions are not compatible with total size: " << m_dataSize;
                throw ss;
            }
        }
#endif
        m_data = new Elem[m_dataSize];

        counter = 0;
        for (const flat_array<Elem, Dim - 1>& x : list)
        {
            for (size_type i = 0; i < x.m_dataSize; i++)
            {
                m_data[counter++] = x.m_data[i];
            }
        }
    }

    template<class... T>
    flat_array(size_type m1, T... k1)
    {
        m_dimensions = new size_type[Dim];
        SetSizes(0, m1, k1...);
        m_data = new Elem[m_dataSize];
        for (size_type i = 0; i < m_dataSize; i++)
        {
            m_data[i] = Elem();
        }
    }

    flat_array(const std::vector<size_type>& s)
    {
        m_dimensions = new size_type[Dim];
        SetSizes(s);
        m_data = new Elem[m_dataSize];
        for (size_type i = 0; i < m_dataSize; i++)
        {
            m_data[i] = Elem();
        }
    }

    template<class ...T>
    flat_array(const Dimensions<T...>& s, Elem* data)
    {
        m_dimensions = new size_type[Dim];
        std::copy(s.m_dimensions, s.m_dimensions + Dim, m_dimensions);
        m_dataSize = s.m_dataSize;
        m_owner = false;
        m_data = data;
    }

    template<class ...T>
    flat_array(Dimensions<T...>&& s, Elem* data)
    {
        m_dimensions = nullptr;
        std::swap(m_dimensions, s.m_dimensions);
        m_dataSize = s.m_dataSize;
        m_owner = false;
        m_data = data;
    }


    template<class...T>
    void resize(size_type m1, T...k1)
    {
        if constexpr (AllocationFlag)
        {
            if (m_owner)
            {
                SetSizes(0, m1, k1...);
                delete[] m_data;
                m_data = new Elem[m_dataSize];
                for (size_type i = 0; i < m_dataSize; i++)
                {
                    m_data[i] = Elem();
                }
            }
            else
            {
                throw FlatArrayException("cannot resize a non-owner");
            }
        }
        else
        {
            throw FlatArrayException("cannot resize a non-owner");
        }
    }

    flat_array(const flat_array& x)
    {
        if constexpr (AllocationFlag)
        {
            m_dimensions = new size_type[Dim];
            m_dataSize = x.m_dataSize;
            m_data = new Elem[m_dataSize];
            std::copy(x.m_data, x.m_data + m_dataSize, m_data);
            std::copy(x.m_dimensions, x.m_dimensions + Dim, m_dimensions);
        }
        else
        {
            m_dimensions = x.m_dimensions;
            m_dataSize = x.m_dataSize;
            m_data = x.m_data;
        }
    }

    template<bool copy2>
    flat_array(const flat_array<Elem, Dim, copy2>& x)
    {
        if constexpr (AllocationFlag)
        {
            m_dimensions = new size_type[Dim];
            m_dataSize = x.m_dataSize;
            m_data = new Elem[m_dataSize];

            std::copy(x.m_data, x.m_data + m_dataSize, m_data);
            std::copy(x.m_dimensions, x.m_dimensions + Dim, m_dimensions);
        }
        else
        {
            m_dimensions = x.m_dimensions;
            m_dataSize = x.m_dataSize;
            m_data = x.m_data;
        }
    }

    flat_array(flat_array&& x)
    {
        m_dimensions = x.m_dimensions;
        m_dataSize = x.m_dataSize;
        m_data = x.m_data;

        x.m_dataSize = 0;
        x.m_data = nullptr;
        x.m_dimensions = nullptr;
    }

    size_type length(size_type k) const
    {
        return m_dimensions[k];
    }

    flat_array& operator=(const flat_array& x)
    {
        if constexpr (AllocationFlag)
        {
            if (m_dataSize != x.m_dataSize)
            {
                if (!m_owner)
                {
                    std::ostringstream ss;
                    ss << "operator= NON-OWNER CANNOT BE RESIZED. size1: " << m_dataSize << " size2: " << x.m_dataSize;
                    throw FlatArrayException(ss.str());
                }
                delete[] m_data;
                m_dataSize = x.m_dataSize;
                m_data = new Elem[m_dataSize];
            }

            std::copy(x.m_data, x.m_data + m_dataSize, m_data);
            std::copy(x.m_dimensions, x.m_dimensions + Dim, m_dimensions);
        }
        else
        {
            m_dimensions = x.m_dimensions;
            m_dataSize = x.m_dataSize;
            m_data = x.m_data;
        }
        return *this;
    }

    template <bool copy2>
    flat_array& operator=(const flat_array<Elem, Dim, copy2>& x)
    {
        if constexpr (AllocationFlag)
        {
            if (m_dataSize != x.m_dataSize)
            {
                if (!m_owner)
                {
                    std::ostringstream ss;
                    ss << "operator= NON-OWNER CANNOT BE RESIZED. size1: " << m_dataSize << " size2: " << x.m_dataSize;
                    throw FlatArrayException(ss.str());
                }
                delete[] m_data;
                m_dataSize = x.m_dataSize;
                m_data = new Elem[m_dataSize];
            }

            std::copy(x.m_data, x.m_data + m_dataSize, m_data);
            std::copy(x.m_dimensions, x.m_dimensions + Dim, m_dimensions);
        }
        else
        {
            m_dimensions = x.m_dimensions;
            m_dataSize = x.m_dataSize;
            m_data = x.m_data;
        }
        return *this;
    }

    flat_array& operator=(flat_array&& x) noexcept
    {
        std::swap(m_dataSize, x.m_dataSize);
        std::swap(m_data, x.m_data);
        std::swap(m_dimensions, x.m_dimensions);
        return *this;
    }

    void set_width(int w)
    {
        output_width = w;
    }

    int get_width() const
    {
        return output_width;
    }


    decltype(auto) operator[](size_type p)
    {
        if constexpr (Dim != 1)
        {
            return flat_array<Elem, Dim - 1, false>(*this, p); // a subarray
        }
        else
        {
            return m_data[p]; // an element access
        }
    }

    decltype(auto) operator[](size_type p) const
    {
        if constexpr (Dim != 1)
        {
            return flat_array<Elem, Dim - 1, false>(*this, p); // a subarray
        }
        else
        {
            return m_data[p]; // an element access
        }
    }

    template<class ...T>
    decltype(auto) operator()(size_type k0, T...indices)
    {
        if constexpr (sizeof...(T) == Dim - 1)
        {
            return m_data[GetIndex(0, 0, k0, indices...)];
        }
        else if constexpr (Dim >= 2 && sizeof...(T) == 0)
        {
            return flat_array<Elem, Dim - 1, false>(*this, k0);
        }
        else if constexpr (Dim >= 2 && sizeof...(T) < Dim - 1)
        {
            return flat_array<Elem, Dim - 1, false>(*this, k0)(indices...);
        }
    }

    template<class ...T>
    decltype(auto) operator()(size_type k0, T...indices) const
    {
        if constexpr (sizeof...(T) == Dim - 1)
        {
            return m_data[GetIndex(0, 0, k0, indices...)];
        }
        else if constexpr (Dim >= 2 && sizeof...(T) == 0)
        {
            return flat_array<Elem, Dim - 1, false>(*this, k0);
        }
        else if constexpr (Dim >= 2 && sizeof...(T) < Dim - 1)
        {
            return flat_array<Elem, Dim - 1, false>(*this, k0)(indices...);
        }
    }

    using iterator = IteratorClass<Elem, true>;
    iterator begin() { return iterator::begin(m_data, m_dataSize); }
    iterator end() { return iterator::end(m_data, m_dataSize); }

    using const_iterator = IteratorClass<const Elem, true>;
    const_iterator begin() const { return const_iterator::begin(m_data, m_dataSize); }
    const_iterator end() const { return const_iterator::end(m_data, m_dataSize); }

    using reverse_iterator = IteratorClass<Elem, false>;
    reverse_iterator rbegin() { return reverse_iterator::begin(m_data, m_dataSize); }
    reverse_iterator rend() { return reverse_iterator::end(m_data, m_dataSize); }

    using const_reverse_iterator = IteratorClass<const Elem, false>;
    const_reverse_iterator rbegin() const { return const_reverse_iterator::begin(m_data, m_dataSize); }
    const_reverse_iterator rend() const { return const_reverse_iterator::end(m_data, m_dataSize); }


    friend std::ostream& operator<<(std::ostream& os, const flat_array& a)
    {
        std::string line(80, '-');
        if constexpr (Dim == 0)
        {
            if (a.m_dataSize != 0)
            {
                os << std::setw(a.get_width()) << a.m_data[0];
            }
        }
        else if constexpr (Dim == 1)
        {
            for (size_type i = 0; i < a.m_dataSize; i++)
            {
                os << std::setw(a.get_width()) << a.m_data[i];
                if (i != a.m_dataSize - 1)
                {
                    os << " ";
                }
            }
            os << std::endl;
        }
        else
        {
            size_type n = a.m_dimensions[0];
            for (size_type i = 0; i < n; i++)
            {
                os << a[i];
                if constexpr (Dim > 2) os << line << std::endl;
            }
        }
        return os;
    }

    template<class Element, int Dim1, bool allocation>
    friend void swap(flat_array<Element, Dim1, allocation>& x, flat_array<Element, Dim1, allocation>& y);

    template<class Element, int Dim1, bool allocation>
    friend void swap(flat_array<Element, Dim1, allocation>& x, flat_array<Element, Dim1, !allocation>& y);

    template<class Element, int Dim1, bool allocation1, bool allocation2>
    friend void swap(flat_array<Element, Dim1, allocation1>&& x, flat_array<Element, Dim1, allocation2>&& y);

    template<class Element, int Dim1, bool allocation1, bool allocation2>
    friend void swap(flat_array<Element, Dim1, allocation1>&& x, flat_array<Element, Dim1, allocation2>& y);

    template<class Element, int Dim1, bool allocation1, bool allocation2>
    friend void swap(flat_array<Element, Dim1, allocation1>& x, flat_array<Element, Dim1, allocation2>&& y);
};

template<class Element, int Dim1, bool allocation>
void swap(flat_array<Element, Dim1, allocation>& x, flat_array<Element, Dim1, allocation>& y)
{
    if (x.m_dataSize != y.m_dataSize)
    {
        std::ostringstream ss;
        ss << "swap. arrays of different sizes. size1: " << x.m_dataSize << " size2: " << y.m_dataSize;
        throw FlatArrayException(ss.str());
    }

    bool simple = allocation && x.m_owner && y.m_owner;

    if (simple)
    {
        std::swap(x.m_data, y.m_data);
    }
    else
    {
        for (std::size_t i = 0; i < x.m_dataSize; i++)
        {
            std::swap(x.m_data[i], y.m_data[i]);
        }
    }
}

template<class Element, int Dim1, bool allocation>
void swap(flat_array<Element, Dim1, allocation>& x, flat_array<Element, Dim1, !allocation>& y)
{
    if (x.m_dataSize != y.m_dataSize)
    {
        std::ostringstream ss;
        ss << "swap. arrays of different sizes. size1: " << x.m_dataSize << " size2: " << y.m_dataSize;
        throw FlatArrayException(ss.str());
    }

    for (std::size_t i = 0; i < x.m_dataSize; i++)
    {
        std::swap(x.m_data[i], y.m_data[i]);
    }
}

template<class Element, int Dim1, bool allocation1, bool allocation2>
void swap(flat_array<Element, Dim1, allocation1>&& x, flat_array<Element, Dim1, allocation2>&& y)
{
    swap(x, y);
}

template<class Element, int Dim1, bool allocation1, bool allocation2>
void swap(flat_array<Element, Dim1, allocation1>&& x, flat_array<Element, Dim1, allocation2>& y)
{
    swap(x, y);
}

template<class Element, int Dim1, bool allocation1, bool allocation2>
void swap(flat_array<Element, Dim1, allocation1>& x, flat_array<Element, Dim1, allocation2>&& y)
{
    swap(x, y);
}

#endif

