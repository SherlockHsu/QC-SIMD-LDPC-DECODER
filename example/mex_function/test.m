load("test_param.mat");
N = 10000;
tic;

for i = 1:N
    decoded_bits = mex_qc_ldpc_decoder(H_BG, Z_c, K, llr, I_max, coef, decoder_mode);
end

time = toc;
throughput = K * N / time;
latency = time / N;
fprintf("Throuput:%fMbps\tlatency:%fus\n", throughput / 1e6, latency * 1e6);

sum_err = sum(abs(bits - decoded_bits));
