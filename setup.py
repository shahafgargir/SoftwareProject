from setuptools import Extension, setup

module = Extension("mysymnmfsp", sources=['symnmf.c','symnmfmodule.c'])
setup(name='mysymmnfsp',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])
