#include <iostream>
#include <string>

int main(int argc, char **argv) {
   if (argc != 6) {
      std::cerr << "requires 5 args\n";
   }
   std::string serverName = argv[0];
   int port = std::stoi(argv[1]);
   int repetition = std::stoi(argv[2]);
   int nbufs = std::stoi(argv[3]);
   int bufsize = std::stoi(argv[4]);
   int type = std::stoi(argv[5]);
}
