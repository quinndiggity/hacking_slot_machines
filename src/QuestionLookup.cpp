// code so hideous that even its accursed creator turns away in disgust

#include "QuestionLookup.hpp"

QuestionLookup::QuestionLookup(void) {
	jackpotpermutation=0;
	permutation=0;
	correctquestioninddex=0;
	normalorjackpot=0;
	index_of_best_matched_q = 0;    
	std::ifstream myfile("/slotbot/text/Q.txt");
		  
	if (myfile.is_open())
	{
		std::string line;
		   while (getline(myfile, line))
		  {
		         questiontextlist.push_back(line);  
		  }
		  myfile.close();
	}
	else std::cout << "Unable to open file";

}

// Levenshtein Distance Function (Martin Ettl) - callable from Main
size_t QuestionLookup::LevenshteinDistance(const std::string &s1 , const std::string &s2 )
{
        const size_t m( s1.size());
        const size_t n( s2.size());

        if (m == 0) return n;
        if (n == 0) return m;

        size_t *costs = new size_t[n + 1];

		
        for ( size_t k = 0; k <= n; k++) costs[k] = k;

        size_t i = 0;

        for ( std::string:: const_iterator it1 = s1 .begin(); it1 != s1.end(); ++it1, ++i)
       {
              costs[0] = i + 1;
               size_t corner = i;

               size_t j = 0;
               for ( std::string:: const_iterator it2 = s2 .begin(); it2 != s2.end(); ++it2, ++j)
              {
                      size_t upper = costs[j + 1];
                      if ( *it1 == *it2)
                     {
                           costs[j + 1] = corner;
                     }
                      else
                     {
                            size_t t(upper<corner ? upper : corner);
                           costs[j + 1] = (costs[j]<t ? costs[j] : t) + 1;
                     }

                     corner = upper;
              }
       }

        size_t result = costs[n];
        delete[] costs;

        return result;
}

// Word_distance function

std::vector <std::string > &QuestionLookup::split(const std::string &s , char delim , std::vector <std::string > &elems ) {
        std::stringstream ss(s );
        std::string item;
        while (getline(ss, item, delim)) {
               elems.push_back(item);
       }
        return elems;
}

std::vector <std::string > QuestionLookup::split(const std::string &s , char delim ) {
        std::vector <std::string > elems;
       split( s, delim, elems);
        return elems;
}

unsigned int QuestionLookup::word_distance(const std::vector <std::string >& s1, const std::vector <std::string >& s2)
{
        const size_t len1 = s1.size(), len2 = s2.size();
              d[0][0] = 0;	
        for ( unsigned int i = 1; i <= len1; ++i) d[i][0] = i;	
        for ( unsigned int i = 1; i <= len2; ++i) d[0][i] = i;		
               for ( unsigned int i = 1; i <= len1; ++i)		
               for ( unsigned int j = 1; j <= len2; ++j)
              {
                     d[i][j] = std::min(d[i - 1][j] + 1, d[i][j - 1] + 1);
                     d[i][j] = std::min(d[i][j], d[i - 1][j - 1] + ( s1[i - 1] == s2[ j - 1] ? 0 : 1));
              }
        return d[len1][len2];
}

// Matrix algebra functions called from main.
void QuestionLookup::swap(int& a, int& b)
{ int x=a;
      a=b;
      b=x;
}  

void QuestionLookup::printVector(std::vector<int>& theVector)
{ 

for (unsigned int i=0; i<theVector.size(); i++)
{
    basisgenerator[i][permutation] = theVector[i];
}
	permutation++;
	for (unsigned int x=0; x<theVector.size(); x++)
	{
	basisgenerator[x][permutation] = theVector[x];
	  }
}


void QuestionLookup::generateAllPermutations(std::vector<int>& toBePermuted, unsigned int nextIndex)
{ if (nextIndex==toBePermuted.size())
  { printVector(toBePermuted);
	return;
  }

  for (unsigned int i=nextIndex; i<toBePermuted.size(); i++)
  {

	swap(toBePermuted[i], toBePermuted[nextIndex]);
	generateAllPermutations(toBePermuted, nextIndex+1);
	swap(toBePermuted[i], toBePermuted[nextIndex]);
  }
}

void QuestionLookup::generateAllPermutations(std::vector<int>& toBePermuted)
{ generateAllPermutations(toBePermuted, 0);
}


void QuestionLookup::generatebasis(){
std::vector<int> theVector;
theVector.push_back(0);
theVector.push_back(1);
theVector.push_back(2);
theVector.push_back(3);
generateAllPermutations(theVector);

for(unsigned int i=0; i<4; ++i)
    for(unsigned int j=0; j<4; ++j)
            for(unsigned int k=0; k<24; ++k)
            {
            basis[i][j][k]=0;
            }
for(unsigned i=0;i<4;i++) {
    for(unsigned k=0;k<24;k++) {
        basis[basisgenerator[i][k]][i][k]=1;
	}
    }
}

void QuestionLookup::jackpot_generateAllPermutations(std::vector<int>& toBePermuted, unsigned int nextIndex)
{ if (nextIndex==toBePermuted.size())
  { printjackpot_Vector(toBePermuted);
    return;
  }
	
  for (unsigned int i=nextIndex; i<toBePermuted.size(); i++)
  {
    swap(toBePermuted[i], toBePermuted[nextIndex]);
    jackpot_generateAllPermutations(toBePermuted, nextIndex+1);
    swap(toBePermuted[i], toBePermuted[nextIndex]);
  }
}

