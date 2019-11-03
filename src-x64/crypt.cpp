#include <Rcpp.h>
#include <Windows.h>
#include <string>

using namespace Rcpp;

void string_to_num(std::string encrypted_str, int num_array[]);
std::string num_to_string(int num_array[], int arr_size);

//[[Rcpp::export]]
Rcpp::NumericVector encrypt(Rcpp::CharacterVector plaintext) {
	std::string plain;
	DATA_BLOB pw_blob;
	DATA_BLOB encrypt_blob;
	bool result;
	int* encrypt_num;
	
	plain = Rcpp::as<std::string>(plaintext);
	pw_blob.pbData = (BYTE *)plain.c_str(); //need to make a C string conversion before byte pointer conversion
	pw_blob.cbData = plain.length() + 1; //size is num of visible characters + null terminator
	result = CryptProtectData(&pw_blob, NULL, NULL, NULL, NULL, 0, &encrypt_blob);
	/*
	&pw_blob_in is pointer to plaintext blob
	&encrypt_blob is pointer to output blob
	result is indicator for success
	*/
	std::string encrypt_string(encrypt_blob.pbData, encrypt_blob.pbData + encrypt_blob.cbData);
	encrypt_num = new int[encrypt_string.length()];
	string_to_num(encrypt_string, encrypt_num);
	
	Rcpp::NumericVector R_encrypted(encrypt_string.length());
		
	for(int i=0;i<encrypt_string.length();i++){
		R_encrypted[i] = encrypt_num[i];
	}
	
	//Rcout << encrypt_string << std::endl;
	//Rcout << encrypt_blob.cbData << std::endl;
	return(R_encrypted);
}

//[[Rcpp::export]]
std::string decrypt(Rcpp::NumericVector encrypt_vector) {
		DATA_BLOB encrypt_blob;
		DATA_BLOB pw_blob;
		int encrypt_size;
		int* encrypt_array;
		const float delta = 0.00001; //nudge numbers slightly to avoid truncation errors
		std::string recrypted;
		
		
		encrypt_size = encrypt_vector.size();
		encrypt_array = new int[encrypt_size];
		
		for(int i=0;i<encrypt_size;i++){
			if(encrypt_vector[i] > 0){
				encrypt_array[i] = encrypt_vector[i]+delta;
			}else{
				encrypt_array[i] = encrypt_vector[i]-delta;
			}
		}
		
		//Rcout << encrypt_size << std::endl;
		
		recrypted = num_to_string(encrypt_array, encrypt_size);
		
		//std::string encrypted = Rcpp::as<std::string>(encrypt_string);
		
		encrypt_blob.pbData = (BYTE *)recrypted.c_str();
		encrypt_blob.cbData = recrypted.length() + 1;
		bool result = CryptUnprotectData(&encrypt_blob, NULL, NULL, NULL, NULL, 0, &pw_blob);
		
		std::string plaintext(pw_blob.pbData, pw_blob.pbData + pw_blob.cbData);
		return(plaintext);
}

void string_to_num(std::string encrypted_str, int num_array[]) {
	int str_leng = encrypted_str.length();
	for (int i = 0; i < str_leng; i++) {
		num_array[i] = encrypted_str[i];
	}
	return;
}

std::string num_to_string(int num_array[], int arr_size) {

	std::string recrypted_str;

	for (int i = 0; i < arr_size; i++) {
		recrypted_str += (char)num_array[i];
	}
	
	return recrypted_str;
}