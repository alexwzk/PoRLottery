/*
 * buffer.h
 *
 *  Created on: Oct 11, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PRM_BUFFER_H_
#define PRM_BUFFER_H_

#include "common.h"
#include "uint256.h"

template<unsigned int BYTES>
class BUFFER : public base_blob<BYTES * 8>{
	public:
	BUFFER() {}
	BUFFER(const base_blob<BYTES * 8>& b) : base_blob<BYTES * 8>(b) {}
	explicit BUFFER(const std::vector<unsigned char>& vch) : base_blob<BYTES * 8>(vch) {}
	BUFFER<BYTES>& operator=(const BUFFER<BYTES>& new_data) {
		memcpy(this->data,new_data.begin(),sizeof(this->data));
		return (*this);
	}

};

#endif /* PRM_BUFFER_H_ */
