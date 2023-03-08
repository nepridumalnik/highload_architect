from conans import ConanFile, CMake


class HighloadServer(ConanFile):
    name = 'HighloadServer'
    version = '1.0.0'

    settings = [
        'os',
        'build_type',
        'arch',
    ]

    options = {
        'shared': [True, False],
        'with_mysql': [True, False],
    }

    default_options = {
        'shared': True,
        'with_mysql': True,
    }

    def configure(self) -> None:
        self.generators = [
            'cmake',
        ]

        self.options['soci'].with_mysql = self.options.with_mysql
        self.options["boost"].shared = True

    def requirements(self) -> None:
        self.requires('nlohmann_json/3.11.2')
        self.requires('boost/1.81.0')
        self.requires('soci/4.0.3')

    def build(self) -> None:
        cmake = CMake(self)
        cmake.definitions['BUILD_SHARED_LIBS'] = 'ON' if self.options.shared else 'OFF'

        cmake.configure()
        cmake.build()

    def imports(self) -> None:
        libraries = [
            '*boost_system*',
            '*boost_thread*',
        ]

        for library in libraries:
            self.copy(library, dst='bin', src='bin', keep_path=False)

    def package(self) -> None:
        self.copy('*.h', dst='include', src='include')
        self.copy('*.lib', dst='lib', src='lib')
        self.copy('*.dll', dst='bin', src='bin')
        self.copy('*.so', dst='lib', src='lib')
        self.copy('*.dylib', dst='lib', src='lib')
        self.copy('*.a', dst='lib', src='lib')
