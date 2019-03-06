#ifndef NR5G_LDPC_SIMD_ENCODER
#define NR5G_LDPC_SIMD_ENCODER

#include <stdint.h>
#include <stdlib.h>

#define SIMD_MODE_AUTO 0
#define SIMD_MODE_SSE 1
#define SIMD_MODE_AVX2 2
#define SIMD_MODE_AVX512 3

typedef struct nr5g_simd_ldpc_encoder_t
{
	const int16_t *H_BG;
	int32_t BG_sel;
	int32_t iLS;
	int32_t col_hbg;
	int32_t row_hbg;
	int32_t Z_c;
	int32_t K;
	int32_t N;
	int32_t simd_mode;

	/* Encoder Parameters */
    int8_t *p0, *p1, *p2, *p3;


} nr5g_simd_ldpc_encoder_t;

int nr5g_simd_ldpc_encoder_mem_init(int32_t max_zc, int32_t simd_mode, nr5g_simd_ldpc_encoder_t *h);

int nr5g_simd_ldpc_encoder_param_init(int32_t BG_sel, int32_t iLS, int32_t Z_c, nr5g_simd_ldpc_encoder_t *h);

int nr5g_ldpc_simd_encoder_scb(const int8_t *info_bits, nr5g_simd_ldpc_encoder_t *h, int8_t *coded_bits);

int free_nr5g_simd_ldpc_encoder_t(nr5g_simd_ldpc_encoder_t *h);

#endif