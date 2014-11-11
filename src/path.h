/*
 * path.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PMC_PATH_H_
#define PMC_PATH_H_

#include "buffer.h"
#include "common.h"

template<unsigned int LEAF_BYTES>
class PATH {

public:

	BUFFER<LEAF_BYTES> leaf;
	std::vector<uint256> vhashes; // the item's hash siblings


	PATH(){
		setNull();
	}

	~PATH(){ }

	void setNull(){
		leaf.clear();
		vhashes.clear();
	}

	bool isNull() const{
		return leaf.isEmpty();
	}

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
    	READWRITE(leaf);
		READWRITE(vhashes);
    }

	void setLeafValue(BUFFER<LEAF_BYTES> nleaf){
		leaf = nleaf;
	}

	void pushHashDigest(uint256 hash_value){
		vhashes.push_back(hash_value);
	}

	BUFFER<LEAF_BYTES> returnLeaf() const {
		return leaf;
	}

	std::vector<uint256> returnHashSiblings() const {
		return vhashes;
	}

	std::string toString() const {
		std::string nString;
		nString = leaf.toString();
		for(size_t i = 0; i < vhashes.size(); i++) {
			nString += vhashes[i].ToString();
		}
		return nString;
	}

	PATH<LEAF_BYTES>& operator=(const PATH<LEAF_BYTES>& p) {
		leaf = p.returnLeaf();
		vhashes = p.returnHashSiblings();
		return (*this);
	}

};
#endif /*PMC_PATH_H_*/
