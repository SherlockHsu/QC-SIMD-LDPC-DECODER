#include "qc_simd_ldpc_decoder.h"
#include "nr5g_simd_ldpc_encoder.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#if defined(_MSC_VER)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define M_PI 3.1415927
#define BLOCK_SIZE 10000
#define EBN0_SIZE 6

float randn(float mu, float sigma);
int main()
{
    int i, j, indx_block, indx_ebn0, sum_err_bits, test_size;
    float EbN0, sigma2, sigma;
    int32_t err_bits[BLOCK_SIZE];
    time_t t;
    double encode_run_time;
    double decode_run_time;
#if defined(_MSC_VER)
    LARGE_INTEGER num;
    long long start, end, freq;
#else
    struct timeval start, end;
    long timeuse;
#endif

    float EbN0_list[EBN0_SIZE] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    test_size = EBN0_SIZE;

    int Z_c = 384;
    int BG_sel = 1;
    int iLS = 2;
    int llr_num = 9984;
    nr5g_simd_ldpc_encoder_t *he = (nr5g_simd_ldpc_encoder_t *)malloc(sizeof(nr5g_simd_ldpc_encoder_t));
    nr5g_simd_ldpc_encoder_mem_init(Z_c, SIMD_MODE_AUTO, he);
    nr5g_simd_ldpc_encoder_param_init(BG_sel, iLS, Z_c, he);
    int K = he->K;
    int N = he->N;
    int col_hbg = he->col_hbg;
    int row_hbg = he->row_hbg;
    int8_t *info_bits = (int8_t *)malloc(sizeof(int8_t) * K);
    int8_t *coded_bits = (int8_t *)malloc(sizeof(int8_t) * N);

    int32_t I_max = 10;
    float coef = 0.5;
    int32_t decoder_mode = 1;
    float *llr = (float *)malloc(sizeof(float) * llr_num);
    uint8_t *decoded_bits = (uint8_t *)malloc(sizeof(uint8_t) * K);
    qc_simd_ldpc_decoder_t *hd = (qc_simd_ldpc_decoder_t *)malloc(sizeof(qc_simd_ldpc_decoder_t));
    qc_simd_ldpc_decoder_mem_init(col_hbg, row_hbg, Z_c, SIMD_MODE_AUTO, hd);
    qc_simd_ldpc_decoder_param_init(he->H_BG, col_hbg, row_hbg, Z_c, K, llr_num, hd);

    float *awgn;
    awgn = (float *)malloc(llr_num * sizeof(float));
    srand((unsigned)time(&t));
    for (j = 0; j < llr_num; j++)
        awgn[j] = randn(0.0, 1.0);

    for (indx_ebn0 = 0; indx_ebn0 < test_size; indx_ebn0++)
    {
        EbN0 = EbN0_list[indx_ebn0];
        sigma2 = (float)(1 / (pow(10, (double)EbN0 / 10) * 2 * K / llr_num));
		sigma = (float)sqrt(sigma2);
        sum_err_bits = 0;
        encode_run_time = 0.0;
        decode_run_time = 0.0;

        for (indx_block = 0; indx_block < BLOCK_SIZE; indx_block++)
        {
			err_bits[indx_block] = 0;
            
            srand((unsigned)time(&t));
            for (i = 0; i < K; i++)
                info_bits[i] = rand() % 2;

#if defined(_MSC_VER)
            QueryPerformanceFrequency(&num);
            freq = num.QuadPart;
            QueryPerformanceCounter(&num);
            start = num.QuadPart;
#else
            gettimeofday(&start, NULL);
#endif
            nr5g_ldpc_simd_encoder_scb(info_bits, he, coded_bits);
#if defined(_MSC_VER)
            QueryPerformanceCounter(&num);
            end = num.QuadPart;
            encode_run_time += (double)(end - start) / freq;
#else
            gettimeofday(&end, NULL);
            timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            encode_run_time += (double)timeuse / 1000000.0;
#endif

            /* pass AWGN channel */
            for (i = 0; i < llr_num; i++)
                llr[i] = -2 * (2 * coded_bits[i] - 1 + awgn[i] * sigma) / sigma2;

#if defined(_MSC_VER)
            QueryPerformanceFrequency(&num);
            freq = num.QuadPart;
            QueryPerformanceCounter(&num);
            start = num.QuadPart;
#else
            gettimeofday(&start, NULL);
#endif
            qc_simd_ldpc_decoder_scb(llr, I_max, coef, decoder_mode, hd, decoded_bits);
#if defined(_MSC_VER)
            QueryPerformanceCounter(&num);
            end = num.QuadPart;
            decode_run_time += (double)(end - start) / freq;
#else
            gettimeofday(&end, NULL);
            timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            decode_run_time += (double)timeuse / 1000000.0;
#endif

            /* statistics */
            for (i = 0; i < K; i++)
                err_bits[indx_block] += (info_bits[i] == decoded_bits[i] ? 0 : 1);

            sum_err_bits += err_bits[indx_block];
        }

        /* print results */
        printf("Eb/N0:%.2f:\tBER:\t%.2e(%d/%d)\n", EbN0_list[indx_ebn0], (float)sum_err_bits / K / BLOCK_SIZE, sum_err_bits, K * BLOCK_SIZE);
        printf("encode_run_time:%lfs\n", encode_run_time);
        printf("encode_Throughput:%.2lfMbps\n", (double)K * BLOCK_SIZE / encode_run_time / 1e6);
        printf("decode_run_time:%lfs\n", decode_run_time);
        printf("decode_Throughput:%.2lfMbps\n", (double)K * BLOCK_SIZE / decode_run_time / 1e6);
    }

    free_nr5g_simd_ldpc_encoder_t(he);
    free_qc_simd_ldpc_decoder_t(hd);

    return 0;
}

float randn(float mu, float sigma)
{
    float U1, U2, W, mult;
    static float X1, X2;
    static int call = 0;

    if (call == 1)
    {
        call = !call;
        return (mu + sigma * (float)X2);
    }

    do
    {
        U1 = -1 + ((float)rand() / RAND_MAX) * 2;
        U2 = -1 + ((float)rand() / RAND_MAX) * 2;
        W = pow(U1, 2) + pow(U2, 2);
    } while (W >= 1 || W == 0);

    mult = sqrt((-2 * log(W)) / W);
    X1 = U1 * mult;
    X2 = U2 * mult;

    call = !call;

    return (mu + sigma * (float)X1);
}