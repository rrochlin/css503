/** ------------- lab2.cpp -------------------------------
 * Lab 2
 *
 * Author : Robert Rochlin
 * Course: GCSDD CSS 503 Systems Programming
 *
 * Creation Date: 2026-04-29
 * Date of Last Modification: 2026-04-29
 * -------------------------------------------------------------------------
 * [Purpose]
 * This program spawns the specified # of threads by
 * the user and then tracks their execution handing off each
 * task in order by task id
 *
 * [Execution]
 * Compile: g++ lab2.cpp -lpthread -o lab2
 * Execute: ./lab2 4
 * -------------------------------------------------------------------------
 */
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int nThreads; // #threads

int turn;             // turn points which thread should run
pthread_mutex_t mtx;  // a lock for this critical section
pthread_cond_t *cond; // array of condition variable[nThreads]

void *thread_func(void *arg) {
   int id = ((int *)arg)[0]; // this thread's identifier
   delete (int *)arg;

   for (int loop = 0; loop < 10; loop++) // Run 10 times

   {
      // enter the critical section
      pthread_mutex_lock(&mtx);

      while (turn != id) {
         // wait until the (id - 1)th thread signals me.
         pthread_cond_wait(&cond[id], &mtx);
      }
      cout << "thread[" << id << "] got " << loop;
      switch (loop) {
      case 1:
         cout << "st";
         break;
      case 2:
         cout << "nd";
         break;
      case 3:
         cout << "rd";
         break;
      default:
         cout << "th";
      }
      cout << " turn" << endl;

      turn = (turn + 1) % nThreads;

      // signal the next thread
      pthread_cond_signal(&cond[turn]);

      // leave the critical section
      pthread_mutex_unlock(&mtx);
   }
   return nullptr;
}

int main(int argc, char *argv[]) {
   if (argc != 2)

   {
      cerr << "usage: lab2 #threads" << endl;
      return -1;
   }

   nThreads = atoi(argv[1]);
   if (nThreads < 1) {
      cerr << "usage: lab2 #threads" << endl;
      cerr << "where #threads >= 1" << endl;
      return -1;
   }

   pthread_t *tid = new pthread_t[nThreads]; // an array of thread identifiers
   cond = new pthread_cond_t[nThreads];      // an array of condition variables
   turn = 0; // points to which thread should run

   pthread_mutex_init(&mtx, nullptr);
   for (int i = 0; i < nThreads; i++) {
      pthread_cond_init(&cond[i], nullptr);
   }

   for (int i = 0; i < nThreads; i++) {
      int *id = new int[1];
      id[0] = i;
      pthread_create(&tid[i], NULL, thread_func, (void *)id);
   }

   for (int i = 0; i < nThreads; i++) // wait for all threads
   {
      pthread_join(tid[i], NULL);
   }

   for (int i = 0; i < nThreads; i++) {
      pthread_cond_destroy(&cond[i]);
   }
   pthread_mutex_destroy(&mtx);

   delete[] cond;
   delete[] tid;
}
