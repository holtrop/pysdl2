#!/usr/bin/env python

import os
import re
from distutils.core import setup, Extension

def get_sources_from_dir(d):
    return map(lambda x: os.path.join(d, x),
        filter(lambda x: re.search(r'\.cc?$', x), os.listdir(d)))

extra_compile_args = ['-std=gnu99', '-I%s' % os.getcwd()]
extra_link_args = []
if os.path.exists('C:\\apps\\SDL-1.2.14'):
    extra_compile_args.append('-Ic:\\apps\\SDL-1.2.14\\include')
    extra_link_args.append('-Lc:\\apps\\SDL-1.2.14\\lib')
sdl_sources = get_sources_from_dir('sdl')
sdl_module = Extension('SDL',
        sources = sdl_sources,
        extra_compile_args = extra_compile_args,
        extra_link_args = extra_link_args,
        libraries = ['SDL'])

setup(name = 'pysdl2',
        version = '0.3',
        description = 'Python Bindings for SDL',
        author = 'Josh Holtrop',
        author_email = 'pysdl2@gmail.com',
        ext_modules = [sdl_module],
        long_description = 'Python Bindings for SDL',
        license = 'LGPL v2.1')
