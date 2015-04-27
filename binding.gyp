{
  'variables' : {
    'node_bin_dir': '<!(npm bin)',
    'tsc': '<(node_bin_dir)/tsc',
    'tslint': '<(node_bin_dir)/tslint',
    'mocha': '<(node_bin_dir)/mocha',
    'blpapi_ts': '<(module_root_dir)/blpapi.ts',
    'blpapi_js': '<(module_root_dir)/blpapi.js',
    'blpapijs_dts': '<(module_root_dir)/etc/blpapijs.d.ts',
    'blpapi_mock_ts': '<(module_root_dir)/test/lib/mock-blpapijs.ts',
    'blpapi_mock_js': '<(module_root_dir)/test/lib/mock-blpapijs.js',
    'blpapi_test_ts': '<(module_root_dir)/test/blpapi.test.ts',
    'blpapi_test_js': '<(module_root_dir)/test/blpapi.test.js',
  },
  'targets' : [
    {
      # Compile typescript code
      'target_name': 'tsc',
      'type': 'none',
      'dependencies': ['copy_dts'],
      'actions': [
        {
          'action_name': 'ts_comp',
          'message': 'Running tsc...',
          'inputs': ['<(blpapi_ts)', '<(blpapijs_dts)'],
          'outputs': ['<(blpapi_js)'],
          'action': [
            '<(tsc)', '--module', 'commonjs', '--target', 'ES5',
            '--noImplicitAny', '--noEmitOnError', '--sourceMap',
            '<(blpapi_ts)'
          ]
        }
      ],
    },
    {
      # Compile testing typescript code
      'target_name': 'tsc_test',
      'type': 'none',
      'dependencies': ['copy_dts'],
      'actions': [
        {
          'action_name': 'ts_comp_test',
          'message': 'Running tsc for testing code...',
          'inputs': ['<(blpapi_mock_ts)', '<(blpapi_test_ts)'],
          'outputs': ['<(blpapi_mock_js)', '<(blpapi_test_js)'],
          'action': [
            '<(tsc)', '--module', 'commonjs', '--target', 'ES5',
            '--noImplicitAny', '--noEmitOnError', '--sourceMap',
            '<@(_inputs)'
          ]
        }
      ],
    },
    {  'target_name': 'copy_dts',
       'type': 'none',
        # Copy 'blapijs' typescript definition files to the build directory,
        # this is necessary to be able to import the module from typescript.
        'copies': [
          {
            'files': ['<(blpapijs_dts)'],
            'destination': '<(PRODUCT_DIR)'
          }
        ]
    },
    {
      # Run tslint, uses a dummy output file.
      'target_name': 'tslint',
      'type': 'none',
      'actions': [
      {
          'action_name': 'tslint',
          'message': 'Running tslint...',
          'inputs': ['<(blpapi_ts)', '<(blpapijs_dts)'],
          'outputs': ['<(module_root_dir)/.tslint.d'],
          'action': ['sh', '-c', '<(tslint) -f <(blpapi_ts) -f <(blpapijs_dts) && touch <@(_outputs)']
        }
      ]
    },
    {
      # Run tslint for testing file, uses a dummy output file.
      'target_name': 'tslint_test',
      'type': 'none',
      'actions': [
      {
          'action_name': 'tslint_test',
          'message': 'Running tslint for testing code...',
          'inputs': ['<(blpapi_mock_ts)', '<(blpapi_test_ts)'],
          'outputs': ['<(module_root_dir)/test/.tslint.d'],
          'action': ['sh', '-c', '<(tslint) -f <(blpapi_mock_ts) -f <(blpapi_test_ts) && touch <@(_outputs)']
        }
      ]
    },
    {
      # Run mocha tests
      'target_name': 'test',
      'type': 'none',
      'dependencies': ['blpapijs', 'tsc', 'tslint', 'tsc_test', 'tslint_test'],
      'actions': [
        {
          'action_name': 'test',
          'message': 'Running mocha tests...',
          'inputs': ['<(blpapi_ts)', '<(blpapijs_dts)', '<(blpapi_mock_ts)', '<(blpapi_test_ts)'],
          'outputs': ['<(module_root_dir)/test/.test.d'],
          'action': ['<(mocha)', '--reporter', 'spec']
        }
      ],
    },
    {
      'target_name': 'blpapijs',
      'sources': [ 'blpapijs.cpp' ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'conditions': [
        ['OS=="win"', {
          'include_dirs': [
            '<(module_root_dir)/deps/blpapi/include-3.7.9.1'
          ],
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
          'include_dirs': [
            '<(module_root_dir)/deps/blpapi/include-3.7.9.1'
          ],
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
          'include_dirs': [
            '<(module_root_dir)/deps/blpapi/include-3.8.1.1'
          ],
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
