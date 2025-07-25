#include "openfhe.h"
#include "test.hpp"

using namespace lbcrypto;

int main() {
  std::cout << "Some very simple example" << std::endl;

  // Setup ================================
  CCParams<CryptoContextCKKSRNS> parameters;
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
  std::vector<std::complex<double>> x1 = {0, 1, 2, 3, 1i, 2i, 3i, 1+1i};
  std::vector<std::complex<double>> x2 = {1i, 2i, 3i, 1+1i, 0, 1, 2, 3};

  std::cout << "Input :" << std::endl;
  std::cout << "x1 ";
  printComplexVector(x1);
  std::cout << "x2 ";
  printComplexVector(x2);

  std::cout << "\nComputing: x1 * x2\n" << std::endl;

  Plaintext ptx1 = cc->MakeCKKSPackedPlaintext(x1);
  Plaintext ptx2 = cc->MakeCKKSPackedPlaintext(x2);

  Ciphertext<DCRTPoly> ctx1 = cc->Encrypt(keys.publicKey, ptx1);
  Ciphertext<DCRTPoly> ctx2 = cc->Encrypt(keys.publicKey, ptx2);

  // Computation ==========================
  auto ctx_result = cc->EvalMult(ctx1, ctx2);

  // Decryption ===========================
  Plaintext result;
  cc->Decrypt(keys.secretKey, ctx_result, &result);
  result->SetLength(8);
  std::cout << "Result: " << std::endl;
  printComplexVector(result->GetCKKSPackedValue());

  std::cout << "Done" << std::endl;

  return 0;
}

