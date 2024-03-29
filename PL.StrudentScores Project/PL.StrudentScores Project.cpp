// PL.StrudentScores Project.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "string"
#include <Windows.h>
#include <sstream>
#include <vector>

using namespace std;

void createFolder() { //creates a folder
	LPCWSTR szDirPath = L"C:\\survey_calc";
	if (!CreateDirectory(szDirPath, NULL)) {
		cout << "Couldnt create file." << endl;
		cout << "This may mean you are attempting to redo the calculation, thus you may have extra data." << endl;
	}
}

void storeAvg(vector<double> arr, int classnumber, int numofstudents) { // stores the average values 
	ofstream avgfile;
	avgfile.open("C:\\survey_calc\\avgsurvey.csv", std::fstream::out | std::fstream::app);
	if (classnumber == 1) {
		avgfile << "CourseNumber," << "Number of students,";
		for (int i = 1; i <= 34; i++) {
			avgfile << "Q" << i << ",";
		}
		avgfile << "\n";
	}
	avgfile << "CourseNunber( " << classnumber << " )" << " ---- " << ",";
	avgfile << numofstudents << ",";
	for (int i = 10; i < 44; i++) {
		if (arr[i] > 0 && arr[i] < 4) {
			avgfile << arr[i] << ",";
		}
		else avgfile << " ,";
	}
	avgfile << "\n";
	avgfile.close();
}

void storeClass(vector<vector<int> > classMatrix, int classnumber) { // stores integer values of the survey answers
	ofstream classfile;
	stringstream thefile;
	int studentnumber = classMatrix.size();
	thefile << "C:\\survey_calc\\Class" << classnumber << ".csv";
	classfile.open(thefile.str(), std::fstream::app);
	classfile << "Student ,";
	for (int i = 10; i < classMatrix[0].size(); i++) {
		classfile << "Q" << i - 9 << ",";
	}
	classfile << "\n";
	for (int i = 0; i < studentnumber; i++) {
		classfile << "Student" << i + 1 << ",";
		for (int j = 10; j < classMatrix[0].size(); j++) {
			if (classMatrix[i][j] >= 0 && classMatrix[i][j] <= 4) {
				classfile << classMatrix[i][j] << ",";
			}
			else classfile << " ,";
		}
		classfile << "\n";
	}
}

void averageClass(vector<vector<int> > allstudents, int classnumber) { //finds the average of a single question
	vector<double> avgsum(allstudents[0].size());
	int numofstudents = allstudents.size();
	for (int i = 10; i < allstudents[0].size(); i++) {
		int validcount = 0;
		for (int j = 0; j < numofstudents; j++) {
			if ((allstudents[j][i]) >= 0 && allstudents[j][i] <= 4) {
				avgsum[i] += allstudents[j][i];
				validcount++;
			}
		}
		if (validcount > 0) {
			avgsum[i] = avgsum[i] / validcount;
		}
		else {
			avgsum[i] = -1;
		}
	}
	storeAvg(avgsum, classnumber, numofstudents);
}

int enumeral(char tempchar) { // turns chars to integers
	switch (tempchar) {
	case 'A':	return 4;

	case 'B':	return 3;

	case 'C':	return 2;

	case 'D':	return 1;

	case 'E':	return 0;

	case ' ':	return 5;

	default:    return 6;

	}//end of switch
}//end of enumeral function

string getFolderlocation() { // gets the location of the .txt course files
	cin.clear();
	cout << "Please enter the path to the folder that includes the .txt files\n";
	string folderlocation;
	getline(cin, folderlocation, '\n');
	return folderlocation;
} //end of getFilelocation


void classMatrix(string filename, int classnumber) { //Opens individual files and for each string's char calls the enumeral function
	vector<vector<int> > allstudents;
	ifstream input(filename);
	if (!input) {
		cout << "File is no longer there???";
		system("pause");
	}
	else {
		while (!input.eof()) {
			string student;
			getline(input, student);
			if (student.length() == 0)
				break;
			allstudents.push_back(vector<int>());
			for (int i = 0; i <= student.length() - 1; i++) {
				allstudents[allstudents.size() - 1].push_back(enumeral(student[i]));
			}
		}
		averageClass(allstudents, classnumber); //Function that looks for avgerage question answers
		storeClass(allstudents, classnumber); //stores numerated answers
	}
	input.close();
}//classMatrix end

void readFiles(string foldername) {//searches for individual course files and if they exsist calls the classMatrix function
	cin.clear();
	cout << "How many courses there are?";
	int a;
	cin >> a;
	for (int i = 1; i <= a; i++) {
		stringstream filename;
		filename << foldername << "/Course" << i << ".txt";
		ifstream input(filename.str());
		if (!input) {
			cout << "No file!";
			system("pause");
		}
		else {
			classMatrix(filename.str(), i);
		}
		input.close();
	}
}// end of readfiles

int main() //Tests if files exsist and calls the readFiles function
{
	cout << "If you have a folder prepared press Y.";
	char a = getchar();
	if (a == 'Y' || a == 'y') {
		createFolder();
		cin.ignore(); // Need to use this as the previous input is stuck on the buffer
		string folderlocation = getFolderlocation();
		readFiles(folderlocation);
	}
	else {
		cout << "Please prepare a folder with the .txt files and restart the program again." << endl;

		return 0;
	}
	cout << endl << endl <<"The folder with the data is located in C:\\survey_calc." << endl;
	cout << "Beware,Rerunning the program may corrupt the data!" << endl;
	cout << "            <<<PRESS ANY KEY TO EXIT>>>";
	cin.ignore();
	getchar();
}//end of main