#include "include/Obj.hpp"

#include <iostream>
#include <string>

int main(const int argc, const char *argv[]) {
    if (argc == 1) {
        std::cerr << "Usage: " << argv[0] << " [filename] [option]" << std::endl;
        return 1;
    }

    const Obj object(argv[1]);

    if (argc == 3 && std::string(argv[2]) == "--verbose") {
        std::cout << "Data loaded : " << std::endl;
        std::cout << object << std::endl;
    }

    return 0;
}