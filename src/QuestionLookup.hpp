
#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include<cmath>


class QuestionLookup {

	public:
	QuestionLookup(void);
	size_t LevenshteinDistance(const std::string & , const std::string & );
	std::vector <std::string > &split(const std::string & , char , std::vector <std::string > & );
	std::vector <std::string > split(const std::string & , char  );
	unsigned int word_distance(const std::vector <std::string >&, const std::vector <std::string >&);
	void swap(int&, int&);
	void printVector(std::vector<int>&);
	void generateAllPermutations(std::vector<int>&, unsigned int);
	void generateAllPermutations(std::vector<int>&);
	void generatebasis();
	void printjackpot_Vector(std::vector<int>&);
	void jackpot_generateAllPermutations(std::vector<int>&, unsigned int);
	void jackpot_generateAllPermutations(std::vector<int>&);
	void generatejackpot_basis();
	void output();
	std::string getAnswer(std::string, std::string, std::string, std::string, std::string);

	int d[30][30]; //sentence length array - we should not encounter a sentence with > 30 words.
	int basisgenerator[4][24];
	int permutation;
	int basis[4][4][24];
	int jackpot_basisgenerator[5][120];
	int jackpotpermutation;
	int jackpot_basis[5][5][120];
	float questionconfidence;
	float answerconfidence;
	std::string q;
	std::string a1;
	std::string a2;
	std::string a3;
	std::string a4;
	int correctanswer;
	int correctquestioninddex;
	std::string truequestion;
	int normalorjackpot;
	float normal_fractional_match_matrix[4][4];
	float normal_matched_basis_matrix[4][4];
	float jackpot_fractional_match_matrix_store[4][5];
	float jackpot_matched_basis_matrix[4][5];
	int index_of_best_matched_q;                                                         

	std::vector <std::string> questiontextlist;  

};
