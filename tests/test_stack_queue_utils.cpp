// tests/test_stack_queue_utils.cpp
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "stackqueue/stack.hpp"
#include "stackqueue/queue.hpp"
#include "stackqueue/stack_queue_utils.hpp"
#include <string>
#include <stdexcept>

using namespace dsac::stackqueue;

TEST_CASE("Problem_1_Remove_Adjacent_Duplicates", "[stack_queue_utils]") {
    REQUIRE(remove_duplicates("abbaca") == "ca");
    REQUIRE(remove_duplicates("abccba") == "");
    REQUIRE(remove_duplicates("") == "");
    REQUIRE(remove_duplicates("abcde") == "abcde");
    REQUIRE(remove_duplicates("aaaaa") == "a");
    REQUIRE(remove_duplicates("aaaa") == "");
    REQUIRE(remove_duplicates("1122334455") == "");
    REQUIRE(remove_duplicates("abbaa") == "a");
}

TEST_CASE("Problem_2_Clean_Backspace_String", "[stack_queue_utils]") {
    REQUIRE(clean_backspace_string("ab<c") == "ac");
    REQUIRE(clean_backspace_string("ab<<") == "");
    REQUIRE(clean_backspace_string("a<<b<<") == "");
    REQUIRE(clean_backspace_string("<") == "");
    REQUIRE(clean_backspace_string("<<a") == "a");
    REQUIRE(clean_backspace_string("a<bc<d") == "bd");
}

TEST_CASE("Problem_3_Stack_Contains_and_Restores_Order", "[stack_queue_utils]") {
    stack<int> st;
    queue<int> q;
    
    st.push(1);
    st.push(2);
    st.push(3);
    st.push(4);
    st.push(5);

    SECTION("Element_exists") {
        REQUIRE(stack_contains(st, q, 5) == true);
        REQUIRE(stack_contains(st, q, 1) == true);
        REQUIRE(stack_contains(st, q, 3) == true);
        
        // Verify state is completely restored!
        REQUIRE(st.top() == 5); st.pop();
        REQUIRE(st.top() == 4); st.pop();
        REQUIRE(st.top() == 3); st.pop();
        REQUIRE(st.top() == 2); st.pop();
        REQUIRE(st.top() == 1); st.pop();
        REQUIRE(st.empty() == true);
    }

    SECTION("Element_does_not_exist") {
        REQUIRE(stack_contains(st, q, 10) == false);
        REQUIRE(st.top() == 5); 
    }

    SECTION("Throws_when_queue_is_not_empty") {
        q.push(1); // Queue not empty initially
        REQUIRE_THROWS_AS(stack_contains(st, q, 3), std::invalid_argument);
    }
}

TEST_CASE("Stack_Basic_Methods", "[stack]") {
    stack<int> st;
    REQUIRE(st.empty() == true);
    st.push(10);
    st.push(20);
    REQUIRE(st.size() == 2);
    REQUIRE(st.top() == 20);
    st.pop();
    REQUIRE(st.top() == 10);
}

TEST_CASE("Queue_Basic_Methods", "[queue]") {
    queue<int> q;
    REQUIRE(q.empty() == true);
    q.push(10);
    q.push(20);
    REQUIRE(q.size() == 2);
    REQUIRE(q.front() == 10);
    REQUIRE(q.back() == 20);
    q.pop();
    REQUIRE(q.front() == 20);
}