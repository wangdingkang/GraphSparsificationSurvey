/*
 * OutputGraph.h
 *
 *  Created on: 2017Äê1ÔÂ9ÈÕ
 *      Author: Dingkang Wang
 */

#ifndef OUTPUTGRAPH_H_
#define OUTPUTGRAPH_H_

#include "Structures.h"

class OutputGraph {
public:
	OutputGraph();
	void output(string filename, EdgeGraph g);
	void output_weighted(string filaname, EdgeGraph g);
	virtual ~OutputGraph();
};

#endif /* OUTPUTGRAPH_H_ */
