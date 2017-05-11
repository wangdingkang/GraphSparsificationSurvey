/*
 * ForestFireSampling.h
 *
 *  Created on: Jan 13, 2017
 *      Author: dkwan
 */

#ifndef FORESTFIRESAMPLING_H_
#define FORESTFIRESAMPLING_H_

#include "Structures.h"

class ForestFireSampling {
public:
	ForestFireSampling();

	int sampled_size;

	EdgeGraph get_sampled_graph(const AdjLinkGraph& g, int arg_N, int arg_K,
			int arg_T);

	EdgeGraph ffs_sampling_with_size(const AdjLinkGraph &g, int arg_N,
			int arg_K, int arg_SN);

	virtual ~ForestFireSampling();

private:

	class GeometricRandomGenerator {
	public:
		std::geometric_distribution<int> distribution;
		GeometricRandomGenerator(int arg_K) {
			counter = 0;
			this->distribution = std::geometric_distribution<int>(1.0 / arg_K);
		}
		int operator()() {
			std::default_random_engine rand_dev(time(NULL) + (counter++));
			std::mt19937 rand_engine(rand_dev());
			return distribution(rand_engine) + 1;
		}
		;
	private:
		int counter;
	};

	vector<int> random_ints(int n, int k) {
		k = min(k, n);
		vector<int> v(n);
		for (int i = 0; i < n; ++i) {
			v[i] = i;
		}

		default_random_engine gen(time(NULL));
		std::mt19937 g(gen());
		std::shuffle(v.begin(), v.end(), g);
		return vector<int>(v.begin(), v.begin() + k);
	}
};

#endif /* FORESTFIRESAMPLING_H_ */
