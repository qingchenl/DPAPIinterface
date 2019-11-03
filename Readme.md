# DPAPIinterface
DPAPIinterface allows R users on Windows OS to use Microsoft's DPAPI to encrypt and decrypt data. The code is connecting DPAPI functions and R are written in C++ and therefore depends on the Rcpp library.

### Encryption
```DPAPIinterface::encrypt(str)```
- ```str``` argument is a ```Character Vector``` of length 1
- returns a numeric vector of arbitrary length

### Decryption
```DPAPIinterface::decrypt(vec)```
- ```vec``` argument is a numeric vector
- returns a character vector of length 1

### Example
```
cryptic <- DPAPIinterface::encrypt("not aPassword123!!!")
notcryptic <- DPAPIinterface::decrypt(cryptic)
cryptic
notcryptic
```

### Under the hood
- ```encrypt``` converts an R character vector of length 1 to a C++ std::string, which is fed to ```CryptProtectData``` after a bit of manipulation. The output DATA_BLOB is converted to a string that is then converted to an R numeric vector to circummvent issues with translating arbitrary C++ std::string to R character vector
- ```decrypt``` reverses the steps of encrypt