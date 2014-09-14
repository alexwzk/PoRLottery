#include "path.h"

PATH::PATH(leaf inleaf) {
	try {
		leafPt = new leaf;
	} catch (std::bad_alloc& err) {
		std::cerr << err.what() << " @ PATH 1st constructor for leafPt."
				<< std::endl;
		exit(MALLOC_ERR);
	}
	memcpy(leafPt, inleaf, sizeof(leaf));
}

PATH::~PATH() {
	for (auto it : siblings) {
		delete[] it;
	}
	delete[] leafPt;
}

uchar* PATH::releaseLeafPt() {
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
		std::cerr << err.what() << "@ PATH pushDigestPt for tmp_hashPt."
				<< std::endl;
		exit(MALLOC_ERR);
	}
	memcpy(tmp_hashPt, hash, sizeof(digest));
	siblings.push_back(tmp_hashPt);
	return FINE;
}
