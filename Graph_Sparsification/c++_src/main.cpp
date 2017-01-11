#include "InputGraph.h"
#include "OutputGraph.h"
#include "RandomNode.h"
#include "RandomWalk.h"
#include "Snowball.h"
#include "Landmark.h"

#define INPUT_FILENAME string("input/LCC_CA-GrQc.txt")
#define OUTPUT_FILENAME string("LCC_CA-GrQc.txt")
#define LANDMARK_DEPTH 2
#define SAMPLE_SIZE 1000
#define SNOWBALL_N 10
#define SNOWBALL_K 5

int main() {
	InputGraph* g = new InputGraph(INPUT_FILENAME);
	OutputGraph* out = new OutputGraph();

	cout << "File " << INPUT_FILENAME << " Read." << endl;

	// Landmark Sampling
	Landmark* l = new Landmark(LANDMARK_DEPTH, g->get_paths());
	EdgeGraph o1 = l->get_sampled_graph();
	out->output("output/landmark_" + OUTPUT_FILENAME, o1);
	delete l;

	cout << "Landmark Sampling Finished." << endl;

	// RandomNode Sampling
	RandomNode* rn = new RandomNode();
	EdgeGraph o2 = rn->get_sampled_graph(g->get_paths(), SAMPLE_SIZE);
	out->output("output/random_node_" + OUTPUT_FILENAME, o2);
	delete rn;

	cout << "Random node Sampling Finished." << endl;

	// RandomWalk Sampling
	RandomWalk* rw = new RandomWalk();
	EdgeGraph o3 = rw->get_sampled_graph(g->get_paths(), SAMPLE_SIZE, RANDOM_WALK);
	out->output("output/random_walk_" + OUTPUT_FILENAME, o3);
	delete rw;

	cout << "RandomWalk Sampling Finished." << endl;

	// Snowball Sampling
	Snowball* sb = new Snowball();
	EdgeGraph o4 = sb->snowball_sampling_with_size(g->get_paths(), SNOWBALL_N, SNOWBALL_K, SAMPLE_SIZE);
	out->output("output/snowball_" + OUTPUT_FILENAME, o4);
	delete sb;

	cout << "Snowball Sampling Finished." << endl;

	delete out;
	delete g;
	return 0;
}
