from conan import ConanFile


class Fima(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    requires = (
        "nlohmann_json/3.12.0",
        "ftxui/6.1.9",
        "libgit2/1.9.1",
        "libarchive/3.8.7",
        "cli11/2.6.2",
        "termcolor/2.1.0",
    )
