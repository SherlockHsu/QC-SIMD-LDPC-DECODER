QC-SIMD-LDPC-DECODER
============================================================

Universal Quasi-Cyclic SIMD-LDPC decoder supporting SSE, AVX2 and AVX512.

* Vesion:   1.1
* Date:     2019.03.02
* Author:   Xu Yi

File Specification
------------------------------------------------------------

```
QC-SIMD-LDPC-DECODER
│   README.md
│   LICENSE
│
└───inc
│   │   qc_simd_ldpc_decoder.h      // Head file
│   
└───lib
│   │   libqc_simd_ldpc_decoder.a   // Static library for Linux
│   │   qc_simd_ldpc_decoder.lib    // Static library for Windows
│   
└───example
    │   Makefile
    │
    └───inc
    │   │   simd_ldpc.h             // Example head file
    │
    └───lib
    │   │   libsimd_ldpc.a          // Example Static library for Linux
    │   │   simd_ldpc.lib           // Example Static library for Windows
    │   
    └───src
        │   main.c                  // Example main source file
```

Build and Execution Instructions
------------------------------------------------------------

### Build:
> cd example
> make

### Execution:
> ./main

### Clean:
> make clean