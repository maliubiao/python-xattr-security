#! /usr/bin/env python
from distutils.core import setup, Extension

m = Extension("xattr",
        sources=["xattr.c"])
        

setup(name = "xattr",
        description = "python library for linux acl, capability",
        ext_modules = [m],
        py_modules = ["acl"]
        )
