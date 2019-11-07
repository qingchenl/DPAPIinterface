#include <Rcpp.h>
#include <Windows.h>
#include <string>

using namespace Rcpp;

void string_to_num(std::string encrypted_str, int num_array[]);
std::string num_to_string(int num_array[], int arr_size);

//[[Rcpp::export]]
Rcpp::NumericVector encrypt(Rcpp::CharacterVector plaintext, Rcpp::CharacterVector entropy = "") {
	//declare variables
	std::string plain;
	std::string entr;
	DATA_BLOB pw_blob;
	DATA_BLOB encrypt_blob;
	DATA_BLOB entropy_blob;
	bool result;
	int* encrypt_num; //will point to a dynamically allocated int array
	
	//convert from CharacterVector to string
	plain = Rcpp::as<std::string>(plaintext);
	entr = Rcpp::as<std::string>(entropy);
	
	//fill DATA_BLOB elements from respective strings
	pw_blob.pbData = (BYTE *)plain.c_str(); //need to make a C string conversion before byte pointer conversion
	pw_blob.cbData = plain.length() + 1; //size is num of visible characters + null terminator
	
	entropy_blob.pbData = (BYTE *)entr.c_str();
	entropy_blob.cbData = entr.length()+1;

	result = CryptProtectData(&pw_blob, NULL, &entropy_blob, NULL, NULL, 0, &encrypt_blob);
	/*
	&pw_blob_in is pointer to plaintext blob
	&entropy_blob is pointer to secondary entropy blob
	&encrypt_blob is pointer to output blob
	result is indicator for success - not currently used
	*/
	
	//construct string from the encrypted output
	std::string encrypt_string(encrypt_blob.pbData, encrypt_blob.pbData + encrypt_blob.cbData);
	
	//convert encrypted string to Numeric Vector
	encrypt_num = new int[encrypt_string.length()]; //construct int array
	string_to_num(encrypt_string, encrypt_num); //call function to convert string into int array of its bytes
	Rcpp::NumericVector R_encrypted(encrypt_string.length()); //construct NumericVector
	
	//read int array into NumericVector
	for(int i = 0; i < encrypt_string.length(); i++){
		R_encrypted[i] = encrypt_num[i];
	}
	
	return(R_encrypted);
}

//[[Rcpp::export]]
std::string decrypt(Rcpp::NumericVector encrypt_vector, Rcpp::CharacterVector entropy = "") {
	//declare variables
	const float delta = 0.00001; //nudge numbers slightly to avoid truncation errors
	
	std::string recrypted;
	std::string entr;
	int encrypt_size;
	int* encrypt_array;
	DATA_BLOB pw_blob;
	DATA_BLOB entropy_blob;
	DATA_BLOB encrypt_blob;

	encrypt_size = encrypt_vector.size();  
	encrypt_array = new int[encrypt_size];
	
	//adjusts numbers in direction opposite truncation
	for(int i = 0; i < encrypt_size; i++){
		if(encrypt_vector[i] > 0){
			encrypt_array[i] = encrypt_vector[i] + delta;
		}else{
			encrypt_array[i] = encrypt_vector[i] - delta;
		}
	}
	recrypted = num_to_string(encrypt_array, encrypt_size); //reconstitute the encrypted string
	encrypt_blob.pbData = (BYTE *)recrypted.c_str();
	encrypt_blob.cbData = recrypted.length() + 1;
	
	//convert secondary entropy to string and then DATA_BLOB
	entr = Rcpp::as<std::string>(entropy);
	entropy_blob.pbData = (BYTE *)entr.c_str();
	entropy_blob.cbData = entr.length() + 1;
	
	bool result = CryptUnprotectData(&encrypt_blob, NULL, &entropy_blob, NULL, NULL, 0, &pw_blob);
	
	std::string plaintext(pw_blob.pbData, pw_blob.pbData + pw_blob.cbData);
	return(plaintext);
}

//convert string to array of its bytes (as int)
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