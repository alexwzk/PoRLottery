#include "merkle.h"

MERKLE::MERKLE(std::vector<uchar *> segmts, size_t leaf_size) {
	using namespace std;
	num_segmts = segmts.size();
	height = ceil(log2(num_segmts)) + 1; // includes the root

	// Arrays memory allocation
	try {
		arrays = new digest*[height];
	} catch (bad_alloc& err) {
		cerr << err.what() << " @ MERKLE 1st constructor for [arrays**]." << endl;
	}
	size_t num_elem = 1;
	now_layer = 0;
	next_layer = 0;
	while (next_layer < height) {
		// coutest H arrays
		// cout << "now layer: " << now_layer << " has " << num_elem << " element(s). " << endl;
		try {
			arrays[next_layer++] = new digest[num_elem];
		} catch (bad_alloc& err) {
			cerr << err.what() << " @ MERKLE 1st constructor for [arrays*]."
					<< endl;
		}
		num_elem = num_elem << 1;
	}
	// State the current layer and its size
	now_layer = next_layer - 1;
	num_elem = num_elem >> 1;

	// Process leaves
	try {
		segments = new leaf[num_elem]; // full binary tree
	} catch (bad_alloc& err) {
		cerr << err.what() << " @ MERKLE 1st constructor for [segments*]." << endl;
	}

	// Hash the leaves (file segments)
	for (size_t i = 0; i < num_segmts; i++) {
		memcpy(segments[i], segmts[i], leaf_size);
		SHA1(segments[i], leaf_size, arrays[now_layer][i]);
		// coutest memcpy
		/*cout << "No. " << i << ": ";
		COMMON::printHex(segments[i], leaf_size);
		cout << " and its";
		cout << " hash value: ";
		COMMON::printHex(arrays[now_layer][i], HASH_SIZE);*/
	}

	// Fill the leaves into a full binary tree
	num_leaves = num_segmts;
	while (num_leaves < num_elem) {
		memset(segments[num_leaves], 0, leaf_size);
		SHA1(segments[num_leaves], leaf_size, arrays[now_layer][num_leaves]);
		num_leaves++;
	}

	// coutest the rest redundant file segments
	/*for (size_t i = num_segmts; i < num_leaves; i++) {
		cout << "Redundnt No. " << i << ": ";
		COMMON::printHex(segments[i], leaf_size);
		cout << " and its";
		cout << " hash value: ";
		COMMON::printHex(arrays[now_layer][i], HASH_SIZE);
	}*/

	// Hash children digests
	next_layer = now_layer - 1;
	num_elem = num_leaves >> 1;
	while (next_layer >= 0) {
		for (size_t i = 0; i < num_elem; i++) {
			SHA1(arrays[now_layer][i * 2], HASH_SIZE * 2,
					arrays[next_layer][i]);
		}
		now_layer--;
		next_layer = now_layer - 1;
		num_elem = num_elem >> 1;
	}

	// coutest Merkle tree 2D array
	/*num_elem = 1;
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < num_elem; j++) {
			cout << "Print [" << i << "] [" << j << "]'s hash value: " << endl;
			COMMON::printHex(arrays[i][j], HASH_SIZE);
		}
		num_elem = num_elem << 1;
	}*/
}

MERKLE::~MERKLE() {
	delete[] segments;
	for (size_t i = 0; i < height; i++) {
		delete[] arrays[i];
	}
	delete[] arrays;
}

PATH* MERKLE::newPath(size_t loca) {
	using namespace std;
	if (loca >= num_leaves || arrays == nullptr) {
		return nullptr;
	}

	int it_index;
	// Get the file segment F_{loca}
	PATH* pathPt = nullptr;
	try{
		pathPt = new PATH(segments[loca]);
	}catch (bad_alloc& err) {
		cerr << err.what() << " @ MERKLE newPath for [pathPt]." << endl;
	}

	size_t now_loca = loca;
	now_layer = height - 1;
	for (size_t i = 0; i < height - 1; i++) {
		if (COMMON::isEven(now_loca)) {
			pathPt->pushDigestPt(arrays[now_layer][now_loca + 1]);
		} else {
			pathPt->pushDigestPt(arrays[now_layer][now_loca - 1]);
		}
		now_loca = now_loca >> 1;
		now_layer--;
	}

	//coutest path
	cout << "The path of item No." << loca << ": " << endl;
//	cout << "its item is: ";
//	COMMON::printHex(pathPt->returnLeafPt(),leaf_size);
	cout << " its hash siblings are: " << endl;
	it_index = 0;
	for (auto it : pathPt->returnSiblings()) {
		cout << "layer " << (it_index++) << " ";
		COMMON::printHex(it, HASH_SIZE);
	}

	return pathPt;

}

uchar* MERKLE::releaseRootPt() {
	if (arrays == nullptr) {
		return nullptr;
	}
	return arrays[0][0];
}

bool MERKLE::validatePath(PATH* p, size_t leaf_size, size_t index, digest root) {
	size_t next_id = index;
	leaf pleaf;
	digest pdigest;
	uchar mkvalue[HASH_SIZE * 2];
	memcpy(pleaf, p->releaseLeafPt(), leaf_size);
	SHA1(pleaf, leaf_size, pdigest);
	for (auto it : p->returnSiblings()) {
		if (COMMON::isEven(next_id)) {
			memcpy(mkvalue, pdigest, HASH_SIZE);
			memcpy(mkvalue + HASH_SIZE, it, HASH_SIZE);
		} else {
			memcpy(mkvalue, it, HASH_SIZE);
			memcpy(mkvalue + HASH_SIZE, pdigest, HASH_SIZE);
		}
		SHA1(mkvalue, HASH_SIZE * 2, pdigest);
		next_id = next_id >> 1;
	}
	if (strncmp((const char *) root, (const char *) pdigest, HASH_SIZE) != 0) {
		return false;
	} else {
		return true;
	}
}

size_t MERKLE::returnNumSegmts() {
	return num_segmts;
}
