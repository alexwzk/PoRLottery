#include "path.h"

PATH::PATH(leaf inleaf) {
	using namespace std;
	try {
		leafPt = new uchar[LEAF_SIZE];
	} catch (bad_alloc& err) {
		cerr << err.what() << " @ PATH 1st constructor for leafPt." << endl;
		exit (PTNULL_ERR);
	}
	memcpy(leafPt,inleaf, sizeof(leaf));
}

PATH::~PATH() {
	for (auto it : siblings) {
		delete[] it;
	}
	delete[] leafPt;
}

uchar* PATH::returnLeafPt() {
	return leafPt;
}

std::list<uchar*> PATH::returnSiblings() {
	return siblings;
}

int PATH::pushDigestPt(digest hash) {
	uchar* tmp_hashPt;
	try {
		tmp_hashPt = new uchar[HASH_SIZE];
	} catch (std::bad_alloc& err) {
		std::cerr << err.what() << "@ PATH pushDigestPt for tmp_hashPt."
				<< std::endl;
		exit (PTNULL_ERR);
	}
	memcpy(tmp_hashPt, hash, sizeof(digest));
	siblings.push_back(tmp_hashPt);
	return FINE;
}
