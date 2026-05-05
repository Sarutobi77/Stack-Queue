// include/stackqueue/stack_queue_utils.hpp
#pragma once

#include "array/vector.hpp"
#include "list/doubly_linked.hpp"
#include "stackqueue/stack.hpp"
#include "stackqueue/queue.hpp"

#include <string>
#include <stdexcept>
#include <algorithm>

namespace dsac::stackqueue {

inline std::string remove_duplicates(std::string input) {
    stack<char> st;
    for (char c : input) {
        if (!st.empty() && st.top() == c) {
            st.pop();
        } else {
            st.push(c);
        }
    }
    
    std::string result = "";
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

inline std::string clean_backspace_string(std::string input) {
    stack<char> st;
    for (char c : input) {
        if (c == '<') {
            if (!st.empty()) {
                st.pop();
            }
        } else {
            st.push(c);
        }
    }
    
    std::string result = "";
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

template <
    typename T, 
    typename StackContainer = dsac::array::Vector<T>,
    typename QueueContainer = dsac::array::Vector<T>
>
bool stack_contains(
    dsac::stackqueue::stack<T, StackContainer>& st, 
    dsac::stackqueue::queue<T, QueueContainer>& qu, 
    const T& elt) {
    
    if (!qu.empty()) {
        throw std::invalid_argument("Q must be initially empty.");
    }
    
    bool found = false;
    int n = st.size();
    
    // Step 1: Unload Stack into Queue (Queue now has elements, but Stack is empty)
    for (int i = 0; i < n; ++i) {
        if (st.top() == elt) {
            found = true;
        }
        qu.push(st.top());
        st.pop();
    }
    
    // Step 2: Queue to Stack (Stack has elements in reversed order)
    for (int i = 0; i < n; ++i) {
        st.push(qu.front());
        qu.pop();
    }
    
    // Step 3: Stack to Queue (Queue has elements reversed)
    for (int i = 0; i < n; ++i) {
        qu.push(st.top());
        st.pop();
    }
    
    // Step 4: Queue back to Stack (Stack is now restored to its original order!)
    for (int i = 0; i < n; ++i) {
        st.push(qu.front());
        qu.pop();
    }
    
    return found;
}

} // namespace dsac::stackqueue