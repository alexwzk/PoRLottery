#include "merkle.h"

MERKLE::MERKLE(std::vector<uchar *> segmts) {
	using namespace std;
	num_segmts = segmts.size();
	height = ceil(log2(num_segmts)) + 1; // includes the root

	// Arrays memory allocation
	try {
		arrays = new digest*[height];
	} catch (bad_alloc& err) {
		cerr << err.what() << " @ MERKLE 1st constructor for [arrays**]."
				<< endl;
	}
	size_t num_elem = 1;
	now_layer = 0;
	next_layer = 0;
	while (next_layer < height) {
		// Coutest H arrays
		// cout << "now layer: " << now_layer << " has " << num_elem << " element(s). " << endl;
		try {
			arrays[next_layer++] = new digest[num_elem];
		} catch (bad_alloc& err){
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
		cerr << err.what() << " @ MERKLE 1st constructor for [segments*]."
				<< endl;
	}

	// Hash the leaves (file segments)
	for (size_t i = 0; i < num_segmts; i++) {
		memcpy(segments[i], segmts[i], LEAF_SIZE);
		SHA1(segments[i], LEAF_SIZE, arrays[now_layer][i]);
		// Coutest memcpy
		cout << "No. " << i << ": ";
//		cout << segmts[i] << " and its";
		cout << " hash value: ";
		COMMON::printHash(arrays[now_layer][i]);
	}

	// Fill the leaves into a full binary tree
	num_leaves = num_segmts;
	while (num_leaves < num_elem) {
		memset(segments[num_leaves], 0, LEAF_SIZE);
		SHA1(segments[num_leaves], LEAF_SIZE, arrays[now_layer][num_leaves]);
		num_leaves++;
	}

	// Coutest the rest redundant file segments
	for (size_t i = num_segmts; i < num_leaves; i++) {
		cout << "No. " << i << ": ";
		COMMON::printHash(segments[i]);
		cout << "And its hash value: ";
		COMMON::printHash(arrays[now_layer][i]);
	}

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

	// Coutest Merkle tree 2D array
	num_elem = 1;
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < num_elem; j++) {
			cout << "Print [" << i << "] [" << j << "]'s hash value: " << endl;
			COMMON::printHash(arrays[i][j]);
		}
		num_elem = num_elem << 1;
	}
}

PATH MERKLE::buildPath(size_t loca) {
	using namespace std;
	if (loca >= num_leaves) {
		throw overflow_error("The location exceeds the max number of leaves.");
	}
	int it_index;
	// Get the file segment F_{loca}
	PATH path_rst(segments[loca]);

	size_t now_loca = loca;
	now_layer = height - 1;
	for (size_t i = 0; i < height - 1; i++) {
		if (COMMON::isEven(now_loca)) {
			path_rst.pushDigestPt(arrays[now_layer][now_loca + 1]);
		} else {
			path_rst.pushDigestPt(arrays[now_layer][now_loca - 1]);
		}
		now_loca = now_loca >> 1;
		now_layer--;
	}

	//Coutest path
	cout << "Coutest the path of " << loca << " : " << endl;
//	cout << "its item is: " << path_rst.item.data();
	cout << " and its hash siblings are: " << endl;
	it_index = 0;
	for (auto it : path_rst.returnSiblings()) {
		cout << "layer " << (it_index++) << " ";
		COMMON::printHash(it);
	}

	return path_rst;

}

MERKLE::~MERKLE() {
	delete[] segments;
	for (size_t i = 0; i < height; i++) {
		delete[] arrays[i];
	}
	delete[] arrays;
}
