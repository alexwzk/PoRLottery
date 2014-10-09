#include "pmc.h"

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

size_t pmc::computeU_i(digest pk, size_t i_inl, size_t num_all) {
	digest hashvalue;
	RANDENGINE rand_engine;
	std::string tmp_str;

//	std::cout << "Here is U_i function." << std::endl;
	tmp_str.assign((char*)pk,HASH_SIZE);
	tmp_str += std::to_string(i_inl);

	// coutest pk||i
//	std::cout << "The pk||i : " << tmp_str << std::endl;
	SHA1((uint8_t *) tmp_str.data(), tmp_str.size(), hashvalue);
	// coutest state
	return rand_engine.randByHash(hashvalue, num_all);
}

size_t pmc::computeI_inL(std::string inputs, size_t num_sub) {
	digest hashvalue;
	RANDENGINE rand_engine;

//	std::cout << "Here is I_inL function." << std::endl;
	SHA1((uint8_t *) inputs.data(), inputs.size(), hashvalue);
	return rand_engine.randByHash(hashvalue, num_sub);
}

size_t pmc::computeR_i(digest pk, std::string inputs, size_t num_sub,
		size_t num_all) {
	return computeU_i(pk, computeI_inL(inputs, num_sub), num_all);
}
