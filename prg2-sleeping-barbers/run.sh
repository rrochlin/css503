g++ -Wall -Wpedantic -Werror -Wextra -fsanitize=address -pthread driver.cpp shop.cpp -o bin/sleeping-barbers && ./bin/sleeping-barbers 1 1 10 1000
