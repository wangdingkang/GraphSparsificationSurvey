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
#define CUT_OFF 25

#define SNOWBALL_N 1	// The size of starting set
#define SNOWBALL_K 5	// each time pick random K neighbors of each nodes in previous set.

#define FOREST_FIRE_N 1	// similar as SNOWBALL_N
#define FOREST_FIRE_K 5	// the number of neighbors picked ~ Geo(K).

// int SAMPLE_SIZE = 1000; // default to 1000, but reset to SAMPLE_RATE * the size of original graph.
vector<int> SAMPLE_SIZES = { 500, 1000, 1500, 2000 };

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

map<string, vector<double>> subset_pairwise;

int main() {

	vector<string> files = fetch_all_input_files(INPUT_FOLDER);

	OutputGraph* out = new OutputGraph();
	for (string filename : files) {
		InputGraph* g = new InputGraph(INPUT_FOLDER + filename);
		cout << "File " << filename << " Read." << endl;
		int original_graph_size = g->num_nodes();
		cout << "Original graph has " << original_graph_size << " nodes."
				<< endl;
		for (auto SAMPLE_SIZE : SAMPLE_SIZES) {

			string landmarkU_key = "LCC_Landmark_Uniform_"
					+ to_string(SAMPLE_SIZE) + "_subset.txt";
			string landmarkD_key = "LCC_Landmark_Degree_"
					+ to_string(SAMPLE_SIZE) + "_subset.txt";
			string snowball_key = "LCC_Snowball_" + to_string(SAMPLE_SIZE)
					+ "_subset.txt";
			string randomwalk_key = "LCC_Randomwalk_" + to_string(SAMPLE_SIZE)
					+ "_subset.txt";

			subset_pairwise.insert(
					make_pair(landmarkD_key, vector<double>(CUT_OFF)));
			subset_pairwise.insert(
					make_pair(landmarkU_key, vector<double>(CUT_OFF)));
			subset_pairwise.insert(
					make_pair(snowball_key, vector<double>(CUT_OFF)));
			subset_pairwise.insert(
					make_pair(randomwalk_key, vector<double>(CUT_OFF)));

			for (int iteration = 1; iteration <= ITERATION; iteration++) {
				cout << endl << "Iteration " << iteration << " started."
						<< endl;
				g->show();

				// Landmark sampling using degree biased distrbution
				cout << "Landmark Degree Sampling." << endl;
				Landmark* ld = new Landmark(*g);

				ld->landmark_sampling(SAMPLE_SIZE, DEGREE_BIASED);
				cout
						<< "Landmark Sampling degree sampling with random assignment "
						<< ld->sampled_size << " nodes, and "
						<< ld->ret_eigen.size() << "/" << ld->ret_apsp.size()
						<< " edges." << endl;

				vector<double> t0 = g->sp_distribution(ld->subset, CUT_OFF);
				for (int c = 0; c <= CUT_OFF; c++) {
					subset_pairwise[landmarkD_key][c] += t0[c];
				}

				out->output_weighted(
						"output/LandmarkDApsp_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename,
						ld->ret_apsp);
				out->output_subset(
						"output/LandmarkDSubset_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename,
						ld->subset);

				delete ld;

				cout << "Landmark Sampling Degree Finished." << endl;

				// Landmark Sampling with uniformly distributed sampling prob
				cout << "Landmark Uniform Sampling." << endl;
				Landmark* lu = new Landmark(*g);

				lu->landmark_sampling(SAMPLE_SIZE, UNIFORM);
				cout
						<< "Landmark Sampling random sampling with random assignment "
						<< lu->sampled_size << " nodes, and "
						<< lu->ret_eigen.size() << "/" << lu->ret_apsp.size()
						<< " edges." << endl;

				vector<double> t1 = g->sp_distribution(lu->subset, CUT_OFF);
				for (int c = 0; c <= CUT_OFF; c++) {
					subset_pairwise[landmarkU_key][c] += t1[c];
				}

				out->output_weighted(
						"output/LandmarkUApsp_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename,
						lu->ret_apsp);
				out->output_subset(
						"output/LandmarkUSubset_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename,
						lu->subset);

				delete lu;

				cout << "Landmark Uniform Sampling Finished." << endl;

				// RandomWalk Sampling
				RandomWalk* rw = new RandomWalk();
				EdgeGraph o3 = rw->get_sampled_graph(g->graph, SAMPLE_SIZE,
						RANDOM_WALK);

				cout << "Random Walk Sampling sampled " << rw->sampled_size
						<< " nodes, and " << o3.size() << " edges." << endl;

				vector<double> t2 = g->sp_distribution(rw->subset, CUT_OFF);
				for (int c = 0; c <= CUT_OFF; c++) {
					subset_pairwise[randomwalk_key][c] += t2[c];
				}

				out->output_weighted(
						"output/RandomWalk_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename, o3);
				out->output_subset(
						"output/RandomWalkSubset_" + to_string(SAMPLE_SIZE)
								+ "_" + to_string(iteration) + "_" + filename,
						rw->subset);
				delete rw;

				cout << "RandomWalk Sampling Finished." << endl;

				// Snowball Sampling
				Snowball* sb = new Snowball();
				EdgeGraph o4 = sb->snowball_sampling_with_size(g->graph,
				SNOWBALL_N,
				SNOWBALL_K, SAMPLE_SIZE);
				cout << "Snowball Sampling with size sampled "
						<< sb->sampled_size << " nodes, and " << o4.size()
						<< " edges." << endl;

				vector<double> t3 = g->sp_distribution(sb->subset, CUT_OFF);
				for (int c = 0; c <= CUT_OFF; c++) {
					subset_pairwise[snowball_key][c] += t3[c];
				}

				out->output_weighted(
						"output/Snowball_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename, o4);

				out->output_subset(
						"output/SnowballSubset_" + to_string(SAMPLE_SIZE) + "_"
								+ to_string(iteration) + "_" + filename,
						sb->subset);

				delete sb;

				cout << "Snowball Sampling Finished." << endl;

			}
		}
		// write subset_distribution
		out->output_subset_spd("output/0_SubsetDistribution_" + filename,
				subset_pairwise, ITERATION);

		delete g;
	}
	delete out;
	return 0;
}



//				out->output_weighted(
//						"output/Landmark_" + to_string(SAMPLE_SIZE) + "_"
//								+ to_string(iteration) + "_" + filename,
//						l->ret_eigen);

//				out->output_assignment(
//						"output/landmarkAssignment_" + to_string(SAMPLE_SIZE)
//								+ "_" + to_string(iteration) + "_" + filename
//								+ ".csv", l->assignment, l->cnt_cluster);
//				out->output_cluster_size_distribution(
//						"output/landmarkClusterDistribution_"
//								+ to_string(SAMPLE_SIZE) + "_"
//								+ to_string(iteration) + "_" + filename,
//						l->cnt_cluster);

//			RandomNode Sampling
//			RandomNode* rn = new RandomNode();
//			EdgeGraph o2 = rn->get_sampled_graph(g->get_graph(), SAMPLE_SIZE);
//			cout << "Random Node Sampling sampled " << SAMPLE_SIZE
//					<< " nodes, and " << o2.size() << " edges." << endl;
//			out->output_weighted("output/RandomNode_" + to_string(iteration) + "_" + filename, o2);
//			delete rn;
//
//			cout << "Random node Sampling Finished." << endl;

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
