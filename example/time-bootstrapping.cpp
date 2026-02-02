#include "openfhe.h"
#include <chrono>

using namespace lbcrypto;

int main() {
    std::cout << "\n=== CKKS Bootstrapping Timing Example ===\n" << std::endl;

    // Setup ================================
    uint32_t numSlots = 8;
    
    CCParams<CryptoContextCKKSRNS> parameters;
    parameters.SetSecretKeyDist(SPARSE_TERNARY);
    parameters.SetSecurityLevel(HEStd_128_classic);
    parameters.SetScalingModSize(48);
    parameters.SetFirstModSize(48);

    // Bootstrapping configuration
    std::vector<uint32_t> levelBudget = {3, 3};
    std::vector<uint32_t> bsgsDim = {0, 0};
    uint32_t levelsAvailable = 3;
    
    usint depth = levelsAvailable + FHECKKSRNS::GetBootstrapDepth(levelBudget, SPARSE_TERNARY);
    parameters.SetMultiplicativeDepth(depth);

    auto cc = GenCryptoContext(parameters);

    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);
    cc->Enable(FHE);

    std::cout << "CKKS scheme is using ring dimension " << cc->GetRingDimension() << std::endl << std::endl;

    // Keys & Precomputations ================
    cc->EvalBootstrapSetup(levelBudget, bsgsDim, numSlots);

    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalBootstrapKeyGen(keys.secretKey, numSlots);

    // Encryption ============================
    std::vector<double> x = {0.25, 0.5, 0.75, 1.0, 2.0, 3.0, 4.0, 5.0};
    
    // Create a ciphertext at the lowest level (simulating a depleted ciphertext)
    Plaintext ptxt = cc->MakeCKKSPackedPlaintext(x, 1, depth - 1, nullptr, numSlots);
    ptxt->SetLength(numSlots);
    
    std::cout << "Input: " << ptxt << std::endl;

    Ciphertext<DCRTPoly> ciph = cc->Encrypt(keys.publicKey, ptxt);

    std::cout << "Initial levels remaining: " << depth - ciph->GetLevel() << std::endl;

    // Bootstrapping & Timing ================
    std::cout << "Running bootstrapping..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto ciph_refreshed = cc->EvalBootstrap(ciph);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    // Decryption & Results ==================
    std::cout << "Final levels remaining:   " << depth - ciph_refreshed->GetLevel() << std::endl << std::endl;

    Plaintext result;
    cc->Decrypt(keys.secretKey, ciph_refreshed, &result);
    result->SetLength(numSlots);
    std::cout << "Result: " << result << std::endl << std::endl;

    std::cout << "=========================================" << std::endl;
    std::cout << "           RUNTIME SUMMARY               " << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << " Operation  : CKKS Bootstrapping         " << std::endl;
    std::cout.precision(2);
    std::cout << " Time       : " << std::fixed << duration.count() << " ms" << std::endl;
    std::cout << "=========================================" << std::endl;

    return 0;
}
