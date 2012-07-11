{
  'targets': [
    {
      'target_name': 'blpapijs',
      'type': 'shared_library',
      'sources': [ 'blpapijs.cpp' ],
      'include_dirs': [
        '<(module_root_dir)/deps/blpapi/include'
      ],
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
          'cflags': [
            '-shared', '-fPIC'
          ],
          'libraries': [
            '-Wl,-R<(module_root_dir)/deps/blpapi/lib',
            '-L<(module_root_dir)deps/blpapi/lib'
          ],
	  'conditions': [
            ['target_arch=="ia32"', {
              'libraries': [ '-lblpapi3_32' ]
            }],
            ['target_arch=="x64"', {
              'libraries': [ '-lblpapi3_64' ]
            }]
          ]
        }]
      ]
    }
  ]
}
