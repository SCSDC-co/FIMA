import os

from conan import ConanFile
from conan.tools.cmake import CMakeToolchain


class Fima(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = (
        "CMakeDeps",
        "VirtualRunEnv",
    )

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

    def generate(self):
        tc = CMakeToolchain(self)

        if self.settings.os != "Windows":
            magic = self.dependencies["libmagic"]

            tc.variables["FIMA_MAGIC_DATABASE"] = os.path.join(
                magic.package_folder,
                "res",
                "magic.mgc",
            )

        tc.generate()
