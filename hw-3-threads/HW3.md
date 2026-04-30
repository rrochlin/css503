# HW 3
## Robert Rochlin

## 4.19
The program shown in Figure 4.23 (see below) uses the Pthreads API. What would be the output from the program at LINE C and LINE P?
```C
  #include <pthread.h>
  #include <stdio.h>  
  int value = 0;
  void *runner(void *param); /* the thread */

  int main(int argc, char *argv[])
  {
  pid_t pid;
  pthread_t tid;
  pthread_attr_t attr;

   pid = fork();
   if (pid == 0) 
   { /* child process */
     pthread_attr_init(&attr);
     pthread create(&tid,&attr,runner,NULL);
     pthread_join(tid,NULL);
     printf("CHILD: value = %d",value); /* LINE C */
   }
   else if (pid > 0) 
   { /* parent process */
     wait(NULL);
     printf("PARENT: value = %d",value); /* LINE P */
   }
  }

  void *runner(void *param) 
  {
     value = 5;
     pthread_exit(0);
  }
```

### Answer
Line C = 5
Line P = 0

## 3
Read and summarize following [page](http://www.thegeekstuff.com/2013/11/linux-process-and-threads)

### Summary

#### Processes

Processes are a fundemental component of Linux, responsilbe for every piece of work done by the OS. Anything that is done on Linux is done through a process that is delegated access to system resources by the kernel making it the fundamental entity for the operating system. These processes are executed in order of priority by the kernel and swapped in and out to prevent stalling execution while a process waits on IO to return. 
- Processes are able to communicate with eachother through interprocess communication and shared memory
- In Linux the `fork()` system call is used to create new processes. This system call copies the current process state into a new child process, sharing all of the segments (text, stack, heap, etc...) until the child process attempts to mutate these things. (The way this part is written implies that the parent processes mutation wouldn't do this and that the child would be looking at the mutations??? It specifically says `until child tries to make any change to stack or heap` which implies parent changes don't create a child copy)

-- follow up I looked into this and the parent process writing also triggers a copy

#### Linux Threads vs Light Weight Processes

Threads are purely a user space concept they are do not exist at the kernel level. To the kernel threads are just other processes that need to go through the standard management cycle. Threads can be referred to as LWP's which is a holdover from a previous era of Linux where threads existed only in user space and the kernel would not actually let them get spawned into different processes.

LWP's are the processes spawned from creating threads and are distinct from normal processes in that they share the same address space and other resources like open files. LWP's/Threads are created by using functions from the pthread library on Linux. The internal implementation uses the `clone()` system call similar to how `fork()` is implemented. However with `pthread` the flags passed to `clone()` are different to allow for the shared memory that LWP's have.

These LWP's share the same life cycle as standard linux processes and are ran according to the load on the scheduler and their priority.