#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void fima_rename(fs::path old_name, fs::path new_name) {
    if (!fs::exists(old_name)) {
        std::cout << "The directory/file you want to rename does not exists!"
                  << '\n';
        std::cout << old_name << '\n';

        return;
    }

    if (fs::exists(new_name)) {
        std::cout << new_name << " does already exists!";

        return;
    }

    rename(old_name, new_name);

    std::cout << old_name << " renamed to " << new_name << '\n';

    return;
}
