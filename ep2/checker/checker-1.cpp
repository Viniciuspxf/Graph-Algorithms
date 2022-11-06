#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

using namespace std;

const size_t TEST_CASES = 12;

int parse(string s){
	stringstream ss (s);
	string k;
	while(getline(ss, k, ' '));
	return stoi(k);
}

int main(){
	size_t correct = 0;
  	for (int testcase = 1; testcase <= TEST_CASES; ++testcase) {
    	stringstream ss;
    	ss << std::setfill('0') << std::setw(2) << testcase;
		string test = ss.str();

		cout << "Testcase " << test << ": ";

		string output_name = "out";
		string solution_name = "sol";
		string input_name = "in";

		output_name += test;
		solution_name += test;
		input_name += test;

		ifstream output, solution, input;

		output.open(output_name);
		solution.open(solution_name);
		input.open(input_name);

		size_t debug, n, m; input >> debug >> n >> m;
  	
	  	std::vector<int> sol_to_student;
	  	std::vector<int> student_to_sol;

	  	std::vector<int> students_all_answers;
	  	int bcc = 0;

	  	sol_to_student.resize(m + 1);
	  	student_to_sol.resize(m + 1);

	  	std::fill(sol_to_student.begin(), sol_to_student.end(), -1);
	  	std::fill(student_to_sol.begin(), student_to_sol.end(), -1);

	  	bool is_correct = true;

	  	for(int i = 0; i < m; i++){
	  		string sol_str, student_str;
	  		int sol, student;

	  		getline(output, student_str);
	  		getline(solution, sol_str);

            if(student_str != sol_str){
                is_correct = false;
                cout << "FAILED TEST - EXPETED: " << sol_str << " OUTPUT: " <<  student_str << "\n"; 
                break;
            }

  	    }
        if (is_correct) {
            cout << "CORRECT - ALL CUT VERTICES FOUND" << "\n";
            correct++;
        }
    }
        
	
	cout << correct << "/" << TEST_CASES << " Correct test cases" << endl;

	return 0;
}
