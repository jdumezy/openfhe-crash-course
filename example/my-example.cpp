#include "openfhe.h"

using namespace std::complex_literals; 
using namespace lbcrypto;

int main() {
    std::cout << "\n=== CKKS Complex Multiplication Example ===\n" << std::endl;

    // Setup ================================
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetSecretKeyDist(SPARSE_TERNARY);
    parameters.SetSecurityLevel(HEStd_128_classic);

    parameters.SetMultiplicativeDepth(1);
    parameters.SetScalingModSize(50);
    parameters.SetBatchSize(8);
    parameters.SetCKKSDataType(COMPLEX);

    auto cc = GenCryptoContext(parameters);
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);

    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);

    // Encryption ===========================
    std::vector<std::complex<double>> x1 = {0, 1, 2, 3, 1i, 2i, 3i, 1.0+1.0i};
    std::vector<std::complex<double>> x2 = {1i, 2i, 3i, 1.0+1.0i, 0, 1, 2, 3};

    Plaintext ptx1 = cc->MakeCKKSPackedPlaintext(x1);
    Plaintext ptx2 = cc->MakeCKKSPackedPlaintext(x2);

    auto ctx1 = cc->Encrypt(keys.publicKey, ptx1);
    auto ctx2 = cc->Encrypt(keys.publicKey, ptx2);

    // Computation ==========================
    auto ctx_result = cc->EvalMult(ctx1, ctx2);

    // Decryption ===========================
    Plaintext result;
    cc->Decrypt(keys.secretKey, ctx_result, &result);
    result->SetLength(8);

    auto resVec = result->GetCKKSPackedValue();

    std::cout.precision(1);
    std::cout << std::fixed;

    std::cout << "Index\tInput x1\t\tInput x2\t\tResult" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;

    for (size_t i = 0; i < 8; i++) {
        std::cout << i << "\t" 
                  << x1[i] << "\t\t" 
                  << x2[i] << "\t\t" 
                  << resVec[i] << std::endl;
    }

    return 0;
}
