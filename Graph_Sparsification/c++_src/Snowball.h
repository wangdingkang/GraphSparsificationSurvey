/*
 * Snowball.h
 *
 *  Created on: Jan 7, 2017
 *      Author: dkwan
 */

#ifndef SNOWBALL_H_
#define SNOWBALL_H_

#include "Structures.h"

class Snowball {
public:
	Snowball();

	EdgeGraph snowball_sampling(const AdjLinkGraph& g, int arg_N, int arg_K, int arg_T);

	EdgeGraph snowball_sampling_with_size(const AdjLinkGraph& g, int arg_N, int arg_K, int arg_SN);

	virtual ~Snowball();

private:

	vector<int> random_ints(int n, int k);

};

#endif /* SNOWBALL_H_ */
