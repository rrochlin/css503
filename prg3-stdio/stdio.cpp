#pragma once
#include "stdio.h"

#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
using namespace std;

char decimal[100];

// recursive_itoa
// Recursive Integer to Char (ASCII) conversion.  Helper for *itoa
// Populates the decimal char array that represents a given int
int recursive_itoa(int arg) {
   int div = arg / 10;
   int mod = arg % 10;
   int index = 0;
   if (div > 0) {
      index = recursive_itoa(div);
   }
   decimal[index] = mod + '0';
   return ++index;
}

// *itoa
// Integer to character array (c string)
char *itoa(const int arg) {
   bzero(decimal, 100);
   int order = recursive_itoa(arg);
   char *new_decimal = new char[order + 1];
   bcopy(decimal, new_decimal, order + 1);
   return new_decimal;
}

// printf
// Implementation of the STL printf function
// Takes a format string and a variable number of arguments
//  and prints the formatted output to the standard output stream
int printf(const void *format, ...) {
   va_list list; // variable argument list type
   va_start(list, format);

   char *msg = (char *)format;
   char buf[1024];
   int nWritten = 0;

   int i = 0, j = 0, k = 0;
   while (msg[i] != '\0') {
      if (msg[i] == '%' && msg[i + 1] == 'd') {
         buf[j] = '\0';
         nWritten += write(1, buf, j);
         j = 0;
         i += 2;

         int int_val = va_arg(list, int);
         char *dec = itoa(abs(int_val));
         if (int_val < 0) {
            nWritten += write(1, "-", 1);
         }
         nWritten += write(1, dec, strlen(dec));
         delete[] dec;
      } else {
         buf[j++] = msg[i++];
      }
   }
   if (j > 0) {
      nWritten += write(1, buf, j);
   }
   va_end(list);
   return nWritten;
}

// setvbuf
// Sets the buffering mode and the size of the buffer for a stream
// Returns 0 if successful, EOF/-1 if using an unsupported mode
int setvbuf(FILE *stream, char *buf, int mode, size_t size) {
   if (mode != _IONBF && mode != _IOLBF && mode != _IOFBF) {
      return -1;
   }
   stream->mode = mode;
   stream->pos = 0;
   if (stream->buffer != (char *)0 && stream->bufown == true) {
      delete[] stream->buffer;
   }

   switch (mode) {
   case _IONBF:
      stream->buffer = (char *)0;
      stream->size = 0;
      stream->bufown = false;
      break;
   case _IOLBF:
   case _IOFBF:
      if (buf != (char *)0) {
         stream->buffer = buf;
         stream->size = size;
         stream->bufown = false;
      } else {
         stream->buffer = new char[BUFSIZ];
         stream->size = BUFSIZ;
         stream->bufown = true;
      }
      break;
   }
   return 0;
}

// setbuf
// Sets the buffering mode and the size of the buffer for a stream
// Will use the defualt buffer size of 8192 if a buffer is null
void setbuf(FILE *stream, char *buf) {
   setvbuf(stream, buf, (buf != (char *)0) ? _IOFBF : _IONBF, BUFSIZ);
}

