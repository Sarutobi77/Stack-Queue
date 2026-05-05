#pragma once
#include <algorithm>     // provides max
#include <stdexcept>     // provides out_of_range exception
#include <utility>       // provides std::move

namespace dsac::array {

template <typename T>
class Vector {
    private:
        int cap{0};
        int sz{0};
        T* data{nullptr};

    public:
        Vector() : cap(0), sz(0), data(nullptr) {}
        
        int capacity() const { return cap; }
        int size() const { return sz; }     
        bool empty() const { return sz == 0; }
        
        const T& operator[](int i) const { return data[i]; }
        T& operator[](int i) { return data[i]; }
        
        const T& at(int i) const {
            if (i < 0 || i >= sz) throw std::out_of_range("Index out of bounds");
            return data[i];
        }
        T& at(int i) {
            if (i < 0 || i >= sz) throw std::out_of_range("Index out of bounds");
            return data[i];
        }

        const T& front() const { return data[0]; }
        T& front() { return data[0]; }

        const T& back() const { return data[sz - 1]; }
        T& back() { return data[sz - 1]; }

        void push_back(const T& elem) {
            if (sz == cap) {
                reserve(cap == 0 ? 1 : cap * 2);
            }
            data[sz++] = elem;
        }

        void pop_back() {
            if (sz > 0) {
                sz--;
            }
        }

        void insert(int i, const T& elem) {
            if (i < 0 || i > sz) throw std::out_of_range("Index out of bounds");
            if (sz == cap) reserve(cap == 0 ? 1 : cap * 2);
            for (int j = sz; j > i; --j) {
                data[j] = data[j - 1];
            }
            data[i] = elem;
            sz++;
        }

        void erase(int i) {
            if (i < 0 || i >= sz) throw std::out_of_range("Index out of bounds");
            for (int j = i; j < sz - 1; ++j) {
                data[j] = data[j + 1];
            }
            sz--;
        }

        void reserve(int minimum) {
            if (minimum > cap) {
                T* new_data = new T[minimum];
                for (int i = 0; i < sz; ++i) {
                    new_data[i] = std::move(data[i]);
                }
                delete[] data;
                data = new_data;
                cap = minimum;
            }
        }

        Vector(const Vector& other) {
            clone(other);
        }

        Vector& operator=(const Vector& other) {
            if (this != &other) {
                delete[] data;
                clone(other);
            }
            return *this;
        }

        Vector(Vector&& other) {
            transfer(other);
        }

        Vector& operator=(Vector&& other) {
            if (this != &other) {
                delete[] data;
                transfer(other);
            }
            return *this;
        }

        ~Vector() {
            delete[] data;
        }
    
    private:
        void clone(const Vector& other) {
            cap = other.cap;
            sz = other.sz;
            data = (cap > 0) ? new T[cap] : nullptr;
            for (int i = 0; i < sz; ++i) {
                data[i] = other.data[i];
            }
        }

        void transfer(Vector& other) {
            cap = other.cap;
            sz = other.sz;
            data = other.data;
            other.cap = 0;
            other.sz = 0;
            other.data = nullptr;
        }

    public:
        class iterator {
            friend class Vector;
            private:
                Vector* vec;
                int ind;        // index within the vector
            
            public:
                iterator(Vector* v = nullptr, int i = -1) : vec(v), ind(i) {}
                T& operator*() const { return vec->data[ind]; }
                T* operator->() const { return &(vec->data[ind]); }
                iterator& operator++() { ind++; return *this; }
                iterator operator++(int) { iterator temp = *this; ind++; return temp; }
                iterator& operator--() { ind--; return *this; }
                iterator operator--(int) { iterator temp = *this; ind--; return temp; }
                bool operator==(iterator rhs) const { return vec == rhs.vec && ind == rhs.ind; }
                bool operator!=(iterator rhs) const { return !(*this == rhs); }
        };

        class const_iterator {
            private:
                const Vector* vec;
                int ind;       
            
            public:
                const_iterator(const Vector* v = nullptr, int i = -1) : vec(v), ind(i) {}
                const T& operator*() const { return vec->data[ind]; }
                const T* operator->() const { return &(vec->data[ind]); }
                const_iterator& operator++() { ind++; return *this; }
                const_iterator operator++(int) { const_iterator temp = *this; ind++; return temp; }
                const_iterator& operator--() { ind--; return *this; }
                const_iterator operator--(int) { const_iterator temp = *this; ind--; return temp; }
                bool operator==(const_iterator rhs) const { return vec == rhs.vec && ind == rhs.ind; }
                bool operator!=(const_iterator rhs) const { return !(*this == rhs); }
        };

        iterator begin() { return iterator(this, 0); }
        iterator end() { return iterator(this, sz); }
        const_iterator begin() const { return const_iterator(this, 0); }
        const_iterator end() const { return const_iterator(this, sz); }

        iterator insert(iterator it, const T& elem) {
            this->insert(it.ind, elem);
            return iterator(this, it.ind);
        }

        iterator erase(iterator it) {
            this->erase(it.ind);
            return iterator(this, it.ind);
        }

        void shrink() { shrink_to_fit(); }
        
        void shrink_to_fit() {
            if (sz < cap) {
                cap = sz;
                T* new_data = (cap > 0) ? new T[cap] : nullptr;
                for (int i = 0; i < sz; ++i) {
                    new_data[i] = std::move(data[i]);
                }
                delete[] data;
                data = new_data;
            }
        }
    
}; // Vector class

}  // namespace dsac::array