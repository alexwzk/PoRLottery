/*
 * ticket.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PMC_TICKET_H_
#define PMC_TICKET_H_

#include "path.h"

template<unsigned int PMC_LFBYTES, unsigned int FPS_LFBYTES>
class TICKET {
public:
	uint160 pubkey;
	std::string seed;
	std::vector< PATH<PMC_LFBYTES> > mkproofs;
	std::vector< PATH<FPS_LFBYTES> > signatures;

	IMPLEMENT_SERIALIZE(
			READWRITE(pubkey);
			READWRITE(seed);
			READWRITE(mkproofs);
			READWRITE(signatures);
	)

	uint160 hashOfTicket() {
		uint160 hashvalue;
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

	TICKET<PMC_LFBYTES, FPS_LFBYTES>& operator=(
			const TICKET<PMC_LFBYTES, FPS_LFBYTES>& t) {
		pubkey = t.pubkey;
		seed = t.seed;
		mkproofs = t.mkproofs;
		signatures = t.signatures;
		return (*this);
	}

};

#endif /* PMC_TICKET_H_ */
