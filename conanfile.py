from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout

class QWar(ConanFile):
    name = "QWar"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    requires = "boost/1.77.0", "qt/6.5.3"
    generators = "CMakeToolchain", "CMakeDeps"


    def configure(self):
        # requiring qt doesn't get all modules, some have to be explicitly added as
        # shown here
        self.options["qt"].qtdeclarative = True
        self.options["qt"].shared = True

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self, src_folder=".")
