/*
 * Sampling.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "Sampling.h"

Sampling::Sampling() {
	// TODO Auto-generated constructor stub
	read_in_graph();
}

Sampling:: Sampling(string filename) {
	N = M = 0;
	this->filename = filename;
	Sampling();
}

void Sampling::read_in_graph() {
	ifstream fin;
	fin.open(filename);
	fin >> N >> M;
	paths = vector<vector<int> >(N, vector<int>());
	int a, b;
	for (int i = 0; i < M; i++) {
		fin >> a >> b;
		paths[a].push_back(b);
		/*
		 * duplicate edges?/??? see the input file to decide.
		 *
		 *
		 */
		// paths[b].push_back(a);
	}
	fin.close();
}



/*
int Sampling::get_real_index(int sudo_index) {
	if (index_map[sudo_index] == -1) {
		index_map[sudo_index] = cnt_index;
		return cnt_index++;
	} else {
		return index_map[sudo_index];
	}
}

void Sampling::init_lcc() {
	for(int i=0; i<cnt_index; i++){
		if(visited[i] == -1){
			temp_comp_set.clear();
			dfs_search(i);
			if((int)temp_comp_set.size() > cnt_temp_max){
				cnt_temp_max = temp_comp_set.size();
				max_con_comp = temp_comp_set;
			}
		}
	}
}

void Sampling::dfs_search(int s) {
	visited[s] = 0;
	temp_comp_set.push_back(s);
	for(auto child : paths[s]){
		if(visited[child] == -1){
			dfs_search(child);
		}
	}
}
*/

Sampling::~Sampling() {
	// TODO Auto-generated destructor stub
}

