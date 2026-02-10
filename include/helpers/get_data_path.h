#include <cstdlib>
#include <filesystem>
#include <string>

namespace fima {

namespace helpers {

inline std::filesystem::path
get_application_data_path()
{
#if defined(_WIN32) || defined(_WIN64)
    return fs::path(std::getenv("APPDATA"));
#elif defined(__APPLE__) || defined(__MACH__)
    return fs::path(std::string(std::getenv("HOME")) + "/Library/Application Support");
#elif defined(__linux__) || defined(__unix) || defined(__unix__) || defined(__FreeBSD__)
    return std::filesystem::path(std::string(std::getenv("HOME")) + "/.local/share");
#endif
}

}

}
