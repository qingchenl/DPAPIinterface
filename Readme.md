# DPAPIinterface
DPAPIinterface allows R users on Windows OS to use Microsoft's DPAPI to encrypt and decrypt data. The code is connecting DPAPI functions and R are written in C++ and therefore depends on the Rcpp library.

### Encryption
```DPAPIinterface::encrypt(str, entropy = "")```
- ```str``` argument is a ```Character Vector``` of length 1
- ```entropy``` argument is a ```Character Vector``` of length 1
- returns a numeric vector of arbitrary length

### Decryption
```DPAPIinterface::decrypt(vec, entropy = "")```
- ```vec``` argument is a numeric vector
- ```entropy``` argument is a ```Character Vector``` of length 1
- returns a character vector of length 1

### Example
*Dramatis Personae*:
- Bob, an R programmer
- Alice, another R programmer
- Eve, a nefarious actor

Data can be encrypted without secondary entropy, which is used to derive an additional key necessary for decryption. For example, Bob may encrypt a string without secondary entropy
```
cryptic <- DPAPIinterface::encrypt("not aPassword123!!!")
notcryptic <- DPAPIinterface::decrypt(cryptic)
cryptic
notcryptic
```

Eve would only be able to decrypt ```cryptic``` if she either knows Bob's Windows user password at the time of encryption, or gains sufficient access Bob's system to run a DPAPI function under Bob's account.

Alice is worried that her system may be compromised at some point of time, so she uses "mellon" for her secondary entropy:
```
cryptic2 <- DPAPIinterface::encrypt("Khazad-dum", "mellon")
cryptic2
```
(In production code, it's probably better to prompt for secondary entropy each session instead of hard coding it)

Alice knows what she used for secondary entropy, so she can decrypt ```cryptic2```
```
DPAPIinterface::decrypt(cryptic2, "mellon")
```

Eve doesn't know what string Alice used for secondary entropy, so she can't decrypt even if she manages to execute DPAPI functions under Alice's account
```
DPAPIinterface::decrypt(cryptic2)
```


### Under the hood
- ```encrypt``` converts an R character vector of length 1 to a C++ std::string, which is fed to ```CryptProtectData``` after a bit of manipulation. ```entropy``` is processed similarly. The output DATA_BLOB is converted to a string that is then converted to an R numeric vector to circummvent issues with translating arbitrary C++ std::string to R character vector
- ```decrypt``` reverses the steps of encrypt
- ```entropy``` provides secondary entropy, which in my understanding modifies the encryption key without changing the difficulty of brute-force attacks

### To-do
- Change how int array (of bytes) to string conversion works: allocate placeholder string of correct length first instead of concatenating one letter at a time
- Check platform before installation and give suitable error messages for non-Windows OS
- Actual manual