void QuestionLookup::jackpot_generateAllPermutations(std::vector<int>& toBePermuted)
{ jackpot_generateAllPermutations(toBePermuted, 0);
}
// Generate all permutation matrices needed
void QuestionLookup::generatejackpot_basis(){
std::vector<int> thejackpot_Vector;
thejackpot_Vector.push_back(0);
thejackpot_Vector.push_back(1);
thejackpot_Vector.push_back(2);
thejackpot_Vector.push_back(3);
thejackpot_Vector.push_back(4);
jackpot_generateAllPermutations(thejackpot_Vector);

for(unsigned int i=0; i<5; ++i)
    for(unsigned int j=0; j<5; ++j)	
            for(unsigned int k=0; k<120; ++k)
            {
            jackpot_basis[i][j][k]=0;
            }

for(unsigned i=0;i<5;i++) {
    for(unsigned k=0;k<120;k++) {
        jackpot_basis[jackpot_basisgenerator[i][k]][i][k]=1;
    }
}
}


// Void to output results
void QuestionLookup::output(){
std::cout << std::endl << "*****************************************************" << std::endl;
std::cout << std::endl << "Identifying..." << std::endl;
std::cout << std::endl << "*****************************************************" << std::endl;
std::cout << std::endl << "Q : "<< truequestion << std::endl;
std::cout << std::endl << "A1: "<< a1 << std::endl;
std::cout << std::endl << "A2: "<< a2 << std::endl;
std::cout << std::endl << "A3: "<< a3 << std::endl;
std::cout << std::endl << "A4: "<< a4 << std::endl;
std::cout << std::endl << "*****************************************************" << std::endl;
std::cout << std::endl << "Correct answer: " << "A" << correctanswer <<  std::endl;
std::cout << std::endl << "Question confidence: " << questionconfidence  << " %"<<std::endl;
std::cout << std::endl << "Answer confidence (given prior): " << answerconfidence << " %"<<std::endl;
if (normalorjackpot==0){std::cout << std::endl << "Question is from normal set"<<std::endl;}
else{std::cout << std::endl << "Question is from jackpot set"<<std::endl;}
std::cout << std::endl << "Found question at line " << index_of_best_matched_q +1 <<std::endl;
std::cout << std::endl << "*****************************************************" << std::endl;
}


std::string QuestionLookup::getAnswer(std::string ocrquestion, std::string Ans1, std::string Ans2, std::string Ans3, std::string Ans4)
{

/* We need to build the basis sets for answer matching. This
   only needs to be done once. */
generatebasis();
generatejackpot_basis();

std::string ocranswer[4];            // Tesseract outputs answer data Ai. We call these ocranswer[i]
ocranswer[0] = Ans1;
ocranswer[1] = Ans2;
ocranswer[2] = Ans3;
ocranswer[3] = Ans4;

int numnormal = 13310;               // number of questions in jamiepot (normal game)
int numjack = 13310;                 // number of questions in jackpot
std::string questionvec[13310];      // vector where each entry is question string from file
int distancevec[13310];              // vector where each entry is distance between ocrquestion and each truequestion

/* Now we read truequestions from file and populate vectors with data   */
std::string line;

/* We now generate a matrix where each element is the fractional
   match of a specific ocranswer against a specific trueanswer */
float fractional_match_matrix[4][4][multiple_match_vec.size()];

/* Algorithm for fractional match */	
for(unsigned int q=0;q<multiple_match_vec.size();q++) {
	for(unsigned int i=0;i<4;i++) {	
		for(unsigned int j=0;j<4;j++) {
		fractional_match_matrix[i][j][q] = 1 - float(LevenshteinDistance(ocranswer[i], trueanswer[j][q]) / std::max( float(trueanswer[j][q].length() ), float(ocranswer[i].length() )  ) );                //we create a measure of matching
		}
	}
}

/* We now multiply each fractional match matrix successively by each
   elementary matrix, and take the trace of the resulting matrix. This
   is a measure of conformity of the ocranswers to a specific permutation
   of the trueanswers */
float projectionmatrix[4][4][multiple_match_vec.size()];             // build projectionmatrix
float tracevector[24][multiple_match_vec.size()];                    // build vector of values for trace

for(int i=0; i<24; ++i) {
	for(int j=0; j<multiple_match_vec.size(); ++j) {
		tracevector[i][j] = 0;
		}
}


/*  We store the identified questions and answers for output */
truequestion = questionvec[index_of_best_matched_q];
	
for(unsigned i=0;i<5;i++) {
	if (jackpot_basis[0][i][biggestzref]==1){
		a1=jackpottrueanswer[biggestmref+i][biggestpref];
	}
		if (jackpot_basis[1][i][biggestzref]==1){
		a2=jackpottrueanswer[biggestmref+i][biggestpref];
	}
		if (jackpot_basis[2][i][biggestzref]==1){
		a3=jackpottrueanswer[biggestmref+i][biggestpref];
	}
		if (jackpot_basis[3][i][biggestzref]==1){
		a4=jackpottrueanswer[biggestmref+i][biggestpref];
	}
}

/*  Output results */
	
std::string coranstext;
std::string answerletter;
if (correctanswer==1) {coranstext = a1; answerletter = "A - ";}
else if (correctanswer==2) {coranstext = a2; answerletter = "B - ";}
else if (correctanswer==3) {coranstext = a3; answerletter = "C - ";} 
else if (correctanswer==4) {coranstext = a4; answerletter = "D - ";} 

std::vector<float> outputvector;
outputvector.push_back((float)correctanswer);
outputvector.push_back(questionconfidence);
outputvector.push_back(answerconfidence);

std::string outputstring = answerletter + coranstext;

return outputstring;

}
