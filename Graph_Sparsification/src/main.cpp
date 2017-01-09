#include "Sampling.h"

#define NODE_NUM 20000
#define NODE_LIMIT 200000
#define SEARCH_DEPTH 2

typedef pair<int, int> PII;

ifstream fin;
ofstream fout;

int N, M; // number of vertices and edges in the original graph
int index_map[NODE_LIMIT], cnt_index = 0; // for index mapping

vector<int> paths[NODE_NUM]; // edges
vector<int> max_con_comp;  // indexes of the points in the largest connected component.

int visited[NODE_NUM]; // -1 not visited yet, otherwise the landmark index it belongs.
int depth[NODE_NUM]; // the distance between a node to its landmark
int cnt_landmark = 0; // count of the landmarks

int degree[NODE_NUM]; // record the degrees of each vertex

int output[NODE_NUM][NODE_NUM]; // record the landmark graph

// index mapping
int get_real_index(int sudo_index) {
	if (index_map[sudo_index] == -1) {
		index_map[sudo_index] = cnt_index;
		return cnt_index++;
	} else {
		return index_map[sudo_index];
	}
}

int cnt_temp_max; // record the temp largest connected component
vector<int> temp_comp_set; // record ids of the vertices in the component

// find out the connected component
void dfs_search(int s){
	visited[s] = 0;
	temp_comp_set.push_back(s);
	for(auto child : paths[s]){
		if(visited[child] == -1){
			dfs_search(child);
		}
	}
}

// find out the largest connected component from the original graph using dfs search
void init_max_con_comp(){
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

// find all the vertices governed by the given landmark
void bfs_search(int s) {
	queue<PII> bfs_queue;
	bfs_queue.push(make_pair(s, 0));
	while (!bfs_queue.empty()) {
		PII root = bfs_queue.front();
		bfs_queue.pop();
		int ri = root.first;
		int rdep = root.second;
		visited[ri] = cnt_landmark;
		depth[ri] = rdep;
		for (auto child : paths[ri]) {
			degree[child]--;
			if (visited[child] == -1) {
				if (rdep < SEARCH_DEPTH)
					bfs_queue.push(make_pair(child, rdep + 1));
			} else if(visited[child] != cnt_landmark){
				int its_comp = visited[child];
				int its_depth = depth[child];
				int ndepth = rdep + its_depth + 1;
				if (ndepth < output[its_comp][cnt_landmark]
						|| !output[its_comp][cnt_landmark])
					output[its_comp][cnt_landmark] = output[cnt_landmark][its_comp] =
							ndepth;
			}
		}
	}
}

int main() {

	/*
	 * read in the original graph.
	 */
	fin.open("D:CA-AstroPh.txt");

	fin >> N >> M;
	int a, b;

	memset(visited, -1, sizeof(visited));
	memset(index_map, -1, sizeof(index_map));

	// map the original indexes to 0 - cnt_index.
	for (int i = 0; i < M; i++) {
		fin >> a >> b;
		a = get_real_index(a);
		b = get_real_index(b);
		paths[a].push_back(b);
		//paths[b].push_back(a);
	}

	fin.close();

	/* output the graph of the largest connected component.
	 * the first line contains the number (N) of the vertices in the graph,
	 * then there are N blocks, each block starts with the id (ID) of the corresponding vertex
	 * and the next line starts with the out-degree (M) of this vertex and followed by M ids of neighbor vertices.
	 */
	fout.open("D:Max_Comp.txt");

	init_max_con_comp();

	fout << max_con_comp.size() << endl;

	for(int i=0; i<(int)max_con_comp.size(); i++){
		fout << max_con_comp[i] << endl;
		fout << paths[max_con_comp[i]].size() << " ";
		for(auto k : paths[max_con_comp[i]]){
			fout << k << " ";
		}
		fout << endl;
	}

	fout.close();

	memset(visited, -1, sizeof(visited));

	for (int i = 0; i < cnt_index; i++) {
		degree[i] = paths[i].size();
	}

	/* find out all the landmarks, each time we will pick the vertex with the maximum out-degree as the
	 * new landmark, and then search from it.
	 */
	bool is_done = false;
	while (!is_done) {
		int max_index = -1;
		int max_degree = -1;
		int len = max_con_comp.size();
		for (int i = 0; i < len; i++) {
			int rid = max_con_comp[i];
			if (visited[rid] == -1 && degree[rid] > max_degree) {
				max_index = rid;
				max_degree = degree[rid];
			}
		}
		if (max_index != -1) {
			bfs_search(max_index);
			cnt_landmark++;
		} else {
			is_done = true;
		}
	}

	/*
	 * output the id of the landmark each vertex belongs to.
	 * first line is the number (N) of the vertices, and followed by (N) numbers denoting
	 * the id of the landmark the ith vertex belongs to and the distance to it.
	 * -1: not belong to the largest connected component
	 * 0 - cnt_landmark the id of belonged landmark.
	 */
	fout.open("D:Which_Landmark_I_Belong_To.txt");

	fout << cnt_index << endl;
	for(int i=0; i<cnt_index; i++){
		fout << visited[i] << " " << depth[i] << endl;
	}

	fout.close();

	/*
	 * output the landmark map
	 * the first line contains the number (N) of landmarks, and then following by N lines,
	 * each line contains N numbers, ith line denotes the shortest path distance from the
	 * ith landmark to other landmarks.
	 */

	fout.open("D:Landmarks_Map.txt");

	fout << cnt_landmark << endl;

	for (int i = 0; i < cnt_landmark; i++) {
		for (int j = 0; j < cnt_landmark; j++) {
			fout << output[i][j] << " ";
		}
		fout << endl;
	}

	fout.close();

	return 0;
}
