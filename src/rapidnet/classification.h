#include <vector>
#include <utility>
#include <string>

std::vector<std::pair<std::string, float> > getPrediction(std::string file);

std::vector<std::pair<std::string, float> > classifyImage(int clf_id, std::string img_file);

int initClassifier(int clfid);

