# HW 4

## Robert Rochlin

## 6.16

Interrupts are not appropriate for implementing synchronization primitives in multiprocessor systems due to the overhead incurred by preventing them on all cores of a process. In order to effectively use interrupts as the controlling feature for systems that are multithreaded it would require passing messages to all of the other processor cores before proceeding with the critical section of code. This is time that the critical section of code needs to hang for until it can actually begin executing even if there isn't another process executing that would prevent it from running. It's necessary to disable interrupts across all cores since any process on a different core could be using them to access sensitive memory.

## 6.19

### The lock is to be held for a short duration

For short duration holding using a spin lock is a better option. The price of context switching will exceed the price of burning CPU cycles having the process spin and ping the lock.

### The lock is to be held for a long duration

For long duration holds using a mutex lock with a process sleep is the better option. Since the process is going to be waiting a while it's better to eat the cost of the context switch and overhead with waking it again than it is to let it idle and burn compute. Other processes can use the core to preform useful work in the meantime increasing the efficiency of the design.

### A thread may be put to sleep while holding the lock

if the thread could be put to sleep while holding the lock you would want to use mutex locks and sleeps. Since the holding thread can be slept you're going to have to wait for the scheduler to run this process again before the lock could even be released, which translates to a long wait time on the mutex becoming unlocked.

### 6.29

See file-monitor.cpp

### 7.4

In the dining philosophers problem each philosopher requires 2 chopsticks to eat but are sat at a table with the same number of chopsticks as philosophers. They are only able to grab the chopstick in front of them to their left and rigth i.e. 2 choices. The philosophers are able to pick up chopsticks one at a time. If all *n* philosophers wanted to eat simultaneously then none of them would be able to eat as they would have in the best case all grabbed 1 chopstick, leaving them all with only 1 and 0 left on the table. This would cause a deadlock since the philosopher's need 2 chopsticks to eat, but they all only have 1 and would be stuck holding a single chopstick waiting to eat.


