/*
 * OutputGraph.cpp
 *
 *  Created on: 2017Äê1ÔÂ9ÈÕ
 *      Author: Dingkang Wang
 */

#include "OutputGraph.h"

OutputGraph::OutputGraph() {
	// TODO Auto-generated constructor stub

}

string get_random_rgd() {
	stringstream ss;
	int R = rand() % 256;
	int G = rand() % 256;
	int B = rand() % 256;
	ss << std::setfill('0') << std::setw(2) << std::hex << R << G << B;
	return "#" + ss.str();
}

void OutputGraph::output(string filename, EdgeGraph g) {
	ofstream fout;
	fout.open(filename);
	for (auto &edge : g) {
		fout << edge.u << " " << edge.v << endl;
	}
	fout.close();
}

void OutputGraph::output_assignment(string filename, vector<int>& assignment,
		vector<int>& cnt_cluster) {
	ofstream fout;
	string BLACK = "#000000";
	fout.open(filename);
	fout << "Id;Color" << endl;
	vector<string> colors((int) cnt_cluster.size());
	for (int i = 0; i < (int) colors.size(); i++) {
		colors[i] = "";
	}
	for (int i = 0; i < (int) assignment.size(); i++) {
		fout << i << ";";
		int c = assignment[i];
		if (colors[c] == "") {
			if (cnt_cluster[c] <= 5) {
				colors[c] = BLACK;
			} else {
				colors[c] = get_random_rgd();
			}
		}
		fout << colors[c] << endl;
	}
	fout.close();
}

void OutputGraph::output_cluster_size_distribution(string filename,
		vector<int>& cnt_cluster) {

	int maxv = *max_element(cnt_cluster.begin(), cnt_cluster.end());
	int cnt[maxv + 1] = { 0 };
	for (auto& t : cnt_cluster) {
		cnt[t]++;
	}

	ofstream fout;
	fout.open(filename);
	for (int i = 1; i <= maxv; i++) {
		fout << i << " " << cnt[i] << endl;
	}
	fout.close();
}

void OutputGraph::output_weighted(string filename, EdgeGraph g) {
	ofstream fout;
	fout.open(filename);
	for (auto &edge : g) {
		fout << edge.u << " " << edge.v << " " << edge.w << endl;
	}
	fout.close();
}

OutputGraph::~OutputGraph() {
	// TODO Auto-generated destructor stub
}

