#include "InputGraph.h"
#include "OutputGraph.h"
#include "RandomNode.h"
#include "RandomWalk.h"
#include "Snowball.h"
#include "ForestFireSampling.h"
#include "Landmark.h"
#include <dirent.h>

#define INPUT_FOLDER "input/"	// input folder

#define ITERATION 100	// for each sampling rate, and each network, and each sampling method, we run 100 iterations and then average the results.
#define CUT_OFF 25		// cut_off don't care about shortest paths with length larger than 25

#define SNOWBALL_N 1	// The size of starting set
#define SNOWBALL_K 5	// each time pick random K neighbors of each nodes in previous set.

#define FOREST_FIRE_N 1	// similar as SNOWBALL_N
#define FOREST_FIRE_K 5	// the number of neighbors picked ~ Geo(K).

vector<double> SAMPLE_RATES = { 0.10, 0.20, 0.40 }; //sampling rates, 10%, 20% to 40%.

map<string, vector<double>> subset_pairwise;	// results for shortest path length distribution

// read in all networks under input folder
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

	OutputGraph* out = new OutputGraph();
	for (string filename : files) {
		InputGraph* g = new InputGraph(INPUT_FOLDER + filename);
		cout << "File " << filename << " Read." << endl;
		int original_graph_size = g->num_nodes();
		cout << "Original graph has " << original_graph_size << " nodes."
				<< endl;
		int start_s = clock();

		for (auto SAMPLE_RATE : SAMPLE_RATES) {

			int SAMPLE_SIZE = SAMPLE_RATE * g->num_nodes();

			string landmarkU_key = "LCC_Landmark_Uniform_"
					+ to_string(SAMPLE_SIZE) + "_subset.txt";
			string landmarkD_key = "LCC_Landmark_Degree_"
					+ to_string(SAMPLE_SIZE) + "_subset.txt";
			string snowball_key = "LCC_Snowball_" + to_string(SAMPLE_SIZE)
					+ "_subset.txt";
			string randomwalk_key = "LCC_Randomwalk_" + to_string(SAMPLE_SIZE)
					+ "_subset.txt";

			subset_pairwise.insert(
					make_pair(landmarkD_key, vector<double>(CUT_OFF + 1)));
			subset_pairwise.insert(
					make_pair(landmarkU_key, vector<double>(CUT_OFF + 1)));
			subset_pairwise.insert(
					make_pair(snowball_key, vector<double>(CUT_OFF + 1)));
			subset_pairwise.insert(
					make_pair(randomwalk_key, vector<double>(CUT_OFF + 1)));

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
				delete ld;

				cout << "Landmark Sampling Degree Finished." << endl;

				//	Landmark Sampling with uniformly distributed sampling prob
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
				delete sb;
			}
		}
		// write subset_distribution
		out->output_subset_spd("output/0_SubsetDistribution_" + filename,
				subset_pairwise, ITERATION);

		delete g;

		// output time consumed
		int stop_s = clock();
		cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000
				<< endl;
	}
	delete out;
	return 0;
}
