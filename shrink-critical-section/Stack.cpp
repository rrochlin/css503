#include "Stack.h"

Stack::Stack() {
   head = nullptr;
   pthread_mutex_init(&mtx, nullptr);
}

bool Stack::push(int val) {
   Node *insNode = new Node();
   insNode->value = val;
   insNode->next = nullptr;
   // lock here to ensure that the head does not change while we add in the new
   // node.
   pthread_mutex_lock(&mtx);
   insNode->next = head;
   head = insNode;
   pthread_mutex_unlock(&mtx);
   return true;
}

bool Stack::pop(int &val) {
   pthread_mutex_lock(&mtx);
   if (head != nullptr) {
      // acquire the lock before capturing stack stack
      val = head->value;
      Node *temp = head;
      head = head->next;
      pthread_mutex_unlock(&mtx);
      // release lock here since we've finished mutating the stack
      delete temp;
      return true;
   } else {
      pthread_mutex_unlock(&mtx);
      return false;
   }
}

Stack::~Stack() {
   int val;
   while (pop(val)) {
   };
   pthread_mutex_destroy(&mtx);
}
