from distutils.core import setup, Extension
module = Extension('chizer', ['chizer_python.c','chizer.c']) # 最初の引数がモジュール名
setup(name='chizer',
      version='1.0',
      ext_modules=[module],
     )