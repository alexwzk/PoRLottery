/*
 * ticket.h
 *
 *  Created on: Sep 27, 2014
 *      Author: Zikai Alex Wen
 */

#ifndef PMC_TICKET_H_
#define PMC_TICKET_H_

#include <utilstrencodings.h>

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

	void clear(){
		pubkey.SetHex("0");
		seed.clear();
		mkproofs.clear();
		signatures.clear();
	}

	uint256 getHash() const{
		return Hash(BEGIN(pubkey), END(signatures));
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
