{
  'targets': [
    {
      'target_name': 'blpapijs',
      'type': 'shared_library',
      'sources': [ 'blpapijs.cpp' ],
      'include_dirs': [
        '<(module_root_dir)/deps/blpapi/include'
      ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="win"', {
          'conditions': [
            ['target_arch=="ia32"', {
              'libraries': [
                '<(module_root_dir)/deps/blpapi/lib/blpapi3_32.lib'
              ]
            }],
            ['target_arch=="x64"', {
              'libraries': [
                '<(module_root_dir)/deps/blpapi/lib/blpapi3_64.lib'
              ]
            }]
          ]
        }],
        ['OS=="linux"', {
          'ldflags': [
            '-Wl,-R<(module_root_dir)/deps/blpapi/lib',
            '-L<(module_root_dir)/deps/blpapi/lib'
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
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}
