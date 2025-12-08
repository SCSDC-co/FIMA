#include "../../include/helpers/get_directories_entries.h"

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace fima {

namespace helpers {

std::vector<fs::directory_entry> get_directories_entries(const fs::path &path) {
    std::vector<fs::directory_entry> directory_content;

    for (const fs::directory_entry &entry : fs::directory_iterator(path)) {
        std::string name{entry.path().filename().string()};

        if (name[0] == '.') {
            continue;
        }

        directory_content.push_back(entry);
    }

    return directory_content;
}

std::vector<fs::directory_entry>
get_directories_entries_recursive(const fs::path &path) {
    std::vector<fs::directory_entry> directory_content;

    for (const auto &entry : fs::directory_iterator(path)) {
        directory_content.push_back(entry);

        if (fs::is_directory(entry)) {
            auto sub_entries = get_directories_entries_recursive(entry);
            directory_content.insert(directory_content.end(),
                                     sub_entries.begin(), sub_entries.end());
        }
    }

    return directory_content;
}

} // namespace helpers

} // namespace fima
