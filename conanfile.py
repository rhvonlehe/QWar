from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake

class QWar(ConanFile):
    name = "QWar"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    requires = "boost/1.77.0", "qt/6.4.2"
    generators = "CMakeToolchain", "CMakeDeps"


    def configure(self):
        # requiring qt doesn't get all modules, some have to be explicitly added as
        # shown here
        self.options["qt"].qtdeclarative = True
        self.options["qt"].shared = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
