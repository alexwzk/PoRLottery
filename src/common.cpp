#include "common.h"

std::vector<unsigned char>* PMC::newByteVec(unsigned char* a_ptr, size_t a_length) {
	if (a_ptr == NULL) {
		return NULL;
	}
	return new std::vector<unsigned char>(a_ptr, a_ptr + a_length);
}

void PMC::printHex(const unsigned char* ucstr, const size_t length) {
	for (size_t i = 0; i < length; i++)
		printf("%02x", ucstr[i]);
	printf("\n");
}

std::string PMC::stringToHex(const std::string& input) {
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

size_t PMC::computeU_i(uint256 pk, size_t i_inl, size_t num_all) {
	// coutest pk||i
	std::string inputs = pk.ToString() + itostr(i_inl);
	uint256 hashvalue = Hash(inputs.begin(),inputs.end());

	return PMC::getRandByHash(hashvalue, num_all);
}

size_t PMC::computeI_inL(std::string inputs, size_t num_sub) {
	uint256 hashvalue = Hash(inputs.begin(),inputs.end());
	return PMC::getRandByHash(hashvalue, num_sub);
}

size_t PMC::computeR_i(uint256 pk, std::string inputs,
		size_t num_sub, size_t num_all) {
	return computeU_i(pk, computeI_inL(inputs, num_sub), num_all);
}
