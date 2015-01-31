from distutils.core import setup, Extension
module = Extension('chizer', ['chizer_python.c','chizer.c'])
setup(name='chizer',
      version='1.0',
      ext_modules=[module],
     )