#include "common.h"

void COMMON::printHash(const digest dgst) {
	for (int i = 0; i < HASH_SIZE; i++)
		printf("%02x", dgst[i]);
	printf("\n");
}

std::string COMMON::stringToHex(const std::string& input) {
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i) {
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}

size_t COMMON::hashToNumber(digest hashv) {
	size_t number = 0;
	for (int i = 0; i < XTRACT_SIZE; i++) {
		number += hashv[i];
	}
	return number;
}

std::string COMMON::newRandStr(size_t k) {
	std::random_device randevice;
	std::default_random_engine rand_engine(randevice());
	size_t remain_k = k;
	size_t nonce = -1;
	std::string randstr;
	digest hashvalue;
	while (remain_k > 20) {
		nonce = rand_engine();
		SHA1(reinterpret_cast<const unsigned char*>(&nonce), sizeof(size_t),
				hashvalue);
		//Coutest print each 160-bits random string
//		COMMON::printHash(hashv);
		randstr.append(hashvalue, hashvalue + HASH_SIZE);
		remain_k -= (HASH_SIZE * 8);
	}
	nonce = rand_engine();
	SHA1(reinterpret_cast<const unsigned char*>(&nonce), sizeof(size_t), hashvalue);
	//Coutest print each 160-bits random string
//	COMMON::printHash(hashv);
	randstr.append(hashvalue, hashvalue + remain_k);

	return randstr;
}
