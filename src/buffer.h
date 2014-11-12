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
	std::vector<unsigned char> vdata;

	BUFFER () {
		vdata.resize(SIZE);
	}

	ADD_SERIALIZE_METHODS;

	template<typename Stream, typename Operation>
	inline void SerializationOp(Stream& s, Operation ser_action, int nType,
			int nVersion) {
		//Guarantee consistency TODO find a more efficient way
		READWRITE(vdata);
	}

	void clear() {
		for(int i = 0; i < SIZE; i++){
			vdata[i] = 0;
		}
	}

	int assign(const unsigned char* bPt, const size_t inlength) {
		if (bPt == NULL || inlength != SIZE) {
			return INVALID_ERR;
		}
		for (size_t i = 0; i < SIZE; i++) {
			vdata[i] = bPt[i];
		}
		return FINE;
	}

	bool isEmpty() const {
		for (size_t i = 0; i < SIZE; i++) {
			if (vdata[i] != 0) {
				return false;
			}
		}
		return true;
	}

	unsigned char* begin() {
		return vdata.data();
	}

	const unsigned char* begin() const{
		return vdata.data();
	}

	unsigned char* end() {
	  return vdata.data() + SIZE -1;
	}


	const unsigned char* end() const{
		return vdata.data() + SIZE -1;
	}

	std::string toString() const{
		return std::string(vdata.begin(),vdata.end());
	}

	BUFFER<SIZE>& operator=(const BUFFER<SIZE>& b) {
		this->vdata = b.vdata;
		return (*this);
	}

};

#endif /* PMC_BUFFER_H_ */
