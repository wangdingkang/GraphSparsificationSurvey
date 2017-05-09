/*
 * Sampling.cpp
 *
 *  Created on: Dec 25, 2016
 *      Author: dkwan
 */

#include "InputGraph.h"

InputGraph::InputGraph(string filename) {
	N = M = 0;
	this->filename = filename;
	read_in_graph();
}

int InputGraph::num_nodes() {
	return this->N;
}

vector<string> InputGraph::split(string s) {
	std::istringstream iss(s);
	vector<string> ret;
	do {
		std::string sub;
		iss >> sub;
		ret.push_back(sub);
	} while (iss);
	return ret;
}

void InputGraph::read_in_graph() {
	ifstream fin;
	fin.open(filename);
	fin >> N;
	graph = AdjLinkGraph(false, N);
	int u, v;
	double w;
	string temp;
	vector<string> t;
	std::getline(fin, temp);
	std::getline(fin, temp);

	t = split(temp);

	if (t.size() == 4) {
		cout << "Input file is weighted." << endl;
		graph.weighted = true;
		graph.insert_link(std::stoi(t[0]), std::stoi(t[1]), std::stod(t[2]));
		graph.insert_link(std::stoi(t[1]), std::stoi(t[0]), std::stod(t[2]));
		M++;
		while (fin >> u >> v >> w) {
			graph.insert_link(u, v, w);
			graph.insert_link(v, u, w);
			M++;
		}
		graph.init_degree();
	} else if (t.size() == 3) {
		cout << "Input file is unweighted." << endl;
		graph.weighted = false;
		graph.insert_link(std::stoi(t[0]), std::stoi(t[1]));
		graph.insert_link(std::stoi(t[1]), std::stoi(t[0]));
		M++;
		while (fin >> u >> v >> w) {
			graph.insert_link(u, v);
			graph.insert_link(v, u);
			M++;
		}
		graph.init_degree();
	} else {
		cout << "Input file is not a valid adjacent link table of a graph."
				<< endl;
	}
	fin.close();
}


bool pairCompare(const pair<double, int> &a, const pair<double, int> &b) {
	return a.first < b.first;
}

vector<int> InputGraph::degree_random_sample(int S) const {
	assert(S <= N);
//	vector<double> temp = this->graph.degrees;
	srand(time(NULL));
	vector<pair<double, int>> value(N);
	for (int i = 0; i < N; i++) {
		double r = (double)rand() / RAND_MAX;
		double exps = -log(r);
		value[i].first = exps / graph.degrees[i];
		value[i].second = i;
	}
	vector<int> rets;
	sort(value.begin(), value.end(), pairCompare);
	for(int i = 0; i<S; i++) {
		rets.push_back(value[i].second);
	}
	return rets;
}

vector<int> InputGraph::uniform_random_sample(int S) const {
	assert(S <= N);
	vector<int> randoms;
	for (int i = 0; i < N; i++) {
		randoms.push_back(i);
	}
	srand(time(NULL));
	random_shuffle(randoms.begin(), randoms.end());
	return vector<int>(randoms.begin(), randoms.begin() + S);
}

vector<double> InputGraph::sp_distribution(unordered_set<int> &indexes,
		int cut_off) {
	vector<int> vindexes(indexes.size());
	copy(indexes.begin(), indexes.end(), vindexes.begin());
	return sp_distribution(vindexes, cut_off);
}

vector<double> InputGraph::sp_distribution(vector<int> &indexes, int cut_off) {
	// 0 - cut_off
	cout << "Calculating subset sp distribution.." << endl;
	vector<double> rets(cut_off + 1);
	fill(rets.begin(), rets.end(), 0);
	unordered_set<int> map(indexes.begin(), indexes.end());
	int num_nodes = indexes.size();
	double num_pairs = num_nodes * (num_nodes - 1);
	for (int i = 0; i < num_nodes; i++) {
		sp_bfs(rets, map, indexes[i], cut_off);
	}
	for (auto& v : rets) {
		v /= num_pairs;
	}
	return rets;
}

void InputGraph::sp_bfs(vector<double> &rets, unordered_set<int> &indexes,
		int p, int cut_off) {
	queue<pair<int, int>> q;
	q.push(make_pair(p, 0));
	bool visited[N];
	memset(visited, false, sizeof(visited));
	visited[p] = true;
	while (!q.empty()) {
		auto t = q.front();
		if (t.second != 0 && indexes.count(t.first)) {
			rets[t.second]++;
		}

		for (auto next : graph.adjlink[t.first]) {
			int np = next.v;
			if (!visited[np]) {
				int ndist = t.second + 1;
				if (ndist <= cut_off) {
					visited[np] = true;
					q.push(make_pair(np, ndist));
				}
			}
		}
		q.pop();
	}
}

InputGraph::~InputGraph() {
// TODO Auto-generated destructor stub
}

