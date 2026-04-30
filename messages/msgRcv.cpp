
//
// System V messages are a key form of IPC.  Here is a simple example which receives a message
// from a queue.
// 

#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>   // IPC_CREAT flag
#include <sys/msg.h>   // msgget, msgrcv
#include <iostream>
using namespace std;

const int MSG_SIZE = 128;
typedef struct 
{
   long msgType;
   char msgText[MSG_SIZE];
} message_buf;

int main()
{
   int msgID;
   size_t msgSize;
   
   // Do you know why 0666 is used?  Take some time to figure it out.
   int msgFlags = IPC_CREAT | 0666;
   message_buf message;

   // The key is the shared piece of information between the sender / receiver
   key_t key = 777;
   msgID = msgget(key, msgFlags);
   if (msgID == -1)
   {
      perror("Error on msgget");
      exit(EXIT_FAILURE);
   }
   
   int rc = msgrcv(msgID, &message, MSG_SIZE, 1, 0);
   if (rc == -1)
   {
      perror("Error on msgrv");
      exit(EXIT_FAILURE);
   }
   cout << message.msgText << endl;
}
