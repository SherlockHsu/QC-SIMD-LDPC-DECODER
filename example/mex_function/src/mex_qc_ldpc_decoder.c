#include <mex.h>
#include <stdint.h>
#include <stdlib.h>

#include "qc_simd_ldpc_decoder.h"

/***************************************************************************
* Name:		mex_nr15_fec_decoder
*
* Function: nr15 decoder, including rate dematching, ldpc decoder and decbs
*
* Input:	prhs[0]:	H_BG(1 * (col_hbg*row_hbg))
*			prhs[1]:	extend factor(Z_c)
*			prhs[2]:    decoded length(K)
*			prhs[3]:    llr(1*llr_len)
*			prhs[4]:    max iteration(I_max)
*			prhs[5]:    decoder coefficient
*			prhs[6]:    decoder mode(1:OMS; 2:NMS)
*
* Output:	plhs[0]:    decoded bits(1*K)
***************************************************************************/
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	int32_t i, j;

	/* get prhs pointer */
	double *H_BG_D = mxGetPr(prhs[0]);
	int32_t col_hbg = (int32_t)mxGetN(prhs[0]);
	int32_t row_hbg = (int32_t)mxGetM(prhs[0]);
	int32_t Z_c = (int32_t)mxGetScalar(prhs[1]);
	int32_t K = (int32_t)mxGetScalar(prhs[2]);
	double *llr_D = mxGetPr(prhs[3]);
	int32_t llr_len = (int32_t)mxGetN(prhs[3]) * (int32_t)mxGetM(prhs[3]);
	int32_t I_max = (int32_t)mxGetScalar(prhs[4]);
	float coef = (float)mxGetScalar(prhs[5]);
	int32_t decoder_mode = (int32_t)mxGetScalar(prhs[6]);

	/* format input */
	int16_t *H_BG = (int16_t *)malloc(sizeof(int16_t) * col_hbg * row_hbg);
	for (i = 0; i < row_hbg; i++)
		for (j = 0; j < col_hbg; j++)
			H_BG[i * col_hbg + j] = (int16_t)H_BG_D[j * row_hbg + i];

	float *llr = (float *)malloc(sizeof(float) * llr_len);
	for (i = 0; i < llr_len; i++)
		llr[i] = (float)llr_D[i];

	/* decode */
	uint8_t *decoded_bits = (uint8_t *)malloc(sizeof(uint8_t) * K);
	qc_simd_ldpc_decoder_t *h = (qc_simd_ldpc_decoder_t *)malloc(sizeof(qc_simd_ldpc_decoder_t));
	qc_simd_ldpc_decoder_mem_init(col_hbg, row_hbg, Z_c, SIMD_MODE_AUTO, h);
	qc_simd_ldpc_decoder_param_init(H_BG, col_hbg, row_hbg, Z_c, K, llr_len, h);
	qc_simd_ldpc_decoder_scb(llr, I_max, coef, decoder_mode, h, decoded_bits);

	/* get plhs pointer */
	plhs[0] = mxCreateDoubleMatrix(K, 1, mxREAL);

	/* format output */
	double *decoded_bits_D = mxGetPr(plhs[0]);
	for (i = 0; i < K; i++)
		decoded_bits_D[i] = (double)decoded_bits[i];

	free(H_BG);
	free(llr);
	free(decoded_bits);
	free_qc_simd_ldpc_decoder_t(h);
}