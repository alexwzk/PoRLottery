#include "pmc.h"

std::vector<uint8_t>* pmc::newByteVec(uint8_t* a_ptr, size_t a_length) {
	if(a_ptr == nullptr){
		return nullptr;
	}
	return new std::vector<uint8_t>(a_ptr, a_ptr + a_length);
}

void pmc::printHex(const uint8_t* ucstr, const size_t length) {
	for (size_t i = 0; i < length; i++)
		printf("%02x", ucstr[i]);
	printf("\n");
}

std::string pmc::stringToHex(const std::string& input) {
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
