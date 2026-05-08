#include "shop.h"
#include <algorithm>
#include <iterator>
#include <vector>

void Shop::init() {
   // load barber ID's into queue to manage their even dispatch
   for (int i = 0; i < max_num_barbers_; i++) { barbers.push(i); }

   customer_in_chair_ = vector<int>(max_num_barbers_, 0);
   in_service_ = vector<bool>(max_num_barbers_, 0);
   money_paid_ = vector<bool>(max_num_barbers_, 0);
   cond_customer_served_ = vector<pthread_cond_t>(max_num_barbers_);
   cond_barber_paid_ = vector<pthread_cond_t>(max_num_barbers_);
   cond_barber_sleeping_ = vector<pthread_cond_t>(max_num_barbers_);

   pthread_mutex_init(&mutex_, NULL);
   pthread_cond_init(&cond_customers_waiting_, NULL);
   for (int i = 0; i < max_num_barbers_; i++) {
      pthread_cond_init(&cond_customer_served_[i], NULL);
      pthread_cond_init(&cond_barber_paid_[i], NULL);
      pthread_cond_init(&cond_barber_sleeping_[i], NULL);
   }
}

string Shop::int2string(int i) {
   stringstream out;
   out << i;
   return out.str();
}

void Shop::print(int person, string message) {
   cout << ((person > 0) ? "customer[" : "barber  [") << person << "]: " << message << endl;
}

int Shop::get_cust_drops() const {
   return cust_drops_;
}

bool Shop::visitShop(int id) {
   pthread_mutex_lock(&mutex_);

   // If all chairs are full then leave shop
   if ((int)waiting_chairs_.size() == max_waiting_cust_) {
      print(id, "leaves the shop because of no available waiting chairs.");
      ++cust_drops_;
      pthread_mutex_unlock(&mutex_);
      return false;
   }

   // if all barbers are busy and there are no free waiting chairs then leave
   if (barbers.empty() || !waiting_chairs_.empty()) {
      waiting_chairs_.push(id);
      print(id, "takes a waiting chair. # waiting seats available = " +
                   int2string(max_waiting_cust_ - waiting_chairs_.size()));
      pthread_cond_wait(&cond_customers_waiting_, &mutex_);
      waiting_chairs_.pop();
   }

   print(id, "moves to the service chair. # waiting seats available = " +
                int2string(max_waiting_cust_ - waiting_chairs_.size()));

   // grab the next open barber and pop
   int barber = barbers.front();
   barbers.pop();

   customer_in_chair_[barber] = id;
   in_service_[barber] = true;

   // wake up the barber just in case if he is sleeping
   pthread_cond_signal(&cond_barber_sleeping_[barber]);

   pthread_mutex_unlock(&mutex_);
   return true;
}

void Shop::leaveShop(int id) {
   pthread_mutex_lock(&mutex_);

   // Wait for service to be completed
   print(id, "wait for the hair-cut to be done");
   auto it = find(customer_in_chair_.begin(), customer_in_chair_.end(), id);
   if (it == customer_in_chair_.end()) {
      cerr << "customer expected in chair was not found aborting leave\n";
      cerr << "customer: " << id << endl;
      return;
   }
   int chair = distance(customer_in_chair_.begin(), it);
   while (in_service_[chair] == true) { pthread_cond_wait(&cond_customer_served_[chair], &mutex_); }

   // Pay the barber and signal barber appropriately
   money_paid_[chair] = true;
   pthread_cond_signal(&cond_barber_paid_[chair]);
   print(id, "says good-bye to the barber.");
   pthread_mutex_unlock(&mutex_);
}

void Shop::helloCustomer(int id) {
   pthread_mutex_lock(&mutex_);

   // If no customers than barber can sleep
   if (waiting_chairs_.empty() && customer_in_chair_[id] == 0) {
      print(-1 * id, "sleeps because of no customers.");
      pthread_cond_wait(&cond_barber_sleeping_[id], &mutex_);
   }

   if (customer_in_chair_[id] == 0) // check if the customer, sit down.
   {
      pthread_cond_wait(&cond_barber_sleeping_[id], &mutex_);
   }

   print(-1 * id, "starts a hair-cut service for " + int2string(customer_in_chair_[id]));
   pthread_mutex_unlock(&mutex_);
}

void Shop::byeCustomer(int id) {
   pthread_mutex_lock(&mutex_);

   // Hair Cut-Service is done so signal customer and wait for payment
   in_service_[id] = false;
   print(-1 * id,
         "says he's done with a hair-cut service for " + int2string(customer_in_chair_[id]));
   money_paid_[id] = false;
   pthread_cond_signal(&cond_customer_served_[id]);
   while (money_paid_[id] == false) { pthread_cond_wait(&cond_barber_paid_[id], &mutex_); }

   // Signal to customer to get next one
   customer_in_chair_[id] = 0;
   print(-1 * id, "calls in another customer");

   barbers.push(id);
   pthread_cond_signal(&cond_customers_waiting_);

   pthread_mutex_unlock(&mutex_); // unlock
}
