from setuptools import setup, Extension

stella_module = Extension(
    'stella',
    sources=[
        'cpp_src/node.cpp',
        'cpp_src/edge.cpp',
        'py_src/stella_extension.cpp',
        'py_src/node.cpp',
        'py_src/edge.cpp',
        'py_src/graph.cpp',
        'py_src/adj_list.cpp',
        'py_src/adj_matrix.cpp'
    ],
    include_dirs=[
        '/usr/include/python3.11',
        'py_src',
        'cpp_src'
    ],
    language='c++',
    extra_compile_args=['-std=c++11'],
)

setup(
    name='stella',
    version='0.1',
    description='A C++ extension for graph algorithms',
    ext_modules=[stella_module],
)
