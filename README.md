QC-SIMD-LDPC-DECODER
============================================================

Universal Quasi-Cyclic SIMD-LDPC decoder supporting SSE, AVX2 and AVX512.

* Vesion:   1.2
* Date:     2019.03.06
* Author:   Xu Yi

File Specification
------------------------------------------------------------

```
QC-SIMD-LDPC-DECODER
│   README.md
│   LICENSE
│
└───inc
│   │   qc_simd_ldpc_decoder.h                  // Head file
│   
└───lib
│   │   libqc_simd_ldpc_decoder_x86.a           // Static library for x86 Linux
│   │   libqc_simd_ldpc_decoder_x64.a           // Static library for x64 Linux
│   │   qc_simd_ldpc_decoder_x86.lib            // Static library for x86 Windows
│   │   qc_simd_ldpc_decoder_x64.lib            // Static library for x64 Windows
│   
└───example
    └───tb_ldpc
    │   │   Makefile
    │   │
    │   └───inc
    │   │   │   nr5g_simd_ldpc_encoder.h        // 5GNR LDPC head file
    │   │
    │   └───lib
    │   │   │   libnr5g_simd_ldpc_encoder_x86.a // 5GNR LDPC Encoder Static library for x86 Linux
    │   │   │   libnr5g_simd_ldpc_encoder_x64.a // 5GNR LDPC Encoder Static library for x64 Linux
    │   │   │   nr5g_simd_ldpc_encoder_x86.lib  // 5GNR LDPC Encoder Static library for x86 Windows
    │   │   │   nr5g_simd_ldpc_encoder_x64.lib  // 5GNR LDPC Encoder Static library for x64 Windows
    │   │   
    │   └───src
    │       │   main.c                          // Example main source file
    │
    └───mex_function
        │   test.m                              // Test m file
        │   test_param.mat                      // Test parameters and data
        │   mex_qc_ldpc_decoder.mexw64          // Mex function
        │
        └───src
            │   mex_qc_ldpc_decoder.c   // interface source file
```


Build and Execution Instructions
------------------------------------------------------------

### Build:
> cd example/tb_ldpc

> make

### Execution:
> ./main

### Clean:
> make clean

Performance
------------------------------------------------------------

### Environment:
* OS: Ubuntu 16.04 xenial
* Kernel: x86_64 Linux 4.4.0-21-generic
* CPU: 4x Intel Xeon Gold 6154 CPU @ 3.001GHz
* ICC: 18.0.2

### Simulation Parameter:

* Base Graph: 5GNR Base Graph 1(i<sub>LS</sub> = 2)
* Code Block Length: 8448
* Code Rate: 948/1024

### Result:
| SIMD          | Throughput    | Code Block Latency    |
| ------------- | ------------- | --------------------- |
| SSE4.1        | 77.54Mbps     | 108.95μs              |
| AVX2          | 145.14Mbps    | 58.21μs               |
| AVX512        | 223.41Mbps    | 37.81μs               |
