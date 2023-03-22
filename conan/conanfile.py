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

        self.options['poco'].enable_data_mysql = self.options.with_mysql
        self.options['poco'].enable_netssl = True
        self.options['poco'].enable_net = True
        self.options['poco'].shared = False
        self.options['boost'].shared = True

    def requirements(self) -> None:
        self.requires('nlohmann_json/3.11.2')
        self.requires('libmysqlclient/8.0.31')
        self.requires('boost/1.81.0')
        self.requires('poco/1.12.4')

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
            self.copy(library, dst='lib', src='lib', keep_path=False)
