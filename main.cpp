#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include "MyLinkedList.cpp"

/*This globar variables are the times for one order be executed. */
double coffee_time = 1;
double beer_time = 0.5;
double whiskey_time = 0.7;
double gin_and_tonic_time = 2;

//Creating list that will take all custemers orders.
MyLinkedList order_list;

void served_orders(std::string order, double multiply_time) {
    //This function take every order and from it shows how much time is need for every one made order.

    if(order == "C"){
        sleep(coffee_time * multiply_time);
        std::cout << "Serving coffee, Prepared for " << coffee_time * multiply_time << " second." << std::endl;
    }
    else if(order == "B"){
        sleep(beer_time * multiply_time);
        std::cout << "Serving beer, Prepared for " << beer_time * multiply_time << " seconds." << std::endl;
    }
    else if(order == "W"){
        sleep(whiskey_time * multiply_time);
        std::cout << "Serving whiskey, Prepared for " << whiskey_time * multiply_time << " seconds." << std::endl;
    }
    else if(order == "G"){
        sleep(gin_and_tonic_time * multiply_time);
        std::cout << "Serving Gin and Tonic, Prepared for " << gin_and_tonic_time * multiply_time << " second." << std::endl;
    }
}

void* bar_thread(void* args){
//This function delete every order that is completed and chech if it has a command that closed the bar.
    while(1){
        Node order = order_list.pop();

        if(order_list.is_closed()){
            return nullptr;
        }
        served_orders(order.Value, order.time_multiply);
    }
}

void bar_work(){
/*This function represents the work of the bar. It creates a thread for the customer and take orders.
It has a condition for every drink and for the moment, when the client pays and leave bigget tips,
the orders will be faster. There is a condition when the client wants to order drink that is not in the menu/ */

    pthread_t consumer_thread;

    pthread_create(&consumer_thread, nullptr, bar_thread, nullptr);

    std::string order;

    double multiply_time = 1;

    while (true) {

        std::cin >> order;

        if (order == "Q") {
            std::cout << "Bar close." << std::endl;
            order_list.quit();
            break;
        }

        if(order == "B"){
            order_list.push(order, multiply_time);
        }
        else if(order == "C"){
            order_list.push(order, multiply_time);
        }
        else if(order == "W"){
            order_list.push(order, multiply_time);
        }
        else if(order == "G"){
            order_list.push(order, multiply_time);
        }
        else if(order[0] == 'T'){
            multiply_time *= stod(order.substr(1));
        }
        else if(order == "print"){
            order_list.print();
        }
        else {
            std::cout << "The bar doesn't serve such a drink!" << std::endl;
        }
    }

    pthread_join(consumer_thread, nullptr);
}

int main()
{
    //Execution of the proccesses in the bar.
    bar_work();

    return 0;
}

