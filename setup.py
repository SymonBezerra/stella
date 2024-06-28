from setuptools import setup, Extension

stella_module = Extension(
    'stella',
    sources=['stella_extension.cpp'],
    include_dirs=['/usr/include/python3.11'],
    language='c++',
    extra_compile_args=['-std=c++11'],
)

setup(
    name='stella',
    version='0.1',
    description='A C++ extension for graph algorithms',
    ext_modules=[stella_module],
)
