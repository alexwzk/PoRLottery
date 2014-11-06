#ifndef PMC_PATH_H_
#define PMC_PATH_H_

#include "buffer.h"
#include "core.h"

template<unsigned int LEAF_BYTES>
class PATH {

private:
	BUFFER<LEAF_BYTES> leaf;
	std::vector<uint256> vhashes; // the item's hash siblings

public:

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

	IMPLEMENT_SERIALIZE(
		READWRITE(leaf);
		READWRITE(vhashes);
	)

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
