#include "merkle.h"
#include "common.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <openssl/sha.h>

MERKLE::MERKLE(leaf *segmts, size_t num_segs) {
	using namespace std;
	num_segmts = num_segs;
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
		cerr << err.what() << " @ MERKLE 1st constructor for [segments*]."
				<< endl;
	}

	// Hash the leaves (file segments)
	for (size_t i = 0; i < num_segmts; i++) {
		memcpy(segments[i], segmts[i], LEAF_SIZE);
		SHA1(segmts[i], LEAF_SIZE, arrays[now_layer][i]);
		// Coutest memcpy
		cout << "No. " << i << ": ";
		COMMON::printHex(segments[i]);
		cout << "And its hash value: ";
		COMMON::printHex(arrays[now_layer][i]);
	}

	// Fill the leaves into a full binary tree
	num_leaves = num_segmts;
	while (num_leaves < num_elem) {
		memset(segments[num_leaves], 0, LEAF_SIZE); // TODO - should be a LEAF_SIZE random bits
		SHA1(segments[num_leaves], LEAF_SIZE, arrays[now_layer][num_leaves]);
		num_leaves++;
	}

	// Coutest the rest redundant file segments
	for (size_t i = num_segmts; i < num_leaves; i++) {
		cout << "No. " << i << ": ";
		COMMON::printHex(segments[i]);
		cout << "And its hash value: ";
		COMMON::printHex(arrays[now_layer][i]);
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
			COMMON::printHex(arrays[i][j]);
		}
		num_elem = num_elem << 1;
	}

	//TODO - Coutest correctness

}

path MERKLE::buildPath(size_t loca) {
	using namespace std;
	if (loca >= num_leaves) {
		throw overflow_error("The location exceeds the max number of leaves.");
	}
	path path_rst;
	// Get the file segment F_{loca}
	memcpy(path_rst.item, segments[loca], LEAF_SIZE);

	// Retrieve the merkle proof digests
	try {
		path_rst.siblings = new digest[height - 1];
	} catch (bad_alloc& err) {
		cerr << err.what() << " @ buildPath for [*siblings]." << endl;
	}

	now_layer = height - 1;
	size_t now_loca = loca;
	for (size_t i = 0; i < height - 1; i++) {
		if (COMMON::isEven(now_loca)) {
			memcpy(path_rst.siblings[i], arrays[now_layer][now_loca + 1],
					HASH_SIZE);
		} else {
			memcpy(path_rst.siblings[i], arrays[now_layer][now_loca - 1],
					HASH_SIZE);
		}
		now_loca = now_loca >> 1;
		now_layer--;
	}

	//Coutest path
	cout << "Coutest the path of " << loca << " : " << endl;
	cout << "its item is: " << path_rst.item << " and its hash siblings are: "
			<< endl;
	for (size_t i = 0; i < height - 1; i++) {
		cout << "layer " << i << " : ";
		COMMON::printHex(path_rst.siblings[i]);
	}

	return path_rst;

}

MERKLE::~MERKLE() {
	delete [] segments;
	for (size_t i = 0; i < height; i++) {
		delete [] arrays[i];
	}
	delete [] arrays;
}

int MERKLE::returnHeight() {
	return height;
}
