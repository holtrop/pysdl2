#!/usr/bin/env python

import os
import re
from distutils.core import setup, Extension

def get_sources_from_dir(d):
    return map(lambda x: os.path.join(d, x),
        filter(lambda x: re.search(r'\.cc?$', x), os.listdir(d)))

sdl_sources = get_sources_from_dir('sdl')
sdl_module = Extension('SDL',
        sources = sdl_sources,
        extra_compile_args = ['-std=gnu99', '-I%s' % os.getcwd()],
        libraries = ['SDL'])

setup(name = 'PySDL2',
        version = '0.1',
        description = 'Python Bindings for SDL',
        author = 'Josh Holtrop',
        author_email = 'jholtrop@gmail.com',
        ext_modules = [sdl_module],
        long_description = 'Python Bindings for SDL',
        license = 'LGPL v2.1')
