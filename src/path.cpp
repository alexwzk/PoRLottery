#include "path.h"

PATH::PATH(uchar* inleaf, size_t leaf_size) {
	try {
		leafPt = new uchar[leaf_size];
	} catch (std::bad_alloc& err) {
		std::cerr << err.what() << " malloc leafPt @ PATH 1st constructor."
				<< std::endl;
		exit(MALLOC_ERR);
	}
	memcpy(leafPt, inleaf, leaf_size);
	this->leaf_size = leaf_size;
}

PATH::~PATH() {
	for (auto it : siblings) {
		delete[] it;
	}
	delete[] leafPt;
}

uchar* PATH::releaseLeaf(size_t &leaf_size) {
	leaf_size = this->leaf_size;
	return leafPt;
}

std::list<uchar*> PATH::returnSiblings() {
	return siblings;
}

int PATH::pushDigestPt(digest hash) {
	if (hash == nullptr) {
		return PTNULL_ERR;
	}
	uchar* tmp_hashPt;
	try {
		tmp_hashPt = new digest;
	} catch (std::bad_alloc& err) {
		std::cerr << err.what() << "malloc tmp_hashPt @ PATH pushDigestPt."
				<< std::endl;
		exit(MALLOC_ERR);
	}
	memcpy(tmp_hashPt, hash, sizeof(digest));
	siblings.push_back(tmp_hashPt);
	return FINE;
}

std::string PATH::returnPathAsStr() {
	std::string result, tmp_str;
	result.assign((char*) leafPt, leaf_size);
	for (uchar* digest : siblings) {
		tmp_str.assign((char*) digest, sizeof(digest));
		result += tmp_str;
	}
	return result;
}

