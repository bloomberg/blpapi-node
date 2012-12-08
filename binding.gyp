{
  'targets': [
    {
      'target_name': 'blpapijs',
      'sources': [ 'blpapijs.cpp' ],
      'include_dirs': [
        '<(module_root_dir)/deps/blpapi/include'
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="win"', {
          'msvs_settings': {
            'VCCLCompilerTool': {
              'AdditionalOptions': [ '/EHsc' ]
            }
          },
          'conditions': [
            ['target_arch=="ia32"', {
              'libraries': [
                '<(module_root_dir)/deps/blpapi/win/blpapi3_32.lib'
              ]
            }],
            ['target_arch=="x64"', {
              'libraries': [
                '<(module_root_dir)/deps/blpapi/win/blpapi3_64.lib'
              ]
            }]
          ]
        }],
        ['OS=="linux"', {
          'ldflags': [
            '-Wl,-R<(module_root_dir)/deps/blpapi/linux',
            '-L<(module_root_dir)/deps/blpapi/linux'
          ],
          'conditions': [
            ['target_arch=="ia32"', {
              'libraries': [ '-lblpapi3_32' ]
            }],
            ['target_arch=="x64"', {
              'libraries': [ '-lblpapi3_64' ]
            }]
          ]
        }],
        ['OS=="mac"', {
          'ldflags': [
            '-Wl,-rpath,<(module_root_dir)/deps/blpapi/osx',
            '-L<(module_root_dir)/deps/blpapi/osx'
          ],
          'conditions': [
            ['target_arch=="ia32"', {
              'libraries': [ '-lblpapi3_32' ],
              'xcode_settings': { 'ARCHS': [ 'i386' ] }
            }],
            ['target_arch=="x64"', {
              'libraries': [ '-lblpapi3_64' ],
              'xcode_settings': { 'ARCHS': [ 'x86_64' ] }
            }]
          ],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_ENABLE_CPP_RTTI': 'NO',
            'INSTALL_PATH': '@rpath',
            'LD_DYLIB_INSTALL_NAME': '',
            'OTHER_LDFLAGS': [
              '-Wl,-search_paths_first',
              '-Wl,-rpath,<(module_root_dir)/deps/blpapi/osx',
              '-L<(module_root_dir)/deps/blpapi/osx'
            ]
          }
        }]
      ]
    }
  ]
}
