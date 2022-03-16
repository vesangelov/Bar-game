//
// Created by User on 22.2.2022 г..
//

#ifndef BAR_MYLINKEDLIST_H
#define BAR_MYLINKEDLIST_H

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <unordered_map>

struct Node
{
    /*Create structure Node, that will store current order, the time that will have to multiply to be completed
    and the pointer to the next order in the linked list. */
    std::string Value;
    double time_multiply;
    Node* Next;
};

class MyLinkedList {
    /*My Linked List with all parameters. It begins with private fields, that don't need to be seen by the user.
     * These are the Node, that is on top of the linked list, the mutex, that will lock and unlock,
     * the condition is empty or not and the variable is bar closed. */
private:
    Node* head;
    pthread_mutex_t linked_list_mutex;
    pthread_cond_t not_empty;
    bool is_bar_closed = false;
public:
    /*The constructor with default parameters. */
    MyLinkedList(){
        head = nullptr;
        pthread_mutex_init(&linked_list_mutex, nullptr);
        pthread_cond_init(&not_empty, nullptr);
    }

    void push(std::string new_element, double multiply) {
        /*This function puts elements in the linked list and checks is it empty when adding an element.
        Every element if is not empty will be added at the end of the list. */
        pthread_mutex_lock(&linked_list_mutex);

        Node* new_node = new Node();

        new_node->Value = new_element;
        new_node->time_multiply = multiply;

        new_node->Next = nullptr;

        if(head == nullptr){
            head = new_node;
        }
        else {
            Node* lastNode = head;
            while(lastNode->Next != nullptr){
                lastNode = lastNode->Next;
            }

            lastNode->Next = new_node;
        }

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&linked_list_mutex);
    }

    Node pop(){
        /*This function will erase the first element in the linked list, if is not empty. If is empty
        will give a message for this, that can't delete element. */
        pthread_mutex_lock(&linked_list_mutex);

        while(head == nullptr){
            pthread_cond_wait(&not_empty, &linked_list_mutex);
            if(is_bar_closed){
                pthread_mutex_unlock(&linked_list_mutex);
                return Node();
            }
        }

        Node result = *head;
        Node* current = head;

        head = head->Next;

        delete current;

        pthread_mutex_unlock(&linked_list_mutex);

        return result;
    }

    void print() {
        /*This function will print all left orders that are waiting ant shows the number of every order,
         * that waits to be completed. */
        pthread_mutex_lock(&linked_list_mutex);
        Node* current = head;

        std::unordered_map<std::string , int> counter_map;

        if (current != nullptr) {
            while (current != nullptr) {
                counter_map[current->Value]++;
                current = current->Next;
            }

            for(auto & current_order : counter_map){
                std::cout << current_order.first << " -> " << current_order.second << std::endl;
            }
        }
        else {
            std::cout << "No orders." << std::endl;
        }

        pthread_mutex_unlock(&linked_list_mutex);
    }

    bool is_closed(){
        //This function check if it has a command that will close the bar for the evening.
        pthread_mutex_lock(&linked_list_mutex);
        bool is_closed = is_bar_closed;
        pthread_mutex_unlock(&linked_list_mutex);
        return is_closed;

    }

    void quit(){
        //This function closed the bar and it can't take more orders for the evening
        pthread_mutex_lock(&linked_list_mutex);

        Node* current = head;

        while (current != nullptr) {
            Node* next = current->Next;
            delete current;
            current = next;
        }

        head = nullptr;

        is_bar_closed = true;
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&linked_list_mutex);
    }
};


#endif //BAR_MYLINKEDLIST_H
