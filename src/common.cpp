#include "common.h"

void COMMON::printHex(const uchar* ucstr, const size_t length) {
	for (size_t i = 0; i < length; i++)
		printf("%02x", ucstr[i]);
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

size_t COMMON::computeU_i(std::string pk, size_t i_inl, size_t num_all){
	digest hashvalue;
	RANDENGINE rand_engine;
	std::string tmp_str = pk + std::to_string(i_inl);
	//std::cout << "The pk||i : " << tmp_str << std::endl;
	SHA1((uchar *) tmp_str.data(), tmp_str.size(), hashvalue);
	//Extract small number from the hash then mod by num_all
	return rand_engine.randByHash(hashvalue,num_all);
}

size_t COMMON::computeI_inL(std::string puz, std::string pk, int i_ink, std::string seed,
		size_t num_sub){
	digest hashvalue;
	RANDENGINE rand_engine;
	std::string tmp_str = puz + pk + std::to_string(i_ink) + seed;
	SHA1((uchar *) tmp_str.data(), tmp_str.size(), hashvalue);
	return rand_engine.randByHash(hashvalue,num_sub);
}

size_t COMMON::computeR_i(std::string puz, std::string pk, int i_ink, std::string seed,
		size_t num_sub, size_t num_all){
	return computeU_i(pk,computeI_inL(puz,pk,i_ink,seed,num_sub),num_all);
}
