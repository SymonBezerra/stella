from setuptools import setup, Extension

stella_module = Extension(
    '_stella',
    sources=[
        'cpp_src/node.cpp',
        'cpp_src/edge.cpp',
        'py_src/stella_extension.cpp',
        'py_src/node.cpp',
        'py_src/edge.cpp',
        'py_src/graph.cpp',
        'py_src/adj_list.cpp',
        'py_src/adj_matrix.cpp',
    ],
    include_dirs=[
        'py_src',
        'cpp_src'
    ],
    language='c++',
    extra_compile_args=['-std=c++14'],
)

setup(
    name='_stella',
    version='0.1',
    description='A C++ extension for graph algorithms',
    ext_modules=[stella_module],
)
