#pragma once

#include <utility>    
#include <iostream>
#include <stdexcept>

namespace dsac::list {

template <typename T>
class DoublyLinkedList;

template <typename T>
void swap(DoublyLinkedList<T>& a, DoublyLinkedList<T>& b) {
    std::swap(a.sz, b.sz);
    std::swap(a.header, b.header);
    std::swap(a.trailer, b.trailer);
}

template <typename T>
class DoublyLinkedList {
    private:
        class Node {
            public:
                T elem;
                Node* prev{nullptr};
                Node* next{nullptr};
                
                Node() {}
                Node(const T& element, Node* prv, Node* nxt) 
                    : elem(element), prev(prv), next(nxt) {}
        };
        int sz{0};
        Node* header; 
        Node* trailer;
        
        void create_sentinels() {
            header = new Node();
            trailer = new Node();
            header->next = trailer;
            trailer->prev = header;
            sz = 0;
        }

    public:
        DoublyLinkedList() {
            create_sentinels();
        }

        int size() const { return sz; }
        bool empty() const { return sz == 0; }
        
        T& front() { return header->next->elem; }
        const T& front() const { return header->next->elem; }
        
        T& back() { return trailer->prev->elem; }
        const T& back() const { return trailer->prev->elem; }

    private:
        Node* insert_before(T elem, Node* successor) {
            Node* new_node = new Node(elem, successor->prev, successor);
            successor->prev->next = new_node;
            successor->prev = new_node;
            sz++;
            return new_node;
        }

        void erase(Node* node) {
            Node* prv = node->prev;
            Node* nxt = node->next;
            prv->next = nxt;
            nxt->prev = prv;
            delete node;
            sz--;
        }

    public:
        void push_front(const T& elem) { insert_before(elem, header->next); }
        void push_back(const T& elem) { insert_before(elem, trailer); }
        
        void pop_front() { 
            if(!empty()) erase(header->next); 
        }
        void pop_back() { 
            if(!empty()) erase(trailer->prev); 
        }

        void concatenate(DoublyLinkedList& M) {
            if (!M.empty()) {
                // Link the end of this list to the start of M
                trailer->prev->next = M.header->next;
                M.header->next->prev = trailer->prev;
                
                // Link the end of M to this list's trailer
                M.trailer->prev->next = trailer;
                trailer->prev = M.trailer->prev;
                
                // Update sizes and reset M
                sz += M.sz;
                M.header->next = M.trailer;
                M.trailer->prev = M.header;
                M.sz = 0;
            }
        }

    public:
        class iterator {
            friend class DoublyLinkedList;
            private:
                Node* node_ptr;
            public:
                iterator(Node* ptr = nullptr) : node_ptr(ptr) {}
                T& operator*() const { return node_ptr->elem; }
                T* operator->() const { return &(node_ptr->elem); }
                iterator& operator++() { node_ptr = node_ptr->next; return *this; }
                iterator operator++(int) { iterator temp = *this; node_ptr = node_ptr->next; return temp; }
                iterator& operator--() { node_ptr = node_ptr->prev; return *this; }
                iterator operator--(int) { iterator temp = *this; node_ptr = node_ptr->prev; return temp; }
                bool operator==(const iterator& other) const { return node_ptr == other.node_ptr; }
                bool operator!=(const iterator& other) const { return !(*this == other); }
        };

        class const_iterator {
            private:
                const Node* node_ptr;
            public:
                const_iterator(const Node* ptr = nullptr) : node_ptr(ptr) {}
                const T& operator*() const { return node_ptr->elem; }
                const T* operator->() const { return &(node_ptr->elem); }
                const_iterator& operator++() { node_ptr = node_ptr->next; return *this; }
                const_iterator operator++(int) { const_iterator temp = *this; node_ptr = node_ptr->next; return temp; }
                const_iterator& operator--() { node_ptr = node_ptr->prev; return *this; }
                const_iterator operator--(int) { const_iterator temp = *this; node_ptr = node_ptr->prev; return temp; }
                bool operator==(const const_iterator& other) const { return node_ptr == other.node_ptr; }
                bool operator!=(const const_iterator& other) const { return !(*this == other); }
        };

    public:
        iterator begin() { return iterator(header->next); }
        const_iterator begin() const { return const_iterator(header->next); }
        iterator end() { return iterator(trailer); }
        const_iterator end() const { return const_iterator(trailer); }

        iterator insert(iterator it, const T& elem) {
            Node* new_node = insert_before(elem, it.node_ptr);
            return iterator(new_node);
        }

        iterator erase(iterator it) {
            Node* nxt = it.node_ptr->next;
            this->erase(it.node_ptr);
            return iterator(nxt);
        }

    private:
        void clone(const DoublyLinkedList& other) {
            create_sentinels();
            for (const_iterator it = other.begin(); it != other.end(); ++it) {
                push_back(*it);
            }
        }

    public:
        friend void swap<>(DoublyLinkedList<T>& a, DoublyLinkedList<T>& b);
        
        void clear() {
            while (!empty()) {
                pop_front();
            }
        }

        DoublyLinkedList(const DoublyLinkedList& other) {
            clone(other);
        }

        DoublyLinkedList& operator=(const DoublyLinkedList& other) {
            if (this != &other) {
                DoublyLinkedList temp(other);
                swap(*this, temp);
            }
            return *this;
        }

        DoublyLinkedList(DoublyLinkedList&& other) {
            create_sentinels();
            swap(*this, other);
        }

        DoublyLinkedList& operator=(DoublyLinkedList&& other) {
            if (this != &other) {
                swap(*this, other);
            }
            return *this;
        }

        ~DoublyLinkedList() {
            clear();
            delete header;
            delete trailer;
        }

        void print() const {
            for (const_iterator it = begin(); it != end(); ++it) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
};

}  // namespace dsac::list