// *fopen
// Opens a file with the specified mode and returns a file pointer.
// It uses the open system call to open the file and sets the buffering mode to
// fully
//  buffered with a buffer size of 8192 using setvbuf.
FILE *fopen(const char *path, const char *mode) {
   FILE *stream = new FILE();
   setvbuf(stream, (char *)0, _IOFBF, BUFSIZ);

   // fopen( ) mode
   // r or rb = O_RDONLY
   // w or wb = O_WRONLY | O_CREAT | O_TRUNC
   // a or ab = O_WRONLY | O_CREAT | O_APPEND
   // r+ or rb+ or r+b = O_RDWR
   // w+ or wb+ or w+b = O_RDWR | O_CREAT | O_TRUNC
   // a+ or ab+ or a+b = O_RDWR | O_CREAT | O_APPEND

   switch (mode[0]) {
   case 'r':
      if (mode[1] == '\0') { // r
         stream->flag = O_RDONLY;
      } else if (mode[1] == 'b') {
         if (mode[2] == '\0') { // rb
            stream->flag = O_RDONLY;
         } else if (mode[2] == '+') { // rb+
            stream->flag = O_RDWR;
         }
      } else if (mode[1] == '+') { // r+  r+b
         stream->flag = O_RDWR;
      }
      break;
   case 'w':
      if (mode[1] == '\0') { // w
         stream->flag = O_WRONLY | O_CREAT | O_TRUNC;
      } else if (mode[1] == 'b') {
         if (mode[2] == '\0') { // wb
            stream->flag = O_WRONLY | O_CREAT | O_TRUNC;
         } else if (mode[2] == '+') { // wb+
            stream->flag = O_RDWR | O_CREAT | O_TRUNC;
         }
      } else if (mode[1] == '+') { // w+  w+b
         stream->flag = O_RDWR | O_CREAT | O_TRUNC;
      }
      break;
   case 'a':
      if (mode[1] == '\0') { // a
         stream->flag = O_WRONLY | O_CREAT | O_APPEND;
      } else if (mode[1] == 'b') {
         if (mode[2] == '\0') { // ab
            stream->flag = O_WRONLY | O_CREAT | O_APPEND;
         } else if (mode[2] == '+') { // ab+
            stream->flag = O_RDWR | O_CREAT | O_APPEND;
         }
      } else if (mode[1] == '+') { // a+  a+b
         stream->flag = O_RDWR | O_CREAT | O_APPEND;
      }
      break;
   }

   mode_t open_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

   if ((stream->fd = open(path, stream->flag, open_mode)) == -1) {
      delete stream;
      printf("fopen failed\n");
      stream = NULL;
   }

   return stream;
}

// fpurge discards any buffered data without synchronizing it to the file.
// For reads this drops prefetched input, and for writes it drops staged output.
int fpurge(FILE *stream) {
   stream->pos = 0;
   stream->actual_size = 0;
   return 0;
}

// fflush synchronizes the stream state with the file descriptor.
// Writes push staged output to the file, while reads discard unread buffered
// input and seek the fd back so the kernel offset matches the logical stream
// position again.
int fflush(FILE *stream) {
   if (stream == NULL) {
      return EOF;
   }

   if (stream->lastop == 'w') {
      int total = 0;
      while (total < stream->pos) {
         int nWritten =
             write(stream->fd, stream->buffer + total, stream->pos - total);
         if (nWritten <= 0) {
            return EOF;
         }
         total += nWritten;
      }
      stream->pos = 0;
      stream->actual_size = 0;
      return 0;
   }

   if (stream->lastop == 'r' && stream->mode != _IONBF &&
       stream->actual_size > stream->pos) {
      int unread = stream->actual_size - stream->pos;
      if (lseek(stream->fd, -unread, SEEK_CUR) == -1) {
         return EOF;
      }
      stream->pos = 0;
      stream->actual_size = 0;
   }

   return 0;
}

// fread supports buffered and unbuffered reads from the file into a provided
// buffer. It attempts to read size * nmemb bytes and returns the number of
// full elements read, which may be smaller on EOF or read error.
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
   if (size == 0 || nmemb == 0) {
      return 0;
   }

   // if there is still data from a write flush it before reading
   if (stream->lastop == 'w' && fflush(stream) == EOF) {
      return 0;
   }

   char *out = (char *)ptr;
   size_t total_requested = size * nmemb;
   size_t total_read = 0;
   stream->lastop = 'r';

   // unbuffered read
   if (stream->mode == _IONBF || stream->buffer == (char *)0) {
      while (total_read < total_requested) {
         int nRead =
             read(stream->fd, out + total_read, total_requested - total_read);
         if (nRead < 0) {
            break;
         }
         if (nRead == 0) {
            stream->eof = true;
            break;
         }
         total_read += nRead;
      }
      return total_read / size;
   }

   // buffered read
   while (total_read < total_requested) {
      // If all buffered bytes have been consumed, refill the stream buffer
      // from the file descriptor.
      if (stream->pos >= stream->actual_size) {
         int nRead = read(stream->fd, stream->buffer, stream->size);

         // On read error or EOF, clear the buffered read state and stop.
         // The function will return however many full elements were read so
         // far.
         if (nRead < 0) {
            stream->pos = 0;
            stream->actual_size = 0;
            break;
         }
         if (nRead == 0) {
            stream->eof = true;
            stream->pos = 0;
            stream->actual_size = 0;
            break;
         }
         stream->pos = 0;
         stream->actual_size = nRead;
      }

      // Copy up to chunk bytes from the internal stream buffer into
      // the provided output buffer.
      int available = stream->actual_size - stream->pos;
      size_t remaining = total_requested - total_read;
      int chunk = (available < (int)remaining) ? available : remaining;
      bcopy(stream->buffer + stream->pos, out + total_read, chunk);
      stream->pos += chunk;
      total_read += chunk;
   }

   return total_read / size;
}

