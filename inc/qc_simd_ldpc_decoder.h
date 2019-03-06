#ifndef QC_SIMD_LDPC_DECODER
#define QC_SIMD_LDPC_DECODER

#include <stdint.h>
#include <stdlib.h>
#include <immintrin.h>

#define SIMD_MODE_AUTO 0
#define SIMD_MODE_SSE 1
#define SIMD_MODE_AVX2 2
#define SIMD_MODE_AVX512 3

#define DECODER_MODE_OMS 1
#define DECODER_MODE_NMS 2

typedef struct qc_simd_ldpc_decoder_t
{
	const int16_t *H_BG;
	int32_t col_hbg;
	int32_t row_hbg;
	int32_t Z_c;
	int32_t K;
	int32_t llr_len;
	int32_t simd_mode;

	/* Decoder Parameters */
	int32_t Nd;
	int32_t M_whole;
	int32_t col_hbg_d;
	int32_t row_hbg_d;
	int32_t units;				 // floor(Z_c/REG_SIZE)
	int32_t whole_degree;		 // sum of degree of every hbg_row_d
	int8_t *llr_fixed;			 // fixed llr info(length:2*REG_SIZE+Nd)
	int8_t *degree;				 // number of connective check nodes(length:M/hbg_row_d)
	int8_t **llr_addr_start;	 // llr address(length:whole_degree*units)
	int8_t *llr_addr_flag;		 // flag for access type(0:no mask;1:1 mask;2:2 mask)
	int8_t **llr_addr_pre;		 // extra llr address for flag=2(length:whole_degree)

	__m128i *cn_msg_sse;		 // sse message from cn to vn(length:M_whole)
	__m128i *vn_msg_sse;		 // temp sse message from vn to cn(length:19)
	__m128i *mask_sse;			 // mask1 for flag=2(length:whole_degree)
	__m128i *mask_pre_sse;		 // mask2 for flag=2(length:whole_degree)
	__m128i *endmask_sse;		 // mask for flag=1

	__m256i *cn_msg_avx2;		 // avx2 message from cn to vn(length:M_whole)
	__m256i *vn_msg_avx2;		 // temp avx2 message from vn to cn(length:19)
	__m256i *mask_avx2;			 // mask1 for flag=2(length:whole_degree)
	__m256i *mask_pre_avx2;		 // mask2 for flag=2(length:whole_degree)
	__m256i *endmask_avx2;		 // mask for flag=1

	__m512i *cn_msg_avx512;		 // avx512 message from cn to vn(length:M_whole)
	__m512i *vn_msg_avx512;		 // temp avx512 message from vn to cn(length:19)
	__mmask64 *mmask_avx512;	 // mmask1 for flag=2(length:whole_degree)
	__mmask64 *mmask_pre_avx512; // mmask2 for flag=2(length:whole_degree)
	__mmask64 endmmask_avx512;   // mmask for flag=1

} qc_simd_ldpc_decoder_t;

/*
* Function:
*	Initialize memory of qc_simd_ldpc_decoder_t
* Input:
*	int32_t max_col_hbg		// Max Column Number of Base Graph
*	int32_t max_row_hbg		// Max Row Number of Base Graph
*	int32_t max_zc			// Max Extend Factor
* Output:
*	qc_simd_ldpc_decoder_t *h	// Parameter Structure of Decoder
* Return:
*	0: Normal, -1: Error
*/
int qc_simd_ldpc_decoder_mem_init(int32_t max_col_hbg, int32_t max_row_hbg, int32_t max_zc, int32_t simd_mode, qc_simd_ldpc_decoder_t *h);

/*
* Function:
*	Initialize parameters of qc_simd_ldpc_decoder_t
* Input:
*	int32_t col_hbg		// Column Number of Base Graph
*	int32_t row_hbg		// Row Number of Base Graph
*	int32_t Z_c			// Extend Factor
*	int32_t K			// Length of Information Bits
*	int32_t llr_len		// Length of Log-Likelihood Ratio
* Output:
*	qc_simd_ldpc_decoder_t *h	// Parameter Structure of Decoder
* Return:
*	0: Normal, -1: Error
*/
int qc_simd_ldpc_decoder_param_init(const int16_t *H_BG, int32_t col_hbg, int32_t row_hbg, int32_t Z_c, int32_t K, int32_t llr_len, qc_simd_ldpc_decoder_t *h);

/*
* Function:
*	Decode QC-LDPC log-likelihood ratio sequence based on SIMD for single block
* Input:
*	const float *llr			// Input Log-Likelihood Ratio Sequence
*	int32_t I_max				// Max Iteration Number
*	float coef					// Decoder Coefficient (Beta for OMS, Alpha for NMS)
*	int32_t decoder_mode		// Decoder Mode (DECODER_MODE_OMS or DECODER_MODE_NMS)
*	qc_simd_ldpc_decoder_t *h	// Parameter Structure of Decoder
* Output:
*	uint8_t *decoded_bits		// Output Decoded Bits Sequence
* Return:
*	0: Normal, -1: Error
*/
int qc_simd_ldpc_decoder_scb(const float *llr, int32_t I_max, float coef, int32_t decoder_mode, qc_simd_ldpc_decoder_t *h, uint8_t *decoded_bits);

/*
* Function:
*	Free structure qc_simd_ldpc_decoder_t
* Input:
*	uint8_t *decoded_bits		// Output Decoded Bits Sequence
* Return:
*	0: Normal, -1: Error
*/
int free_qc_simd_ldpc_decoder_t(qc_simd_ldpc_decoder_t *h);

#endif