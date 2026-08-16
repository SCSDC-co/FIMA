from conan import ConanFile


class Fima(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    requires = (
        "ftxui/7.0.3",
        "libgit2/1.9.1",
        "libzippp/7.1-1.10.1",
        "cli11/2.6.2",
        "rang/3.2",
        "p-ranav-glob/0.0.1",
        "tomlplusplus/3.4.0",
    )

    def requirements(self):
        if self.settings.os != "Windows":
            self.requires("libmagic/5.45")
