#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void get_perms(fs::path path) {
    auto st = fs::status(path);

    fs::perms permissions{st.permissions()};

    auto show = [=](char op, fs::perms perm) {
        std::cout << (fs::perms::none == (perm & permissions) ? '-' : op);
    };

    if (fs::is_regular_file(st)) {
        std::cout << "-";
    } else if (fs::is_directory(st)) {
        std::cout << "d";
    } else if (fs::is_symlink(st)) {
        std::cout << "l";
    } else if (fs::is_character_file(st)) {
        std::cout << "c";
    } else if (fs::is_block_file(st)) {
        std::cout << "b";
    } else if (fs::is_fifo(st)) {
        std::cout << "p";
    } else if (fs::is_socket(st)) {
        std::cout << "s";
    } else {
        std::cout << "?"; // unknown type
    }

    show('r', fs::perms::owner_read);
    show('w', fs::perms::owner_write);
    show('x', fs::perms::owner_exec);
    show('r', fs::perms::group_read);
    show('w', fs::perms::group_write);
    show('x', fs::perms::group_exec);
    show('r', fs::perms::others_read);
    show('w', fs::perms::others_write);
    show('x', fs::perms::others_exec);
    std::cout << '\n';
}
