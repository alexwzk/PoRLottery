#include "dealer.h"

DEALER::DEALER(std::string filenamePt) {
	using namespace std;
	ifstream inputs;
	vector<zk_leaf> file_segmts;
	char inbuffer[LEAF_SIZE] = { };
	zk_leaf tmp_zleaf;
	try {
		inputs.open(filenamePt, ifstream::binary);
	} catch (ifstream::failure& err) {
		cerr << err.what() << " @ Opening file at Dealer 1st constructor."
				<< endl;
	}
	while (inputs) {
		inputs.read(inbuffer, LEAF_SIZE);
		tmp_zleaf.assign(reinterpret_cast<unsigned char*>(inbuffer), LEAF_SIZE);
		file_segmts.push_back(tmp_zleaf);
	}
	if (!inputs.eof()) {
		//File size can't be divided by the LEAF_SIZE
		//Coutest remainder of the file size
		inputs.read(inbuffer, inputs.gcount());
		for (int i = inputs.gcount(); i < LEAF_SIZE; i++) {
			inbuffer[i] = 0;
		}
		tmp_zleaf.assign(reinterpret_cast<unsigned char*>(inbuffer), LEAF_SIZE);
		file_segmts.push_back(tmp_zleaf);
	}

	mktreePt = new MERKLE(file_segmts);

}
int DEALER::createSubset() {
	if (!can_update) {
		return INVALID_ERR;
	}
	digest hashvalue;
	subset_pk.clear();
	// Hash(pk||i) for i: 0 -> l
	for (size_t i = 0; i < num_subset; i++) {
		pubkey.push_back(i); //TODO: size_t is truncated into char, is it save?
		//Coutest pubkey
		std::cout << "The pk||i : " << pubkey << std::endl;
		SHA1((unsigned char *) pubkey.data(), pubkey.size(), hashvalue);
		//pick part of the hash value and mod by n
		//Coutest extractHash
		std::cout << "extract from hash : " << COMMON::extractHash(hashvalue)
				<< " then mod by " << num_all << " turns out "
				<< COMMON::extractHash(hashvalue) % num_all << std::endl;
		subset_pk.insert(COMMON::extractHash(hashvalue) % num_all);	//TODO: Duplicates?
		pubkey.pop_back();
	}
	//Coutest subset_pk
	std::cout << "The size of subset_{pk}: " << subset_pk.size() << std::endl;
	for (auto i : subset_pk) {
		std::cout << i << " ";
	}
	std::cout << std::endl;

	can_update = false;
	return 0;
}

std::set<PATH> DEALER::createSource(std::string usr_pubkey) {
	if (pubkey.compare(usr_pubkey) != 0) {
		pubkey = usr_pubkey;
		can_update = true;
		createSubset();
		path_set.clear();
		for (auto it : subset_pk) {
			path_set.insert(mktreePt->buildPath(it));
		}
	}
	return path_set;
}
