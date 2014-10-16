#ifndef PMC_PATH_H_
#define PMC_PATH_H_

#include "pmc.h"
#include "buffer.h"

template<unsigned int LEAF_BYTES,class HASH_TYPE>
class PATH {

private:
	BUFFER<LEAF_BYTES> leaf;
	std::vector<HASH_TYPE> vhashes; // the item's hash siblings

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

	void pushHashDigest(HASH_TYPE hash_value){
		vhashes.push_back(hash_value);
	}

	BUFFER<LEAF_BYTES> returnLeaf() const{
		return leaf;
	}

	std::vector<HASH_TYPE> returnHashSiblings() const{
		return vhashes;
	}

	PATH<LEAF_BYTES,HASH_TYPE>& operator=(const PATH<LEAF_BYTES,HASH_TYPE>& p) {
		leaf = p.returnLeaf();
		vhashes = p.returnHashSiblings();
		return (*this);
	}

};
#endif /*PMC_PATH_H_*/
