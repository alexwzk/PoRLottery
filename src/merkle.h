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
	uint8_t **segments;
	uint256 **arrays; //a hierarchical chunks of buffer (# elements from 0 to height: 1,2,4,8...)
	int now_layer, next_layer;

public:

	/**
	 * TODO Merkle setnull (?)
	 * Merkle tree 1st constructor
	 * INPUT a vector of buffer whose size is LEAF_BYTES
	 * AFFECT all private variables
	 */
	MERKLE(const std::vector<BUFFER<LEAF_BYTES> >& segmts) {
		num_segmts = segmts.size();
		height = ceil(log2(num_segmts)) + 1; // includes the root
		size_t num_elem = 1;
		now_layer = 0;
		next_layer = 0;

		// Arrays memory allocation
		try {
			arrays = new uint256*[height];
		} catch (std::bad_alloc& err) {
			throw error(err.what());
		}

		while (next_layer < height) {
			// coutest H arrays
			// cout << "now layer: " << now_layer << " has " << num_elem << " element(s). " << endl;
			try {
				arrays[next_layer] = new uint256[num_elem];
			} catch (std::bad_alloc& err) {
				throw error(err.what());
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
			throw error(err.what());
		}

		// Hash the leaves (file segments)
		for (size_t i = 0; i < num_segmts; i++) {
			memcpy(segments[i], segmts[i].begin(), LEAF_BYTES);
			arrays[now_layer][i] = Hash(segments[i], segments[i] + LEAF_BYTES);
			// coutest memcpy
			/*std::cout << "No. " << i << ": ";
			PMC::printHex(segments[i], LEAF_BYTES);
			std::cout << " and its";
			std::cout << " hash value: ";
			std::cout << arrays[now_layer][i].GetHex() << std::endl;*/
		}

		// Fill the leaves into a full binary tree
		num_leaves = num_segmts;
		while (num_leaves < num_elem) {
			memset(segments[num_leaves], 0, LEAF_BYTES);
			arrays[now_layer][num_leaves] = Hash(segments[num_leaves],
					segments[num_leaves] + LEAF_BYTES);
			num_leaves++;
		}

		// Hash children digests
		next_layer = now_layer - 1;
		num_elem = num_leaves >> 1;
		size_t loc_input = 0;
		while (next_layer >= 0) {
			for (size_t i = 0; i < num_elem; i++) {
				loc_input = i << 1;
				arrays[next_layer][i] = Hash(arrays[now_layer][loc_input].begin(),
						arrays[now_layer][loc_input].end(),
						arrays[now_layer][loc_input + 1].begin(),
						arrays[now_layer][loc_input + 1].end());
			}
			now_layer--;
			next_layer = now_layer - 1;
			num_elem = num_elem >> 1;
		}
		// coutest Merkle tree 2D array
		/*num_elem = 1;
		for (size_t i = 0; i < height; i++) {
			for (size_t j = 0; j < num_elem; j++) {
				std::cout << "Print [" << i << "] [" << j << "]'s hash value: "
						<< arrays[i][j].GetHex() << std::endl;
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

		size_t now_locate = locate;
		size_t arrays_locate = 0;
		BUFFER<LEAF_BYTES> inleaf;

		inleaf.assign(segments[locate], LEAF_BYTES);
		npath.setLeafValue(inleaf);

		now_layer = height - 1;
		while (now_layer > 0) {
			if (PMC::isEven(now_locate)) {
				arrays_locate = now_locate + 1;
			} else {
				arrays_locate = now_locate - 1;
			}
			npath.pushHashDigest(arrays[now_layer][arrays_locate]);
			now_locate = now_locate >> 1;
			now_layer--;
		}

		return npath;
	}

	/**
	 * Return the root digest
	 * OUTPUT a new copy of the root
	 */
	uint256 returnRoot() const {
		return arrays[0][0];
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
			const uint256& root) {
		size_t next_id = u_i;
		BUFFER<LEAF_BYTES> nleaf = vpath.returnLeaf();
		uint256 hashvalue = Hash(nleaf.begin(), nleaf.end());
		std::vector<uint256> hash_siblings = vpath.returnHashSiblings();
		for (size_t i = 0; i < hash_siblings.size(); i++) {
			if (PMC::isEven(next_id)) {
				hashvalue = Hash(hashvalue.begin(), hashvalue.end(),
						hash_siblings[i].begin(), hash_siblings[i].end());
			} else {
				hashvalue = Hash(hash_siblings[i].begin(), hash_siblings[i].end(),
						hashvalue.begin(), hashvalue.end());
			}
			next_id = next_id >> 1;
		}
		if (root.CompareTo(hashvalue) != 0) {
			return false;
		} else {
			return true;
		}
	}

};

#endif /*PMC_MERKLE_H_*/
