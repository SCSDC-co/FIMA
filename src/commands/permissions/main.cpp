#include "../../../include/commands/permissions/main.h"

#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

namespace fima {

void get_perms(const std::vector<fs::path> &paths) {
    for (const auto &entry : paths) {
        if (!fs::exists(entry)) {
            std::cerr << "The path doesn't exists: " << entry.string()
                      << std::endl;

            continue;
        }

        try {
            auto st = fs::status(entry);

            fs::perms permissions{st.permissions()};

            auto show = [=](char op, fs::perms perm) {
                std::cout << (fs::perms::none == (perm & permissions) ? '-'
                                                                      : op);
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
            std::cout << " " << entry.string() << '\n';
        } catch (const std::exception &ex) {
            std::cerr << "Failed to get permissions for: " << entry.string()
                      << std::endl;
            std::cerr << ex.what() << std::endl;
        }
    }
}

} // namespace fima
