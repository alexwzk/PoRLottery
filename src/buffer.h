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
		std::string nstr = this->toString();
		READWRITE(nstr);
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

	char* begin() {
		return (char *) &data[0];
	}

	const char* begin() const{
		return (char *) &data[0];
	}

	char* end() {
	  return (char *) &data[SIZE];
	}


	const char* end() const{
		return (char *) &data[SIZE];
	}


	std::string toString() const {
		return std::string((const char*) data);
	}

	BUFFER<SIZE>& operator=(const BUFFER<SIZE>& b) {
		this->assign(b.data, SIZE);
		return (*this);
	}

};

#endif /* PMC_BUFFER_H_ */
