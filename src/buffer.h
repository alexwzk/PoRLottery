/*
 * buffer.h
 *
 *  Created on: Oct 11, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PMC_BUFFER_H_
#define PMC_BUFFER_H_

#include "common.h"

template<size_t SIZE>
class BUFFER {
public:
	unsigned char data[SIZE];

	ADD_SERIALIZE_METHODS;

	template<typename Stream, typename Operation>
	inline void SerializationOp(Stream& s, Operation ser_action, int nType,
			int nVersion) {
		//Guarantee consistency TODO find a more efficient way
		std::string nstr = this->toString();
		READWRITE(nstr);
		this->assign((const unsigned char*) nstr.c_str(),SIZE);
	}

	void clear() {
		memset(data, 0, SIZE);
	}

	int assign(const unsigned char* bPt, const size_t inlength) {
		if (bPt == NULL || inlength != SIZE) {
			return INVALID_ERR;
		}
		for (size_t i = 0; i < SIZE; i++) {
			data[i] = bPt[i];
		}
		return FINE;
	}

	bool isEmpty() const {
		for (size_t i = 0; i < SIZE; i++) {
			if (data[i] != 0) {
				return false;
			}
		}
		return true;
	}

	unsigned char* begin() {
		return (unsigned char *) &data[0];
	}

	const unsigned char* begin() const{
		return (unsigned char *) &data[0];
	}

	unsigned char* end() {
	  return (unsigned char *) &data[SIZE];
	}


	const unsigned char* end() const{
		return (unsigned char *) &data[SIZE];
	}

	std::string toString() const{
		return std::string((const char *) this->begin(),SIZE);
	}

	BUFFER<SIZE>& operator=(const BUFFER<SIZE>& b) {
		this->assign(b.data, SIZE);
		return (*this);
	}

};

#endif /* PMC_BUFFER_H_ */
