from setuptools import Extension, setup

module = Extension("symnmfsp", sources=['symnmf.c','symnmfmodule.c'])
setup(name='symmnfsp',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])
