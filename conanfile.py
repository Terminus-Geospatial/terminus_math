#    File:    conanfile.py
#    Author:  Marvin Smith
#    Date:    7/8/2023
from conan import ConanFile, tools
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools.files import copy

class ConanProject(ConanFile):

    name = "terminus_math"
    version = "0.0.15"

    license = "Terminus Proprietary"
    author  = "Marvin Smith <marvin_smith1@me.com>"
    url     = "https://bitbucket.org/msmith81886/terminus-outcome/src"
    description = "Straightforward Math APIs"
    topics = ("terminus","math")

    options = { "shared": [True, False],
                "with_tests": [True, False],
                "with_docs": [True, False],
                "with_coverage": [True, False]
    }

    default_options = { "shared": True,
                        "with_tests": True,
                        "with_docs": True,
                        "with_coverage": False
    }

    settings = "os", "compiler", "build_type", "arch"

    def build_requirements(self):
        self.test_requires("gtest/1.17.0")
        self.tool_requires("terminus_cmake/1.0.8")

    def requirements(self):
        self.requires("boost/1.89.0")
        self.requires("eigen/3.4.0")
        self.requires("terminus_core/0.0.12")
        self.requires("terminus_log/0.0.13")
        self.requires("terminus_outcome/0.0.12")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["NAME_FROM_CONANFILE"]        = self.name
        tc.variables["VERSION_FROM_CONANFILE"]     = self.version
        tc.variables["DESCRIPTION_FROM_CONANFILE"] = self.description
        tc.variables["URL_FROM_CONANFILE"]         = self.url

        tc.variables["TERMINUS_MATH_ENABLE_TESTS"]    = self.options.with_tests
        tc.variables["TERMINUS_MATH_ENABLE_DOCS"]     = self.options.with_docs
        tc.variables["TERMINUS_MATH_ENABLE_COVERAGE"] = self.options.with_coverage
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["terminus_math"]

    def package_id(self):
        self.info.clear()

    def export_sources(self):

        for p in [ "CMakeLists.txt", "include/*", "src/*", "test/*", "README.md" ]:
            copy( self,
                  p,
                  self.recipe_folder,
                  self.export_sources_folder )
