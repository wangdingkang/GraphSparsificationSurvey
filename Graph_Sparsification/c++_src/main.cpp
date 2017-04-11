#include "InputGraph.h"
#include "OutputGraph.h"
#include "RandomNode.h"
#include "RandomWalk.h"
#include "Snowball.h"
#include "ForestFireSampling.h"
#include "Landmark.h"
#include <dirent.h>

#define INPUT_FOLDER "input/"

//#define LANDMARK_DEPTH 1	// Each selected landmark will 'govern' all nodes in 2 HOPs

//#define SAMPLE_RATE 0.075	// how much fraction to be sampled

#define ITERATION 10

#define SNOWBALL_N 1	// The size of starting set
#define SNOWBALL_K 5	// each time pick random K neighbors of each nodes in previous set.

#define FOREST_FIRE_N 1	// similar as SNOWBALL_N
#define FOREST_FIRE_K 5	// the number of neighbors picked ~ Geo(K).

int SAMPLE_SIZE = 1000; // default to 1000, but reset to SAMPLE_RATE * the size of original graph.
vector<double> SAMPLE_RATES = { 0.001, 0.01, 0.1 };

vector<string> fetch_all_input_files(const string input_folder) {
	struct dirent *entry;
	DIR *dp;
	vector<string> filenames;
	dp = opendir(input_folder.c_str());
	if (dp == NULL) {
		cerr << "Fold not exist." << endl;
		return filenames;
	}

	while ((entry = readdir(dp))) {
		string temp = entry->d_name;
		if (temp.find(".txt") != string::npos)
			filenames.push_back(temp);
	}

	closedir(dp);
	return filenames;
}

int main() {

	vector<string> files = fetch_all_input_files(INPUT_FOLDER);

	for (string filename : files) {
		InputGraph* g = new InputGraph(INPUT_FOLDER + filename);
		cout << "File " << filename << " Read." << endl;
		int original_graph_size = g->num_nodes();
		cout << "Original graph has " << original_graph_size << " nodes." << endl;
		for (auto rate : SAMPLE_RATES) {
			SAMPLE_SIZE = original_graph_size * rate;
			for (int iteration = 1; iteration <= ITERATION; iteration++) {
				cout << endl << "Iteration " << iteration << " started."
						<< endl;
				OutputGraph* out = new OutputGraph();

				g->show();

				// Landmark Sampling
				cout << "Landmark Sampling." << endl;
				Landmark* l = new Landmark(g->get_graph());
				EdgeGraph o1;

				// random sample
				l->random_landmark_sampling(SAMPLE_SIZE);
				cout
						<< "Landmark Sampling random sampling with random assignment "
						<< l->sampled_size << " nodes, and "
						<< l->ret_eigen.size() << "/" << l->ret_apsp.size()
						<< " edges." << endl;
				out->output_weighted(
						"output/Landmark_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename,
						l->ret_eigen);
				out->output_weighted(
						"output/Landmark_apsp_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename,
						l->ret_apsp);
				out->output_assignment(
						"output/landmark_assignment_" + to_string(SAMPLE_SIZE)
								+ "_" + to_string(iteration) + "_" + filename
								+ ".csv", l->assignment, l->cnt_cluster);
				out->output_cluster_size_distribution(
						"output/landmark_cluster_distribution_"
								+ to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename,
						l->cnt_cluster);
				delete l;

				cout << "Landmark Sampling Finished." << endl;

				//	RandomNode Sampling
//			RandomNode* rn = new RandomNode();
//			EdgeGraph o2 = rn->get_sampled_graph(g->get_graph(), SAMPLE_SIZE);
//			cout << "Random Node Sampling sampled " << SAMPLE_SIZE
//					<< " nodes, and " << o2.size() << " edges." << endl;
//			out->output_weighted("output/RandomNode_" + to_string(iteration) + "_" + filename, o2);
//			delete rn;
//
//			cout << "Random node Sampling Finished." << endl;

				// RandomWalk Sampling
				RandomWalk* rw = new RandomWalk();
				EdgeGraph o3 = rw->get_sampled_graph(g->get_graph(),
						SAMPLE_SIZE, RANDOM_WALK);
				cout << "Random Walk Sampling sampled " << rw->sampled_size
						<< " nodes, and " << o3.size() << " edges." << endl;
				out->output_weighted(
						"output/RandomWalk_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename, o3);
				delete rw;

				cout << "RandomWalk Sampling Finished." << endl;

				// Snowball Sampling
				Snowball* sb = new Snowball();
				EdgeGraph o4 = sb->snowball_sampling_with_size(g->get_graph(),
				SNOWBALL_N,
				SNOWBALL_K, SAMPLE_SIZE);
				cout << "Snowball Sampling with size sampled "
						<< sb->sampled_size << " nodes, and " << o4.size()
						<< " edges." << endl;
				out->output_weighted(
						"output/Snowball_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename, o4);
				delete sb;

				cout << "Snowball Sampling Finished." << endl;

				delete out;
			}
		}
		delete g;
	}
	return 0;
}

//		// Forest Fire Sampling
//		ForestFireSampling* ffs = new ForestFireSampling();
//		EdgeGraph o5 = ffs->ffs_sampling_with_size(g->get_graph(),
//				FOREST_FIRE_N,
//				FOREST_FIRE_K, SAMPLE_SIZE);
//		cout << "Forest Fire Sampling with size sampled " << ffs->sampled_size
//				<< " nodes." << endl;
//		out->output_weighted("output/forest_fire_" + filename, o5);
//		delete ffs;

//		cout << "Forest Fire Sampling Finished." << endl << endl;
