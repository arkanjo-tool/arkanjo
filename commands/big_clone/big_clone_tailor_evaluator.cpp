#include "big_clone_tailor_evaluator.hpp"

void Big_Clone_Tailor_Evaluator::read_clone_labels(){
	count_of_samples_by_type = vector<int>(NUMBER_OF_TYPES);
	vector<string> content = Utils::read_file_generic(CLONE_LABELS_FILE_PATH);
	for(auto line : content){
		vector<string> tokens = Utils::split_string(line,',');
		if(int(tokens.size()) < 4){
			continue;
		}
		int id0 = stoi(tokens[0]);
		int id1 = stoi(tokens[1]);
		int type = stoi(tokens[3]);
		if(id0 > id1){
			swap(id0,id1);
		}
		pair<int,int> aux = {id0,id1};
		id_pair_to_type[aux] = type;
		count_of_samples_by_type[type] += 1;
	}
}

int Big_Clone_Tailor_Evaluator::path_to_id(Path path){
	string relative_path = path.build_relative_path();
	vector<string> tokens = Utils::split_string(relative_path,'/');
	string file_name = tokens.back();
	for(int i = 0; i < int(EXTENSION.size()); i++){
		file_name.pop_back();
	}
	return stoi(file_name);
}

vector<tuple<double,int,int>> Big_Clone_Tailor_Evaluator::similar_path_pairs_formated_with_id(){
	auto similar_path_pairs = similarity_table->get_all_path_pairs_and_similarity_sorted_by_similarity();
	vector<tuple<double,int,int>> ret;
	for(auto [similarity,path0,path1] : similar_path_pairs){
		int id0 = path_to_id(path0);
		int id1 = path_to_id(path1);
		if(id0 > id1){
			swap(id0,id1);
		}
		ret.push_back({similarity,id0,id1});
	}
	return ret;
}

bool Big_Clone_Tailor_Evaluator::is_relevant_pair(int id0, int id1){
	pair<int,int> ids = {id0,id1};
	return id_pair_to_type.find(ids) != id_pair_to_type.end();
}

set<pair<int,int>> Big_Clone_Tailor_Evaluator::filter_similar_id_pairs_only_relevant_ones(
		vector<pair<int,int>> similar_id_pairs){
	set<pair<int,int>> ret;
	for(auto [id0,id1] : similar_id_pairs){
		if(is_relevant_pair(id0,id1)){
			ret.insert({id0,id1});
		}
	}
	return ret;
}

vector<pair<int,int>> Big_Clone_Tailor_Evaluator::filter_similar_path_pairs_by_similarity(
		vector<tuple<double,int,int>> similar_id_pairs,
		double minimum_similarity){
	vector<pair<int,int>> ret;
	for(auto [similarity,id0,id1] : similar_id_pairs){
		if(similarity >= minimum_similarity){
			ret.push_back({id0,id1});
		}
	}
	return ret;
}

vector<int> Big_Clone_Tailor_Evaluator::build_frequency_corrected_guessed_by_type(
		vector<pair<int,int>> similar_id_pairs){
	set<pair<int,int>> similar_id_pairs_set = filter_similar_id_pairs_only_relevant_ones(similar_id_pairs);
	vector<int> frequency(NUMBER_OF_TYPES);
	for(auto ids : similar_id_pairs_set){
		frequency[id_pair_to_type[ids]] += 1;
	}
	//for not clone if it is marked as duplicate count is wrong instead of right
	frequency[NOT_CLONE_TYPE_ID] *= -1;
	frequency[NOT_CLONE_TYPE_ID] += count_of_samples_by_type[NOT_CLONE_TYPE_ID];
	return frequency;
}

double Big_Clone_Tailor_Evaluator::calc_recall(vector<int> frequency, int type){
	double TP = frequency[type];
	double FN = count_of_samples_by_type[type] - frequency[type];
	double recall = TP/(TP+FN);
	return recall;
}

void Big_Clone_Tailor_Evaluator::print_recall_per_type(vector<int> frequency){
	cout << RECALL_PER_TYPE_PRINT << '\n';
	for(int type = 0; type < NUMBER_OF_TYPES; type++){
		double recall = calc_recall(frequency,type);
		cout << ID_TO_TYPE_LABEL[type] << ' ';
		cout << fixed << setprecision(2) << recall << '\n';
	}
}

void Big_Clone_Tailor_Evaluator::evaluate(double minimum_similarity){
	vector<tuple<double,int,int>> similar_id_pairs_similarity =  similar_path_pairs_formated_with_id();
	vector<pair<int,int>> similar_id_pairs = filter_similar_path_pairs_by_similarity(
			similar_id_pairs_similarity,
			minimum_similarity);
	vector<int> frequency = build_frequency_corrected_guessed_by_type(similar_id_pairs);
	print_recall_per_type(frequency);
}

Big_Clone_Tailor_Evaluator::Big_Clone_Tailor_Evaluator(Similarity_Table *_similarity_table){
	similarity_table = _similarity_table;
	read_clone_labels();
	evaluate(MINIMUM_SIMILARITY_TEMP);
}
