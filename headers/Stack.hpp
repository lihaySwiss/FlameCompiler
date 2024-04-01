#pragma once
#include <iostream>

#define SIZE 1000
 
using namespace std;

template <class T> class Stack {
public:
    Stack() { top = -1; };
 
     // To add element element k to stack
    void push(T k){
        // Checking whether stack is completely filled
        if (isFull()) {
            cout << "Stack is full\n";
        }
    
        // Inserted element
        top = top + 1;
        st[top] = k;
    }

    // To remove the top element from stack
    T pop(){
        // Initialising a variable to store popped element
        T popped_element = st[top];
        top--;
        return popped_element;
    }
    
    // To get the top element of stack
    T peek()
    {
        // Initialising a variable to store top element
        T top_element = st[top];
    
        // Returning the top element
        return top_element;
    }

    // To check if the stack is full
    bool isFull(){
        if (top == (SIZE - 1))
            return 1;
        else
            return 0;
    }


    // To check if the stack is empty
    bool isEmpty()
    {
        if (top == -1)
            return 1;
        else
            return 0;
    }
 
private:
    int top;
    T st[SIZE];
};
