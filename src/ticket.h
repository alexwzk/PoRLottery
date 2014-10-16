/*
 * ticket.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PMC_TICKET_H_
#define PMC_TICKET_H_

#include "path.h"

template<unsigned int LEAF_BYTES, class HASH_TYPE>
class TICKET {
public:
	HASH_TYPE pubkey;
	std::string seed;
	std::vector<PATH<LEAF_BYTES, HASH_TYPE>> mkproofs;
	std::vector<PATH<FPS_LEAFSIZE, HASH_TYPE>> signatures;

	IMPLEMENT_SERIALIZE(
			READWRITE(pubkey);
			READWRITE(seed);
			READWRITE(mkproofs);
			READWRITE(signatures);
	)

	HASH_TYPE hashOfTicket() {
		HASH_TYPE hashvalue;
		std::stringstream ss;
		this->Serialize(ss, SER_DISK, CLIENT_VERSION);
		std::string ticket_str = ss.str();
		// Coutest ticket_str
		/*std::cout << "Ticket String: " << ticket_str << std::endl;
		std::cout << "Hex of Ticket: " << pmc::stringToHex(ticket_str) << std::endl;*/
		SHA1((const unsigned char*) ticket_str.c_str(), ticket_str.length(),
				hashvalue.begin());
		return hashvalue;
	}

	TICKET<LEAF_BYTES, HASH_TYPE>& operator=(
			const TICKET<LEAF_BYTES, HASH_TYPE>& t) {
		pubkey = t.pubkey;
		seed = t.seed;
		mkproofs = t.mkproofs;
		signatures = t.signatures;
		return (*this);
	}

};

#endif /* PMC_TICKET_H_ */
