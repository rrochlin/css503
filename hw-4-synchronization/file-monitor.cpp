/** ------------- file-monitor.cpp -------------------------------
 * HW 4
 *
 * Author : Robert Rochlin
 * Course: GCSDD CSS 503 Systems Programming
 *
 * -------------------------------------------------------------------------
 * [Purpose]
 * This is sample psuedocode meant to fulfil the requirements of problem
 * 6.29 where we will develop a Monitor ADT to coordinate file access
 * based off of assigned numbers of processes
 *
 * [Execution]
 * Compile: g++ file-monitor.cpp -lpthread -o monitor
 * Execute: ./monitor {\d+}
 *
 * [Assumptions}
 * we will be using sum > 4 strictly
 * -------------------------------------------------------------------------
 */
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

// print helper
void logState(const char *event, int processNum, int sum) {
   cout << "process " << processNum << ' ' << event << ", active sum = " << sum
        << endl;
}

class FileMonitor {
 public:
   FileMonitor(int limit) : limit(limit), currentSum(0) {
      pthread_mutex_init(&mtx, nullptr);
      pthread_cond_init(&canEnter, nullptr);
   }

   ~FileMonitor() {
      pthread_cond_destroy(&canEnter);
      pthread_mutex_destroy(&mtx);
   }

   int enter(int processNum) {
      pthread_mutex_lock(&mtx);

      while (currentSum + processNum >= limit) {
         pthread_cond_wait(&canEnter, &mtx);
      }

      currentSum += processNum;
      int sum = currentSum;
      pthread_mutex_unlock(&mtx);
      return sum;
   }

   int leave(int processNum) {
      pthread_mutex_lock(&mtx);
      currentSum -= processNum;
      int sum = currentSum;

      // Any waiting process might now fit under the limit.
      pthread_cond_broadcast(&canEnter);
      pthread_mutex_unlock(&mtx);
      return sum;
   }

 private:
   pthread_mutex_t mtx;
   pthread_cond_t canEnter;
   int limit;
   int currentSum;
};

struct ThreadArgs {
   FileMonitor *monitor;
   int processNum;
};

void *accessFile(void *arg) {
   ThreadArgs *args = static_cast<ThreadArgs *>(arg);

   int enterSum = args->monitor->enter(args->processNum);
   logState("entered", args->processNum, enterSum);

   // Placeholder for the actual shared file access.
   sleep(1);

   int leaveSum = args->monitor->leave(args->processNum);
   logState("left", args->processNum, leaveSum);

   return nullptr;
}

int main(int argc, char *argv[]) {
   if (argc != 2) {
      cerr << "usage: " << argv[0] << " <n>" << endl;
      return 1;
   }

   int limit = atoi(argv[1]);
   if (limit <= 4) {
      cerr << "n must be > 4" << endl;
      return 1;
   }

   FileMonitor monitor(limit);

   const int processNums[] = {1, 2, 3, 4};
   const int threadCount = sizeof(processNums) / sizeof(processNums[0]);
   pthread_t threads[threadCount];
   ThreadArgs args[threadCount];

   for (int i = 0; i < threadCount; i++) {
      args[i].monitor = &monitor;
      args[i].processNum = processNums[i];
      pthread_create(&threads[i], nullptr, accessFile, &args[i]);
   }

   for (int i = 0; i < threadCount; i++) {
      pthread_join(threads[i], nullptr);
   }

   return 0;
}
