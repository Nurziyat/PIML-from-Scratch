#include "matrix_utils.h"
#include <stdexcept>

// Ikki ulkan FlatMatrix ni ko'paytirish funksiyasi
FlatMatrix multiply(const FlatMatrix& A, const FlatMatrix& B) {
    // Matematik cheklov: Birinchi matritsaning ustunlar soni ikkinchisining satrlar soniga teng bo'lishi shart
    if (A.cols != B.rows) {
        throw std::invalid_argument("Matritsalar o'lchami ko'paytirish qonuniyatiga mos kelmaydi!");
    }

    // Natijaviy C matritsasining o'lchami: A.rows x B.cols
    FlatMatrix C;
    C.rows = A.rows;
    C.cols = B.cols;
    C.data.assign(C.rows * C.cols, 0.0); // Hammasini 0.0 bilan to'ldiramiz

    // DAXSHATLI BILIM NUQTASI: Oddiy i-j-k emas, i-k-j siklidan foydalanamiz!
    for (size_t i = 0; i < A.rows; ++i) {
        for (size_t k = 0; k < A.cols; ++k) {
            
            // A(i,k) elementni kesh xotirada (L1/L2 cache) bitta registrga yuklab olamiz
            double r = A.at(i, k); 
            
            for (size_t j = 0; j < B.cols; ++j) {
                // B(k, j) va C(i, j) elementlarga xotirada ketma-ket (sequential) murojaat qilinadi.
                // Bu protsessor keshining (Cache Miss) oldini oladi va kodni 10-20 barobar tezlashtiradi!
                C.at(i, j) += r * B.at(k, j);
            }
        }
    }

    return C;
}
