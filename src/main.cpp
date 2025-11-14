#include <filesystem>
#include <iostream>
#include <string>

#include "../include/tui/main.h"

int main(int argc, char *argv[]) {
    std::string path;

    if (argc < 2) {
        path = ".";
    } else if (std::filesystem::is_directory(argv[1])) {
        path = argv[1];
    } else {
        std::cout << "You need to specify a directory!" << std::endl;
        std::cout << argv[1] << "Is not a valid directory" << std::endl;

        return 1;
    }

    printTui(path);

    return 0;
}