// fwrite supports buffered and unbuffered writes. It attempts to write
// size * nmemb bytes from the caller-provided buffer into the stream and
// returns the number of full elements written.
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
   if (size == 0 || nmemb == 0) {
      return 0;
   }

   // If the stream was previously reading, discard any unread buffered input
   // and realign the file descriptor before switching to writes.
   if (stream->lastop == 'r' && fflush(stream) == EOF) {
      return 0;
   }

   const char *in = (const char *)ptr;
   size_t total_requested = size * nmemb;
   size_t total_written = 0;
   stream->lastop = 'w';
   stream->eof = false;

   if (stream->mode == _IONBF || stream->buffer == (char *)0) {
      while (total_written < total_requested) {
         int nWritten = write(stream->fd, in + total_written,
                              total_requested - total_written);
         if (nWritten <= 0) {
            break;
         }
         total_written += nWritten;
      }
      return total_written / size;
   }

   // Buffered writes stage data in the stream buffer and flush it to the
   // file descriptor whenever the buffer becomes full.
   while (total_written < total_requested) {
      if (stream->pos == stream->size && fflush(stream) == EOF) {
         break;
      }

      int space = stream->size - stream->pos;
      size_t remaining = total_requested - total_written;
      int chunk = (space < (int)remaining) ? space : remaining;
      bcopy(in + total_written, stream->buffer + stream->pos, chunk);
      stream->pos += chunk;
      total_written += chunk;
   }

   return total_written / size;
}

// fgetc is a one-byte wrapper around fread. It returns the byte as an int or
// EOF if no byte can be read.
int fgetc(FILE *stream) {
   unsigned char c;
   if (fread(&c, sizeof(char), 1, stream) != 1) {
      return EOF;
   }
   return c;
}

// fputc is a one-byte wrapper around fwrite. It returns the written byte or
// EOF if the write fails.
int fputc(int c, FILE *stream) {
   unsigned char out = c;
   if (fwrite(&out, sizeof(char), 1, stream) != 1) {
      return EOF;
   }
   return out;
}

// fgets reads at most size - 1 bytes one character at a time, stopping after
// a newline or EOF, and always null-terminates the returned string.
char *fgets(char *str, int size, FILE *stream) {
   if (size <= 0) {
      return NULL;
   }

   if (size == 1) {
      str[0] = '\0';
      return str;
   }

   int i = 0;
   while (i < size - 1) {
      int c = fgetc(stream);
      if (c == EOF) {
         break;
      }
      str[i++] = c;
      if (c == '\n') {
         break;
      }
   }

   if (i == 0) {
      return NULL;
   }

   str[i] = '\0';
   return str;
}

// fputs writes the full null-terminated string to the stream but does not
// append an extra newline.
int fputs(const char *str, FILE *stream) {
   size_t len = strlen(str);
   return (fwrite(str, sizeof(char), len, stream) == len) ? len : EOF;
}

int feof(FILE *stream) { return stream->eof == true; }

// fseek moves the file position after first reconciling any buffered state.
// Pending writes are flushed, and SEEK_CUR after buffered reads is adjusted so
// the seek is relative to the logical stream position rather than the advanced
// kernel fd offset.
int fseek(FILE *stream, long offset, int whence) {
   if (stream->lastop == 'w' && fflush(stream) == EOF) {
      return EOF;
   }

   if (stream->lastop == 'r' && whence == SEEK_CUR) {
      offset -= stream->actual_size - stream->pos;
   }

   if (lseek(stream->fd, offset, whence) == -1) {
      return EOF;
   }

   stream->pos = 0;
   stream->actual_size = 0;
   stream->lastop = 0;
   stream->eof = false;
   return 0;
}

// fclose flushes pending buffered output, closes the underlying file
// descriptor, releases any owned stream buffer, and deletes the FILE object.
int fclose(FILE *stream) {
   int ret = 0;
   if (stream->lastop == 'w' && fflush(stream) == EOF) {
      ret = EOF;
   }
   if (close(stream->fd) == -1) {
      ret = EOF;
   }
   if (stream->buffer != (char *)0 && stream->bufown == true) {
      delete[] stream->buffer;
   }
   delete stream;
   return ret;
}
