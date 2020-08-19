{
  "targets": [
    {
      "target_name": "bff",
      "cflags!": [ "--std=c++11 -fno-exceptions -fno-strict-aliasing" ],
      "cflags_cc!": [ "--std=c++11 -fno-exceptions -fno-strict-aliasing" ],
      "include_dirs": [
        ".",
        "./libraries/libbf",
        "./libraries/libbloom",
        "./libraries/libbloom/murmur2",
        "<!@(node -p \"require('node-addon-api').include\")"
        
      ],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "sources": [
        "<!@(node -p \"require('fs').readdirSync('./libraries/libbf/src').map(f=>'libraries/libbf/src/'+f).join(' ')\")",
        "<!@(node -p \"require('fs').readdirSync('./libraries/libbf/src/bloom_filter').map(f=>'libraries/libbf/src/bloom_filter/'+f).join(' ')\")",
        "<!@(node -p \"require('fs').readdirSync('./libraries/libbloom').map(f=>'libraries/libbloom/'+f).join(' ')\")",
        "<!@(node -p \"require('fs').readdirSync('./libraries/libbloom/murmur2').map(f=>'libraries/libbloom/murmur2/'+f).join(' ')\")",
        "./bloom-napi.cc"
      ],
    }
  ]
}
