import platform
import Options
import os
from os import unlink, symlink
from os.path import exists, lexists

srcdir = '.'
blddir = 'build'
VERSION = '1.1.0'

def set_options(opt):
  opt.tool_options('compiler_cxx')
  opt.add_option('--blpapi', type = 'string', dest = 'blpapi', default = '',
                 help = 'Bloomberg API SDK directory (APIv3)')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')
  blpapi = Options.options.blpapi
  if blpapi == '':
    try: blpapi = os.environ['BLPAPI_ROOT']
    except KeyError: pass
  conf.env.append_unique('CXXFLAGS', '-shared -fPIC'.split())
  conf.env.append_unique('CXXFLAGS', '-I' + blpapi + '/include')
  conf.env.append_unique('LIBPATH', blpapi + '/Linux')
  conf.env.append_unique('LINKFLAGS', '-Wl,-R' + blpapi + '/Linux')
  if platform.architecture()[0] == '64bit':
    conf.check(lib='blpapi3_64', uselib_store='BLPAPI')
    conf.env.append_unique('LIB', 'blpapi3_64')
  else:
    conf.check(lib='blpapi3_32', uselib_store='BLPAPI')
    conf.env.append_unique('LIB', 'blpapi3_32')
  conf.check(header_name = 'blpapi_defs.h', use = 'BLPAPI', mandatory = True,
             errmsg = "Bloomberg API SDK not found.")

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = 'blpapijs'
  obj.source = 'blpapijs.cpp'

def shutdown(bld):
  if Options.commands['clean'] and not Options.commands['build']:
    if lexists('blpapijs.node'):
      unlink('blpapijs.node')
  elif Options.commands['build']:
    if exists('build/Release/blpapijs.node') and not lexists('blpapijs.node'):
      symlink('build/Release/blpapijs.node', 'blpapijs.node')
