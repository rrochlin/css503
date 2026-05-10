#ifndef SHOP_ORG_H_
#define SHOP_ORG_H_
#include <iostream>
#include <pthread.h>
#include <queue>
#include <sstream>
#include <string>
using namespace std;

#define kDefaultNumChairs 3
#define kDefaultNumBarbers 1

class Shop {
 public:
   // we're only initializing the const's here because of the ternary in vec
   // initializer
   Shop(int num_barbers, int num_chairs)
       : max_waiting_cust_((num_chairs >= 0) ? num_chairs : kDefaultNumChairs),
         max_num_barbers_((num_barbers > 0) ? num_barbers : kDefaultNumBarbers), cust_drops_(0) {
      init();
   };
   Shop()
       : max_waiting_cust_(kDefaultNumChairs), max_num_barbers_(kDefaultNumBarbers),
         cust_drops_(0) {
      init();
   };

   bool visitShop(int id); // return true only when a customer got a service
   void leaveShop(int id);
   void helloCustomer(int id);
   void byeCustomer(int id);
   int get_cust_drops() const;

 private:
   const int max_waiting_cust_; // the max number of threads that can wait
   const int max_num_barbers_;
   vector<int> customer_in_chair_; // tracks which customers are where
   vector<bool> in_service_;       // indicates barber is working
   vector<bool> money_paid_;       // used to signal that customer finished
   queue<int> waiting_chairs_;     // includes the ids of all waiting threads
   queue<int> barbers;             // FIFO queue to handle provisioning barbers
   int cust_drops_;                // track the number of customers turned away

   // global mutex for data interaction that cannot be done in parallel
   pthread_mutex_t mutex_;
   // condition flag to signal waiting customers, can be singular since
   // any waiting customer would want to be notified if a barber is free
   pthread_cond_t cond_customers_waiting_;
   // array tracking when a customer is served by a barber
   vector<pthread_cond_t> cond_customer_served_;
   // array tracking which barber is paid
   vector<pthread_cond_t> cond_barber_paid_;
   // array tracking which barbers are sleeping
   vector<pthread_cond_t> cond_barber_sleeping_;

   void init();
   string int2string(int i);
   void print(int person, string message);
};
#endif
