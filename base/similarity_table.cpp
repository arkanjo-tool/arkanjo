#include "similarity_table.hpp"

int Similarity_Table::find_id_path(Path path){
	if(path_id.find(path) == path_id.end()){
		path_id[path] = paths.size();
		paths.push_back(path);
		vector<pair<int,double>> empty_vec;
		similarity_graph.push_back(empty_vec);
	}
	return path_id[path];
}

void Similarity_Table::read_comparation(ifstream &table_file){
	string string_path1, string_path2;
	double similarity;
	table_file >> string_path1 >> string_path2 >> similarity;

	int id1 = find_id_path(Path(string_path1));
	int id2 = find_id_path(Path(string_path2));

	if(id1 > id2){
		swap(id1,id2);
	}

	similarity_graph[id1].push_back(make_pair(id2,similarity));
	similarity_graph[id2].push_back(make_pair(id1,similarity));
	similarity_table[make_pair(id1,id2)] = similarity;
}

void Similarity_Table::read_file_table(ifstream &table_file){
	int number_comparations;
	table_file >> number_comparations;
	for(int i = 0; i < number_comparations; i++){
		read_comparation(table_file);
	}
}

void Similarity_Table::init_similarity_table(){
	std::ifstream table_file;
	table_file.open(SIMILARITY_TABLE_FILE_NAME);
	Utils::ensure_file_is_open(table_file,SIMILARITY_TABLE_FILE_NAME);

	read_file_table(table_file);

	table_file.close();
}

Similarity_Table::Similarity_Table(double _similarity_threshold){
	similarity_threshold= _similarity_threshold;
	init_similarity_table();
}

Similarity_Table::Similarity_Table(){
	similarity_threshold = DEFAULT_SIMILARITY;
	init_similarity_table();
}

void Similarity_Table::update_similarity(double new_similarity_threshold){
	similarity_threshold = new_similarity_threshold;
}

double Similarity_Table::get_similarity(Path path1, Path path2){
	int id1 = find_id_path(path1);
	int id2 = find_id_path(path2);

	if(id1 == id2){
		return MAXIMUM_SIMILARITY;
	}
	if(id1 > id2){
		swap(id1,id2);
	}
	pair<int,int> aux = make_pair(id1,id2);
	if(similarity_table.find(aux) != similarity_table.end()){
		return similarity_table[aux];
	}
	return MINIMUM_SIMILARITY;
}

bool Similarity_Table::is_above_threshold(double similarity){
	return similarity_threshold <= similarity + EPS_ERROR_MARGIN;
}

double Similarity_Table::is_similar(Path path1, Path path2){
	double similarity = get_similarity(path1, path2);
	return is_above_threshold(similarity);
}

vector<Path> Similarity_Table::get_path_list(){
	return paths;
}

vector<Path> Similarity_Table::get_similar_path_to_the_reference(Path reference){
	int id = find_id_path(reference);
	vector<Path> ret;
	for(auto [id,similarity] : similarity_graph[id]){
		if(is_above_threshold(similarity)){
			ret.push_back(paths[id]);
		}
	}
	return ret;
}

vector<tuple<double,Path,Path>> Similarity_Table::get_all_path_pairs_and_similarity_sorted_by_similarity(){
	vector<tuple<double,Path,Path>> similar_path_pairs;
	for(auto [ ids, similarity] : similarity_table){
		Path path1 = paths[ids.first];
		Path path2 = paths[ids.second];
		if(is_similar(path1,path2)){
			similar_path_pairs.push_back({similarity,path1,path2});
		}
	}
	sort(similar_path_pairs.rbegin(),similar_path_pairs.rend());
	return similar_path_pairs;
}

vector<pair<Path,Path>> Similarity_Table::get_all_similar_path_pairs_sorted_by_similarity(){
	auto similar_path_pairs = get_all_path_pairs_and_similarity_sorted_by_similarity();
	vector<pair<Path,Path>> ret;
	for(auto [similarity,path1,path2] : similar_path_pairs){
		ret.push_back({path1,path2});
	}
	return ret;
}

vector<tuple<int,Path,Path>> Similarity_Table::sort_pairs_by_line_number(vector<pair<Path,Path>> similar_path_pairs){
	vector<tuple<int,Path,Path>> similar_path_pairs_with_number_of_lines;
	for(auto [path1,path2] : similar_path_pairs){
		Function function(path1);
		tuple<int,Path,Path> aux = {function.number_of_lines(),path1,path2};
		similar_path_pairs_with_number_of_lines.push_back(aux);
	}
	sort(
			similar_path_pairs_with_number_of_lines.begin(),
			similar_path_pairs_with_number_of_lines.end(),
			[&](tuple<int,Path,Path> pair1, tuple<int,Path,Path> pair2){
				int number_lines1 = get<0>(pair1);
				int number_lines2 = get<0>(pair2);
				return number_lines1 > number_lines2;
			}			
		);
	return similar_path_pairs_with_number_of_lines;
}

vector<pair<Path,Path>> Similarity_Table::get_all_similar_path_pairs_sorted_by_line_number(){
	vector<pair<Path,Path>> similar_path_pairs = get_all_similar_path_pairs_sorted_by_similarity();

	vector<tuple<int,Path,Path>> similar_path_pairs_with_number_of_lines = 
		sort_pairs_by_line_number(similar_path_pairs);
	
	vector<pair<Path,Path>> ret;
	for(auto [line_number,path1,path2] : similar_path_pairs_with_number_of_lines){
		ret.push_back({path1,path2});
	}

	return ret;
}
