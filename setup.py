#! /usr/bin/env python
from distutils.core import setup, Extension

xattrm = Extension("xattr",
        sources=["xattr.c"],
        )
        
capm = Extension("_cap",
        sources=["cap.c"],
        )
prctlm = Extension("prctl",
        sources=["prctl.c"],
        )

setup(name = "xattr",
        description = "python library for linux acl, capability",
        ext_modules = [xattrm, capm, prctlm],
        py_modules = ["acl"]
        )
