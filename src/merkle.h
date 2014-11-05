/*
 * merkle.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PMC_MERKLE_H_
#define PMC_MERKLE_H_

#include "path.h"

template<unsigned int LEAF_BYTES>
class MERKLE {
private:
	size_t num_segmts, num_leaves, height;
	uint160 root;
	uint8_t **segments;
	uint8_t **arrays; //a hierarchical chunks of buffer (# elements from 0 to height: 1,2,4,8...)
	int now_layer, next_layer;

public:

	/**
	 * TODO Merkle setnull (?)
	 * Merkle tree 1st constructor
	 * INPUT a vector of buffer whose size is LEAF_BYTES
	 * AFFECT all private variables
	 */
	MERKLE(const std::vector< BUFFER<LEAF_BYTES> >& segmts) {
		num_segmts = segmts.size();
		height = ceil(log2(num_segmts)) + 1; // includes the root
		size_t num_elem = 1;
		size_t hash_size = root.size();
		std::vector<uint8_t> *tmp_vecPt = NULL;
		now_layer = 0;
		next_layer = 0;

		// Arrays memory allocation
		try {
			arrays = new uint8_t*[height];
		} catch (std::bad_alloc& err) {
			std::cerr << err.what() << " malloc arrays** height: " << height
					<< " @ MERKLE 1st constructor." << std::endl;
		}

		while (next_layer < height) {
			// coutest H arrays
			// cout << "now layer: " << now_layer << " has " << num_elem << " element(s). " << endl;
			try {
				arrays[next_layer] = new uint8_t[num_elem * hash_size];
			} catch (std::bad_alloc& err) {
				std::cerr << err.what()
						<< "malloc arrays* @ MERKLE 1st constructor."
						<< std::endl;
			}
			next_layer += 1;
			num_elem = num_elem << 1;
		}
		// State the current layer and its size
		now_layer = next_layer - 1;
		num_elem = num_elem >> 1;

		// Process leaves
		try {
			segments = new uint8_t*[num_elem]; // full binary tree
			for (size_t i = 0; i < num_elem; i++) {
				segments[i] = new uint8_t[LEAF_BYTES];
			}
		} catch (std::bad_alloc& err) {
			std::cerr << err.what()
					<< "malloc segments @ MERKLE 1st constrcutor." << std::endl;
		}

		// Hash the leaves (file segments)
		for (size_t i = 0; i < num_segmts; i++) {
			memcpy(segments[i], segmts[i].data, LEAF_BYTES);
			SHA1(segments[i], LEAF_BYTES, arrays[now_layer] + i * hash_size);
			// coutest memcpy
			/*std::cout << "No. " << i << ": ";
			 pmc::printHex(segments[i], LEAF_BYTES);
			 std::cout << " and its";
			 std::cout << " hash value: ";
			 pmc::printHex(arrays[now_layer][i], hash_size);*/
		}

		// Fill the leaves into a full binary tree
		num_leaves = num_segmts;
		while (num_leaves < num_elem) {
			memset(segments[num_leaves], 0, LEAF_BYTES);
			SHA1(segments[num_leaves], LEAF_BYTES,
					(arrays[now_layer] + num_leaves * hash_size));
			num_leaves++;
		}

		// Hash children digests
		next_layer = now_layer - 1;
		num_elem = num_leaves >> 1;
		while (next_layer >= 0) {
			for (size_t i = 0; i < num_elem; i++) {
				SHA1(arrays[now_layer] + i * 2 * hash_size, 2 * hash_size,
						arrays[next_layer] + i * hash_size);
			}
			now_layer--;
			next_layer = now_layer - 1;
			num_elem = num_elem >> 1;
		}

		tmp_vecPt = PMC::newByteVec(arrays[0] + 0, hash_size); //newptr
		root = uint160(*tmp_vecPt);
		delete tmp_vecPt; //deleteptr

		// coutest Merkle tree 2D array
		/*num_elem = 1;
		 for (size_t i = 0; i < height; i++) {
		 for (size_t j = 0; j < num_elem; j++) {
		 std::cout << "Print [" << i << "] [" << j << "]'s hash value: "
		 << std::endl;
		 pmc::printHex(arrays[i] + j * hash_size, hash_size);
		 }
		 num_elem = num_elem << 1;
		 }*/
	}

	/**
	 * Merkle tree destructor
	 * AFFECT memory of segments[] and arrays[][] are released
	 */
	~MERKLE() {
		delete[] segments;
		for (size_t i = 0; i < height; i++) {
			delete[] arrays[i];
		}
		delete[] arrays;
	}

	/**
	 * Builds the Merkle proof for the corresponding segment
	 * INPUT locate the wanted leaf and its Merkle proof
	 * OUTPUT a path
	 */
	PATH<LEAF_BYTES> returnPath(const size_t locate) {
		PATH<LEAF_BYTES> npath;

		if (locate >= num_leaves || arrays == NULL) {
			return npath;
		}

		size_t hash_size = root.size();
		size_t now_locate = locate;
		size_t arrays_locate = 0;
		BUFFER<LEAF_BYTES> inleaf;
		std::vector<uint8_t> *tmp_vecPt = NULL;

		inleaf.assign(segments[locate], LEAF_BYTES);
		npath.setLeafValue(inleaf);

		now_layer = height - 1;
		for (size_t i = 0; i < height - 1; i++) {
			if (PMC::isEven(now_locate)) {
				arrays_locate = (now_locate + 1) * hash_size;
			} else {
				arrays_locate = (now_locate - 1) * hash_size;
			}
			tmp_vecPt = PMC::newByteVec(arrays[now_layer] + arrays_locate,
					hash_size); //newptr
			npath.pushHashDigest(uint160(*tmp_vecPt));
			delete tmp_vecPt; //deleteptr
			now_locate = now_locate >> 1;
			now_layer--;
		}

		return npath;
	}

	/**
	 * Return the root digest
	 * OUTPUT a new copy of the root
	 */
	uint160 returnRoot() const {
		return root;
	}

	/**
	 * OUTPUT num_segmts of the Merkle tree
	 */
	size_t returnNumSegmts() const {
		return num_segmts;
	}

	/**
	 *Validates the received path is legitimate in a merkle tree
	 *INPUT a pointer to the merkle proof, its challenge index and the root digest
	 *OUTPUT true if it's correct otherwise false
	 */
	static bool verifyPath(const PATH<LEAF_BYTES>& vpath, size_t u_i,
			const uint160& root) {
		size_t next_id = u_i;
		size_t hash_size = root.size();
		uint8_t hashvalue[hash_size];
		uint8_t mkfvalue[hash_size * 2];

		BUFFER<LEAF_BYTES> nleaf = vpath.returnLeaf();
		SHA1(nleaf.data, LEAF_BYTES, hashvalue);
		std::vector<uint160> vsiblingns = vpath.returnHashSiblings();
		for (std::vector<uint160>::iterator it = vsiblingns.begin(); it != vsiblingns.end(); ++it) {
			if (PMC::isEven(next_id)) {
				memcpy(mkfvalue, hashvalue, hash_size);
				memcpy(mkfvalue + hash_size, it->begin(), hash_size);
			} else {
				memcpy(mkfvalue, it->begin(), hash_size);
				memcpy(mkfvalue + hash_size, hashvalue, hash_size);
			}
			SHA1(mkfvalue, hash_size * 2, hashvalue);
			next_id = next_id >> 1;
		}
		if (memcmp(root.begin(), hashvalue, hash_size) != 0) {
			return false;
		} else {
			return true;
		}
	}

};

#endif /*PMC_MERKLE_H_*/
