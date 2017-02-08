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

#define SAMPLE_RATE 0.075	// how much fraction to be sampled

#define SNOWBALL_N 1	// The size of starting set
#define SNOWBALL_K 5	// each time pick random K neighbors of each nodes in previous set.

#define FOREST_FIRE_N 1	// similar as SNOWBALL_N
#define FOREST_FIRE_K 5	// the number of neighbors picked ~ Geo(K).

int SAMPLE_SIZE = 1000; // default to 1000, but reset to SAMPLE_SIZE * the size of original graph.

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
//	for (; SAMPLE_SIZE < 1001; SAMPLE_SIZE += 100) {
		for (string filename : files) {
			InputGraph* g = new InputGraph(INPUT_FOLDER + filename);
			OutputGraph* out = new OutputGraph();

			// SAMPLE_SIZE = SAMPLE_RATE * g->num_nodes();

			cout << "File " << filename << " Read." << endl;
			g->show();

			// Landmark Sampling
			cout << "Landmark Sampling." << endl;
			Landmark* l = new Landmark(g->get_graph());
			EdgeGraph o1;
//		// based on the degree, selected the nodes with largest degree first.
//		o1 = l->get_sampled_graph_degree_biased();
//		cout << "Landmark Sampling basic degree biased sampled "
//				<< l->sampled_size << " nodes." << endl;
//		out->output_weighted(
//				"output/landmark_degree_biased_"
//						+ string(
//								"depth_") + to_string(LANDMARK_DEPTH) + "_" + filename,
//				o1);

//		// same as the above one, but assign node to a random landmark if it can be reached by multiple landmarks.
//		o1 = l->get_sampled_graph_degree_biased_random_assignment();
//		cout << "Landmark Sampling degree biased with random assignment "
//				<< l->sampled_size << " nodes." << endl;
//		out->output_weighted(
//				"output/landmark_degree_biased_random_assignment_"
//						+ string(
//								"depth_") + to_string(LANDMARK_DEPTH) + "_" + filename,
//				o1);
//
//		// uniform sample with random assignment.
//		o1 = l->get_sampled_graph_uniform_random_assignment();
//		cout << "Landmark Sampling random sampling with random assignment "
//				<< l->sampled_size << " nodes." << endl;
//		out->output_weighted(
//				"output/landmark_uniform_random_assignment_"
//						+ string(
//								"depth_") + to_string(LANDMARK_DEPTH) + "_" + filename,
//				o1);

			// farthest sample
			o1 = l->random_landmark_sampling(SAMPLE_SIZE);
			cout
					<< "Landmark Sampling random sampling with random assignment "
					<< l->sampled_size << " nodes, and " << o1.size()
					<< " edges." << endl;
			out->output_weighted("output/landmark_farthest_" + to_string(SAMPLE_SIZE)
//						+ string(
//								"depth_") + to_string(LANDMARK_DEPTH) + "_"
					+ filename, o1);
			delete l;

			cout << "Landmark Sampling Finished." << endl;

//		// RandomNode Sampling
//		RandomNode* rn = new RandomNode();
//		EdgeGraph o2 = rn->get_sampled_graph(g->get_graph(), SAMPLE_SIZE);
//		cout << "Random Node Sampling sampled " << SAMPLE_SIZE << " nodes, and "
//				<< o2.size() << " edges." << endl;
//		out->output_weighted("output/random_node_" + filename, o2);
//		delete rn;
//
//		cout << "Random node Sampling Finished." << endl;
//
//		// RandomWalk Sampling
//		RandomWalk* rw = new RandomWalk();
//		EdgeGraph o3 = rw->get_sampled_graph(g->get_graph(), SAMPLE_SIZE,
//				RANDOM_WALK);
//		cout << "Random Walk Sampling sampled " << rw->sampled_size
//				<< " nodes, and " << o3.size() << " edges." << endl;
//		out->output_weighted("output/random_walk_" + filename, o3);
//		delete rw;
//
//		cout << "RandomWalk Sampling Finished." << endl;
//
//		// Snowball Sampling
//		Snowball* sb = new Snowball();
//		EdgeGraph o4 = sb->snowball_sampling_with_size(g->get_graph(),
//		SNOWBALL_N,
//		SNOWBALL_K, SAMPLE_SIZE);
//		cout << "Snowball Sampling with size sampled " << sb->sampled_size
//				<< " nodes, and " << o4.size() << " edges." << endl;
//		out->output_weighted("output/snowball_" + filename, o4);
//		delete sb;
//
//		cout << "Snowball Sampling Finished." << endl;
//
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

			delete out;
			delete g;
//		}
	}
	return 0;
}
