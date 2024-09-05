from conan import ConanFile
from conan.tools.files import copy

class XWalletRecipe(ConanFile):
    name = "xwallet"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"

    # def build_requirements(self):
    #   self.tool_requires("cmake/3.26.3")
    #   self.tool_requires("ninja/1.11.1")
    #   self.test_requires("gtest/cci.20210126")

    def requirements(self):
        if self.settings.os != "Windows":
            self.requires("boost/1.76.0")

        self.requires("protobuf/3.17.1")
        self.requires("nlohmann_json/3.10.4")
        self.requires("threadpool/20140926")
        self.requires("plog/1.1.10")
        # self.requires("qt/6.5.3")
        self.requires("openssl/3.2.2")
        self.requires("gtest/1.11.0")
        self.requires("libqrencode/4.1.1")
        self.requires("zstd/1.5.5")
        self.requires("libpq/15.5")

    def configure(self):
        self.options["qt"].shared=True
        self.options["qt"].with_openal=False
        self.options["qt"].qtsvg=True
        self.options["qt"].qtdeclarative=True
        self.options["qt"].qttools=True
        self.options["qt"].qttranslations=True
        self.options["qt"].qtquickcontrols2=True
        self.options["qt"].qtshadertools=True
        self.options["qt"].qtmultimedia=True
        self.options["qt"].qtwebsockets=True
        self.options["qt"].qtwebview=True
        self.options["qt"].qtquickeffectmaker=True

        self.options["openssl"].shared=True
        self.options["libpq"].shared=True

        self.options["gtest"].shared=False

        # self.options["boost"].shared=False
        # self.options["boost"].without_container=True
        # self.options["boost"].without_context=True
        # self.options["boost"].without_contract=True
        # self.options["boost"].without_coroutine=True
        # self.options["boost"].without_fiber=True
        # self.options["boost"].without_graph=True
        # self.options["boost"].without_graph_parallel=True
        # self.options["boost"].without_json=True
        # self.options["boost"].without_math=True
        # self.options["boost"].without_mpi=True
        # self.options["boost"].without_nowide=True
        # self.options["boost"].without_python=True
        # self.options["boost"].without_serialization=True
        # self.options["boost"].without_stacktrace=True
        # self.options["boost"].without_timer=True
        # self.options["boost"].without_type_erasure=True
        # self.options["boost"].without_url=True
        # self.options["boost"].without_wave=True
        # self.options["boost"].bzip2=False

        self.options["boost"].extra_b2_flags="define=BOOST_NO_CXX98_FUNCTION_BASE=1"

    def layout(self):
        self.folders.build = "build"
        self.folders.generators = "build"

    def deploy(self):
        for require, dependency in self.dependencies.items():
            if dependency.ref.name == "openssl" or dependency.ref.name == "libpq":
                copy(self, "*.dll", src=dependency.package_folder, dst=self.deploy_folder)
