import os
import platform
import subprocess
import sys
from pprint import pprint
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from typing import List

# Filename for the C extension module library
c_module_name = '_navipy'

# Command line flags forwarded to CMake
cmake_cmd_args = []
for f in sys.argv:
    if f.startswith('-D'):
        cmake_cmd_args.append(f)

for f in cmake_cmd_args:
    sys.argv.remove(f)


def _get_env_variable(name: str, default: str = 'OFF'):
    if name not in os.environ.keys():
        return default
    return os.environ[name]


class CMakeExtension(Extension):

    def __init__(self, name: str, cmake_lists_dir: str = '.', sources: List[str] = None, **kwargs):
        sources = [] if not sources else sources

        Extension.__init__(self, name, sources=sources, **kwargs)

        self.cmake_lists_dir = os.path.abspath(cmake_lists_dir)


class CMakeBuild(build_ext):

    build_temp: str

    def build_extensions(self):
        try:
            subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError('Cannot find CMake executable')

        for ext in self.extensions:
            extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
            cfg = 'Debug' if _get_env_variable('DISPTOOLS_DEBUG') == 'ON' else 'Release'

            cmake_args = [
                '-DDISPTOOLS_PYTHON_SUPPORT=ON',
                f'-DNAVIPY_PYTHON_CXX_MODULE_NAME={c_module_name}',
                f'-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}',
                f'-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_{cfg.upper()}={self.build_temp}',
                f'-DPYTHON_EXECUTABLE={sys.executable}',
            ]

            if platform.system() == 'Windows':
                plat = 'x64' if platform.architecture()[0] == '64bit' else 'Win32'
                cmake_args += [
                    '-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE',
                    '-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_{}={}'.format(cfg.upper(), extdir),
                ]
                if self.compiler.compiler_type == 'msvc':
                    cmake_args += [f'-DCMAKE_GENERATOR_PLATFORM={plat}']
                else:
                    cmake_args += ['-G', 'MinGW Makefiles']

                cmake_args += cmake_cmd_args

                pprint(cmake_args)

                if not os.path.exists(self.build_temp):
                    os.makedirs(self.build_temp)

                # Config and build the extension
                subprocess.check_call(
                    ['cmake', ext.cmake_lists_dir] + cmake_args,
                    cwd=self.build_temp)
                subprocess.check_call(
                    ['cmake', '--build', '.', '--config', cfg],
                    cwd=self.build_temp)


# The following line is parsed by Sphinx
version = '0.1.0'

setup(
    name='navipy',
    version=version,
    description='Some bread-and-butter algorithms for motion planning',
    author='William Bell',
    author_email='wjbell97@gmail.com',
    keywords=['path planning', 'motion planning'],
    install_requires=['numpy>=1.15.1'],
    ext_modules=[CMakeExtension(c_module_name)],
    cmdclass={'build_ext': CMakeBuild},
    zip_safe=False,
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: Microsoft :: Windows"
      ]
    )
