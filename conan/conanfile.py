from conans import ConanFile, CMake


class HighloadServer(ConanFile):
    name = 'HighloadServer'
    version = '1.0.0'

    settings = [
        'os',
        'build_type',
        'arch',
    ]

    def configure(self) -> None:
        self.generators = [
            'cmake',
        ]

        self.options['poco'].enable_data_mysql = True
        self.options['poco'].enable_netssl = True
        self.options['poco'].enable_net = True

    def requirements(self) -> None:
        self.requires('nlohmann_json/3.11.2')
        self.requires('poco/1.12.4')

    def build(self) -> None:
        cmake = CMake(self)
        cmake.definitions['BUILD_SHARED_LIBS'] = 'ON' if self.options.shared else 'OFF'

        cmake.configure()
        cmake.build()
