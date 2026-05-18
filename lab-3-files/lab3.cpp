
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
using namespace std;
struct timeval start, end2; // maintain starting and finishing wall time.

void startTimer() {
   gettimeofday(&start, NULL);
}
void stopTimer(const char *str) {
   gettimeofday(&end2, NULL);
   cout << str << "'s elapsed time\t= "
        << ((end2.tv_sec - start.tv_sec) * 1000000 + (end2.tv_usec - start.tv_usec)) << endl;
}

int main(int argc, char *argv[]) {
   if (argc != 3) {
      cerr << "usage: lab3 filename bytes" << endl;
      return -1;
   }
   int bytes = atoi(argv[2]);
   if (bytes < 1) {
      cerr << "usage: lab3 filename bytes" << endl;
      cerr << "where bytes > 0" << endl;
      return -1;
   }
   char *filename = argv[1];
   char *buf = new char[bytes];
   // linux i/o
   int fd = open(filename, O_RDONLY);
   if (fd == -1) {
      cerr << filename << " not found" << endl;
      return -1;
   }
   startTimer();
   while (read(fd, buf, bytes) > 0);
   stopTimer("Unix read");
   close(fd);
   // standard i/o
   // write the same functionality as in linux i/o
   // but use fopen(), fgetc(), fread(), and fclose( )
   // use fgetc() if bytes == 1
   FILE *fd2 = fopen(filename, "r");
   if (fd2 == nullptr) {
      cerr << filename << " not found" << endl;
      cerr << "error code: " << errno << endl;
      return -1;
   }
   if (bytes == 1) {
      startTimer();
      while (fgetc(fd2) != EOF);
      stopTimer("fgetc time");
   } else {
      char *buf2 = new char[bytes];
      startTimer();
      while (fread(buf2, sizeof(char), bytes, fd2) > 0);
      stopTimer("fread time");
   }
   fclose(fd2);
   return 0;
}
