// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// encrypt
Rcpp::NumericVector encrypt(Rcpp::CharacterVector plaintext, Rcpp::CharacterVector entropy);
RcppExport SEXP _DPAPIinterface_encrypt(SEXP plaintextSEXP, SEXP entropySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type plaintext(plaintextSEXP);
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type entropy(entropySEXP);
    rcpp_result_gen = Rcpp::wrap(encrypt(plaintext, entropy));
    return rcpp_result_gen;
END_RCPP
}
// decrypt
std::string decrypt(Rcpp::NumericVector encrypt_vector, Rcpp::CharacterVector entropy);
RcppExport SEXP _DPAPIinterface_decrypt(SEXP encrypt_vectorSEXP, SEXP entropySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::NumericVector >::type encrypt_vector(encrypt_vectorSEXP);
    Rcpp::traits::input_parameter< Rcpp::CharacterVector >::type entropy(entropySEXP);
    rcpp_result_gen = Rcpp::wrap(decrypt(encrypt_vector, entropy));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_DPAPIinterface_encrypt", (DL_FUNC) &_DPAPIinterface_encrypt, 2},
    {"_DPAPIinterface_decrypt", (DL_FUNC) &_DPAPIinterface_decrypt, 2},
    {NULL, NULL, 0}
};

RcppExport void R_init_DPAPIinterface(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
