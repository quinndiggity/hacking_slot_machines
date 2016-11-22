#include "QuestionLookup.hpp"

QuestionLookup::QuestionLookup(void) {
	jackpotpermutation=0;
	permutation=0;
	correctquestioninddex=0;
	normalorjackpot=0;
	index_of_best_matched_q = 0;    


	std::ifstream myfile("/quizviz/text/Q.txt");
		  
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

// Levenshtein Distance Function - callable from Main
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

// Word_distance function (Levenshtein Distance for words) - callable from Main. This should not be used unless question processing time is an issue.

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
// End of word_distance functions



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

/* Without the next paragraph, the first basis matrix comes out wrong. I have no idea why this is! */
basisgenerator[0][0] = 0;
basisgenerator[1][0] = 1;
basisgenerator[2][0] = 2;
basisgenerator[3][0] = 3;

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



void QuestionLookup::printjackpot_Vector(std::vector<int>& thejackpot_Vector)
{ 

for (unsigned int i=0; i<thejackpot_Vector.size(); i++)
{
          jackpot_basisgenerator[i][jackpotpermutation] = thejackpot_Vector[i];

}

     jackpotpermutation++;
    
	for (unsigned int x=0; x<thejackpot_Vector.size(); x++)
  {
  jackpot_basisgenerator[x][jackpotpermutation] = thejackpot_Vector[x];
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



void QuestionLookup::generatejackpot_basis(){
std::vector<int> thejackpot_Vector;
  thejackpot_Vector.push_back(0);
  thejackpot_Vector.push_back(1);
  thejackpot_Vector.push_back(2);
  thejackpot_Vector.push_back(3);
    thejackpot_Vector.push_back(4);
  jackpot_generateAllPermutations(thejackpot_Vector);

/* Without the next paragraph, the first jackpot_basis matrix comes out wrong. I have no idea why this is! */
jackpot_basisgenerator[0][0] = 0;
jackpot_basisgenerator[1][0] = 1;
jackpot_basisgenerator[2][0] = 2;
jackpot_basisgenerator[3][0] = 3;
jackpot_basisgenerator[4][0] = 4;

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

// End of matrix algebra functions.


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

/*
if (normalorjackpot==0){




        std::cout << std::endl << "Fractional match matrix:"  << std::endl<< std::endl;
      for(unsigned i=0;i<4;i++) {
              for(unsigned j=0;j<4;j++) {
                    std::cout << normal_fractional_match_matrix[i][j] << " ";
              }
                      std::cout << std::endl ;
      }


    std::cout << std::endl << "Matched basis matrix:" << std::endl<< std::endl;
      for(unsigned i=0;i<4;i++) {
              for(unsigned j=0;j<4;j++) {
                    std::cout << normal_matched_basis_matrix[i][j] << " ";
              }
                      std::cout << std::endl ;
      }
}
else{


    std::cout << std::endl << "Matched basis matrix:" << std::endl<< std::endl;


      for(unsigned i=0;i<4;i++) {
              for(unsigned j=0;j<5;j++) {
                    std::cout << jackpot_matched_basis_matrix[i][j] << " ";
              }
                      std::cout << std::endl ;
      }



}

std::cout << std::endl << "*****************************************************" << std::endl;

std::cout << std::endl << "Diagnostic information:" << std::endl;

std::cout << std::endl << "*****************************************************" << std::endl;




 std::cout << std::endl << "Sample basis matrix (#22):"  << std::endl<< std::endl;

        for(unsigned i=0;i<4;i++) {
              for(unsigned j=0;j<4;j++) {
                    std::cout << basis[i][j][22] << " ";
              }
                      std::cout << std::endl ;
      }
        std::cout << std::endl << "From generator (#22):"  << std::endl<< std::endl;
    for(unsigned i=0;i<4;i++) {
 std::cout << basisgenerator[i][22]<< " ";
    }
    std::cout << std::endl;

             std::cout << std::endl << "Sample jackpot basis matrix (#70):"  << std::endl<< std::endl;

        for(unsigned i=0;i<4;i++) {
              for(unsigned j=0;j<5;j++) {
                    std::cout << jackpot_basis[i][j][70] << " ";
              }
                      std::cout << std::endl ;
      }
        std::cout << std::endl << "From jackpot generator (#70):"  << std::endl<< std::endl;
    for(unsigned i=0;i<5;i++) {
 std::cout << jackpot_basisgenerator[i][70]<< " ";
    }
std::cout << std::endl;



std::cout << std::endl << "*****************************************************" << std::endl;
*/

// If we want to discard a bad reading, do it here.


}


std::string QuestionLookup::getAnswer(std::string ocrquestion, std::string Ans1, std::string Ans2, std::string Ans3, std::string Ans4)
{



// We need to build the basis sets for answer matching. This only needs to be done once.
generatebasis();
generatejackpot_basis();

std::string ocranswer[4];                                                                        // Tesseract outputs answer data Ai. We call these ocranswer[i]
    ocranswer[0] = Ans1;
    ocranswer[1] = Ans2;
    ocranswer[2] = Ans3;
    ocranswer[3] = Ans4;

int numnormal = 13310;                                                                      // number of questions in jamiepot (normal game)
int numjack = 13310;                                                                        // number of questions in jackpot
std::string questionvec[13310];                                                                  // vector where each entry is question string from file
int distancevec[13310];                                                                     // vector where each entry is distance between ocrquestion and each truequestion



/* Split strings into vectors of words. This is only needed if using the word_distance metric (only used if we have set dictionary to only contain quiz words, and processing time is an issue) */

//Sentence sen1 = split(ocrquestion, ' ');
//Sentence sen2;




/* Now we read truequestions from file and populate vectors with data   */
std::string line;

for (int i=0;i<13310;++i)
{
     std::string s2 = questiontextlist[i];
//           sen2 = split(s2, ' ');                                                        // only enable if using word distance
     distancevec[i] = LevenshteinDistance(ocrquestion, s2);                    // Here is where you set the distance calculator.
     questionvec[i] = s2;
}

/* Find smallest element in discancevec - this corresponds to the likely truequestion  */

int smallestdistance = distancevec[0];                                                     // Distance to matched question (Column of distancevec corresponding to matched question)

for (unsigned int i = 0; i < numnormal; i++) {
       if (distancevec[i] < smallestdistance) {
             smallestdistance = distancevec[i];
             index_of_best_matched_q = i;
      }
}

std::vector<int> multiple_match_vec;



/* Here is where we differentiate between normal and jackpot questions: index_of_best_matched_q represents the indwex number of the best matched question  */

if (index_of_best_matched_q<=13021){                                                      // Best matched question is a normal question
	
    for (unsigned int q = 0; q < numnormal; q++) {                                        // Give multiple_match_vec an element for each similar question
        if (distancevec[q] == smallestdistance) {
            multiple_match_vec.push_back (q);
        }
    }

    int answerdistance[4][multiple_match_vec.size()];                                     // Levenshtein distance from ocrquestion to each truequestion
    std::string trueanswer[4][multiple_match_vec.size()];                                      // Give multiple_match_vec an element for each similar question



    /* Having found the truequestion, we can retrieve the trueanswers for comparison against the ocranswers */

    std::ifstream myfile2( "/quizviz/text/Q&A.txt");
	
    for (int lineno = 0; getline (myfile2,line) && lineno < 1000000; lineno++){
		
        for (unsigned int q = 0; q < multiple_match_vec.size(); q++) {
                if (lineno == multiple_match_vec[q]*5 + 1)
                  trueanswer[0][q]=line;
                if (lineno == multiple_match_vec[q]*5 + 2)
                  trueanswer[1][q]=line;
                 if (lineno == multiple_match_vec[q]*5 + 3)
                  trueanswer[2][q]=line;
                 if (lineno == multiple_match_vec[q]*5 + 4)
                  trueanswer[3][q]=line;
        }
    }



    /* We now generate a matrix where each element is the fractional match of a specific ocranswer against a specific trueanswer */

    float fractional_match_matrix[4][4][multiple_match_vec.size()];



    /* Algorithm for fractional match */
	
    for(unsigned int q=0;q<multiple_match_vec.size();q++) {
		
        for(unsigned int i=0;i<4;i++) {
			
            for(unsigned int j=0;j<4;j++) {
                fractional_match_matrix[i][j][q] = 1 - float(LevenshteinDistance(ocranswer[i], trueanswer[j][q]) / std::max( float(trueanswer[j][q].length() ), float(ocranswer[i].length() )  ) );                //we create a measure of matching
            }
        }
    }



    /* We now multiply each fractional match matrix successively by each elementary matrix, and take the trace of the resulting matrix. This is a measure of conformity of the ocranswers to a specific permutation of the trueanswers*/

    float projectionmatrix[4][4][multiple_match_vec.size()];             // build projectionmatrix
    float tracevector[24][multiple_match_vec.size()];                    // build vector of values for trace
	
	for(int i=0; i<24; ++i) {
			
			for(int j=0; j<multiple_match_vec.size(); ++j) {
				tracevector[i][j] = 0;
			}
		}

	
    for(unsigned int q=0;q<multiple_match_vec.size();q++) {
		 
         for(unsigned int z=0; z<24; ++z){                               // set projectionmatrix to 0 initially
		  
          for(unsigned i=0;i<4;i++) {
			
            for(unsigned j=0;j<4;j++) {
                projectionmatrix[i][j][q]=0;
                }
            }
			
            for(unsigned int i=0; i<4; ++i)
			
            for(unsigned int j=0; j<4; ++j)
			
            for(unsigned int k=0; k<4; ++k)
            {
                projectionmatrix[i][j][q]+=fractional_match_matrix[i][k][q]*basis[j][k][z];
            }
			
            for (unsigned int i=0; i<4; ++i){
                 tracevector[z][q]+=projectionmatrix[i][i][q];
            }
         }
    }



    /* We can now scan through all the traces to find the best match. */

    float biggesttrace=0;                                                 // stores the index value of the biggest value in the tracevector
    int biggesttraceref=0;                                                // stores the value of the biggest element of the tracevector
    int biggestqref=0;                                                    // references the index number of the corresponding question
	
    for(unsigned int q=0;q<multiple_match_vec.size();q++) {               // set values of these variables above
		
        for (unsigned int i = 0; i < 24; i++) {
            if (tracevector[i][q] > biggesttrace) {
                biggesttrace = tracevector[i][q];
                biggesttraceref = i;
                biggestqref = q;
            }
        }
    }



    /* Now we can store the correct answers for processsing later */
	
    for(unsigned i=0;i<4;i++) {
        if (basis[0][i][biggesttraceref]==1){
    a1=trueanswer[i][biggestqref];
        }
            if (basis[1][i][biggesttraceref]==1){
    a2=trueanswer[i][biggestqref];
        }
            if (basis[2][i][biggesttraceref]==1){
    a3=trueanswer[i][biggestqref];
        }
            if (basis[3][i][biggesttraceref]==1){
    a4=trueanswer[i][biggestqref];
        }
    }
		
        for(unsigned i=0;i<4;i++) {
        if (basis[i][0][biggesttraceref]==1){
            correctanswer =  i+1;
        }
    }

    truequestion = questionvec[index_of_best_matched_q];
    questionconfidence = 100*(1 - (float)distancevec[index_of_best_matched_q ]/ (float)std::max(ocrquestion.length() , questionvec[index_of_best_matched_q].length()));
    answerconfidence = 100* biggesttrace/4;
    normalorjackpot=0;

		
      for(unsigned i=0;i<4;i++) {
			
              for(unsigned j=0;j<4;j++) {
                    normal_matched_basis_matrix[i][j] = basis[i][j][biggesttraceref];
                     normal_fractional_match_matrix[i][j] = fractional_match_matrix[i][j][biggestqref];
              }
      }
}
else{



/* Everything beyond here refers to jackpot questions */

    int startrecording=0;
    int jackpotcounter=0;
    int qnumber=0;
    std::string jackpottrueanswer[50][4];
    int noofanswers[4];
    noofanswers[0]=0;
    noofanswers[1]=0;
    noofanswers[2]=0;
    noofanswers[3]=0;
    int noofquestions =0;



    /* Having found the truequestion, we can retrieve the trueanswers for comparison against the ocranswers */

    std::ifstream myfile4( "/quizviz/text/Q&A.txt");
	
    for (int lineno = 65110; getline (myfile4,line) && lineno < 1000000; lineno++){


    if (startrecording==1){
       if (jackpotcounter>0){
        jackpottrueanswer[jackpotcounter-1][qnumber-1]=line;
                    noofanswers[qnumber-1]=jackpotcounter - 1;

        jackpotcounter++;

    }
    }
                if (line==questionvec[index_of_best_matched_q]){
                    startrecording = 1;
                    jackpotcounter++;
                    qnumber++;
                    noofquestions++;
                }

                if (line == "#"){
                    startrecording=0;

                    jackpotcounter = 0;
                }
    }




    /* We now have a list of possible answers for each jackpot question. We need to project each permutation of 4 possible answers onto the ocranswers and find the best match */

    float jackpot_fractional_match_matrix[4][5];
    float jackpottracevector;
    float jackpottrace[50][4][120];
		
         for(unsigned int z=0; z<120; ++z){           // set jackpottrace to 0 initially
		
          for(unsigned m=0;m<50;m++) {

            for(unsigned p=0;p<4;p++) {
                jackpottrace[m][p][z]=0;
                }
            }
         }
    std::string jackpotanswervec[5];

    for (unsigned int p = 0; p< noofquestions; p++) {

    for (unsigned int m = 0; m< noofanswers[p]-4; m++) {

    for (unsigned int t = m; t< m+5; t++) {
    jackpotanswervec[t-m] = jackpottrueanswer[t][p];
    }

       for(unsigned int i=0;i<4;i++) {

            for(unsigned int j=0;j<5;j++) {
                jackpot_fractional_match_matrix[i][j] = 1 - float(LevenshteinDistance(ocranswer[i], jackpotanswervec[j]) /( float(jackpotanswervec[j].length()) + float(ocranswer[i].length())  ))   ;                //we create a measure of matching
            }
        }
    float jackpotprojectionmatrix[4][4];                // build jackpotprojectionmatrix
    float jackpot_trace;

        for(unsigned int z=0; z<120; ++z){              // set jackpotprojectionmatrix to 0 initially

          for(unsigned i=0;i<4;i++) {

            for(unsigned j=0;j<4;j++) {
                jackpotprojectionmatrix[i][j]=0;
                }
            }

            for(unsigned int i=0; i<4; ++i)

            for(unsigned int j=0; j<4; ++j)

            for(unsigned int k=0; k<5; ++k)
            {
                jackpotprojectionmatrix[i][j]+=jackpot_fractional_match_matrix[i][k]*jackpot_basis[j][k][z];
            }

            for (unsigned int i=0; i<4; ++i){
                    jackpot_trace+= jackpotprojectionmatrix[i][i];  // we take the trace of the projection of the answer matrix against each possible basis. This is a measure of fit.
            }
                 jackpottrace[m][p][z] = jackpot_trace;
            jackpot_trace=0;
         }
    }
    }



/* We can now scan through all the traces to find the best match. */

    float biggestjackpot_trace=0;            // this stores the value of the biggest element of the jackpottracevector
    int biggestmref=0;                       // this stores the coordinate of the answer translation (we pick 5 out of n answers - this stores which 5 we have picked)
    int biggestpref=0;;                      // this stores the coordinate of the question with the best matching answer from the generated group of similar questions (e.g. What is the correct spelling?)
    int biggestzref=0;                       // this stores the coordinate of permutation of the answers (how we have scrambled them)

         for(unsigned int z=0; z<120; ++z){

          for(unsigned m=0;m<50;m++) {

            for(unsigned p=0;p<4;p++) {
              if (jackpottrace[m][p][z] > biggestjackpot_trace) {
                biggestjackpot_trace = jackpottrace[m][p][z];
                biggestmref=m;               // this stores the value of the biggest element of the jackpottracevector
                biggestpref=p;
                biggestzref=z;
            }
                }
            }
         }



/*  We store the idfentified questions and answers for output */

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

    for(unsigned i=0;i<4;i++) {
        if (jackpot_basis[i][0][biggestzref]==1){
            correctanswer =  i+1;
        }
    }

      for(unsigned i=0;i<4;i++) {

              for(unsigned j=0;j<5;j++) {
                    jackpot_matched_basis_matrix[i][j] = jackpot_basis[i][j][biggestzref];
              }
      }

/*  We generate confidences for our answers */

    questionconfidence = 100*(1 - (float)((float)distancevec[index_of_best_matched_q]/ (float)std::max(ocrquestion.length(), questionvec[index_of_best_matched_q].length())));
    answerconfidence = 100* biggestjackpot_trace/4;
    normalorjackpot=1;

}

//output();                   // output results

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
