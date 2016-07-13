#ifndef GESTUREMANAGER
#define GESTUREMANAGER

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Gesture.cpp"
#include "DTW.cpp"
using namespace std;

const string EXAMPLARSFILE = "examplars.txt";
const string EXAMPLARSINDEX = "examplarsIndex.txt";
const string MEANEXAMPLAR = "meanExampla.txt";
const string GESTUREFILE = "gestureJab.txt";
const string GESTUREINDEXFILE = "gestureIndex.txt";
const int NUMOFTRAININGSEQ = 25;

class GestureDataManager {
public :

	vector<Gesture*> examplars;
	vector<int> indexOfExamplars;
	vector<Gesture*> testGestures;
	vector<int> indexOfTestGestures;
	bool mode = 0; // 0 train, 1 load

	vector <Gesture*> meanExamplar;

	GestureDataManager(int type = 0) {
		cout << "type : " << type << endl;
		if (type == 1) {
			mode = type;
			loadExamplars();
		}
		else if (type == 2) {
			mode = type;
			loadExamplars();
			loadTestGestures();
		} else if (type == 3) {
			loadMeanExamplars();
			mode = type;
		}
	}

	~GestureDataManager() {
		if (mode == 0)
			saveExamplars();
		for (int i = 0; i < 5; i++) {
			delete examplars[i];
		}
	}

	bool addExamplars(Gesture insertExamplar) {
		if (indexCurrentInsertExamplar < NUMOFTRAININGSEQ) {
			Gesture *copyExamplar = new Gesture(insertExamplar);

			cout << indexCurrentInsertExamplar << endl;
			examplars.push_back(copyExamplar);
			indexOfExamplars.push_back(0);
			if (indexCurrentInsertExamplar >= NUMOFTRAININGSEQ-1)
				saveExamplars();
			indexCurrentInsertExamplar++;
			 
			return true;
		} else {
			return false;
		}
	}

	void addTestGestures(Gesture testSeq,int typeOfSeq) {
		Gesture *copyExamplar = new Gesture(testSeq);

		cout << indexCurrentInsertGestures++;
		testGestures.push_back(copyExamplar);
		indexOfTestGestures.push_back(typeOfSeq);
	}

	void getMeanExamplar() {
		vector<Gesture*> cluster[5];
		DTW createMean;
		for (int i = 0; i < examplars.size();i++) {
			cluster[indexOfExamplars[i]].push_back(examplars[i]);
		}
		for (int i = 0; i < 5; i++) {
			meanExamplar.push_back(createMean.calMeanOfGesutures(cluster[i]));
		}
		saveMeanExamplars();
	}

private:
	int indexCurrentInsertExamplar = 0;
	int indexCurrentInsertGestures = 0;

	void loadMeanExamplars() {
		ifstream openfile(MEANEXAMPLAR);
		string line;
		int index = 0;

		if (openfile.is_open()) {
			while (!openfile.eof())
			{
				if (index > 4)
					cout << "error line 99 GestureDataManager" <<endl;
				getline(openfile, line);
				meanExamplar.push_back(fromStringToGesture(line));
				index++; // this line still here for testing
			}
			openfile.close();
		}
		else
		{
			cout << "can't not open file";
		}
	}

	void saveMeanExamplars() {
		ofstream openfile(EXAMPLARSFILE);
		if (openfile.is_open())
		{
			for (int i = 0; i < meanExamplar.size(); i++) {
				openfile << fromGestureToString(meanExamplar[i]) + "\n";
			}
			openfile << fromGestureToString(meanExamplar[meanExamplar.size() - 1]);
			openfile.close();
		}
		else cout << "Unable to open file";
	}

	void loadExamplars() {
		loadExamplarsSeq();
		loadExamplarsIndex();
		// check length 
		cout << examplars.size() << endl;
		cout << indexOfExamplars.size() << endl;
	}

	void loadExamplarsSeq() {
		ifstream openfile(EXAMPLARSFILE);
		string line;
		int index = 0;

		if (openfile.is_open()) {
			while (!openfile.eof())
			{
				getline(openfile, line);
				examplars.push_back(fromStringToGesture(line));
				index++; // this line still here for testing
			}
			openfile.close();
		}
		else
		{
			cout << "can't not open file load examplars sequence ";
		}
	}

	void loadExamplarsIndex() {
		ifstream openfile(EXAMPLARSINDEX);
		string line;
		int index = 0;

		if (openfile.is_open()) {
			while (!openfile.eof())
			{
				getline(openfile, line);
				indexOfExamplars.push_back(stoi(line));
				index++; // this line still be here for testing.
			}
			openfile.close();
		}
		else
		{
			cout << "can't not open file load examplars index";
		}
	}

	void saveExamplars() {
		saveExamplarsSeq();
		saveExamplarsIndex();
		cout << examplars.size()<< endl;
		cout << indexOfExamplars.size() << endl;
	}

	void saveExamplarsSeq() {
		ofstream openfile(EXAMPLARSFILE);
		if (openfile.is_open())
		{
			for (int i = 0; i < examplars.size()-1; i++) {
				openfile << fromGestureToString(examplars[i]) + "\n";
			}
			openfile << fromGestureToString(examplars[examplars.size()-1]) ;
			openfile.close();
		}
		else cout << "Unable to open file";
	}

	void saveExamplarsIndex() {
		ofstream openfile(EXAMPLARSINDEX);
		if (openfile.is_open())
		{
			cout <<"size index : "<< indexOfExamplars.size() << endl;
			for (int i = 0; i < indexOfExamplars.size()-1; i++)
			{
				openfile << to_string(indexOfExamplars[i]) + "\n";
			}
			openfile << to_string(indexOfExamplars[indexOfExamplars.size()-1]);
			openfile.close();
		}
		else cout << "Unable to open file";
	}

	void loadTestGestures() {
		loadTestGesturesData();
		loadTestGesturesIndex();
		// check length 
		if (testGestures.size() != indexOfTestGestures.size())
			cout << "error in line 154: save and load test gesture error";
	}

	void loadTestGesturesData() {
		ifstream openfile(GESTUREFILE);
		string line;

		if (openfile.is_open()) {
			while (!openfile.eof())
			{
				getline(openfile, line);
				testGestures.push_back(fromStringToGesture(line));
			}
			openfile.close();
		}
		else
		{
			cout << "can't not open file";
		}
	}

	void loadTestGesturesIndex() {
		ifstream openfile(GESTUREINDEXFILE);
		string line;

		if (openfile.is_open()) {
			while (!openfile.eof())
			{
				getline(openfile, line);
				indexOfTestGestures.push_back(stoi(line));
			}
			openfile.close();
		}
		else
		{
			cout << "can't not open file";
		}
	}

	void saveTestGestures() {
		saveTestGesturesData();
		saveTestGesturesIndex();
	}

	void saveTestGesturesData() {
		ofstream openfile(GESTUREFILE, std::ofstream::app);
		if (openfile.is_open())
		{
			for (int i = 0; i < testGestures.size()-1; i++)
				openfile << fromGestureToString(testGestures[i]) + "\n";
			openfile << fromGestureToString(testGestures[testGestures.size()-1]);
			openfile.close();
		}
		else cout << "Unable to open file";
	}

	void saveTestGesturesIndex() {
		ofstream openfile(GESTUREINDEXFILE, std::ofstream::app);
		if (openfile.is_open())
		{
			for (int i = 0; i < indexOfTestGestures.size()-1; i++)
				openfile << to_string(indexOfTestGestures[i]) + "\n";
			openfile << to_string(indexOfTestGestures[indexOfTestGestures.size() - 1]);
			openfile.close();
		}
		else cout << "Unable to open file";
	}

	string fromGestureToString(Gesture *vc) {
		string result = "";
		string stepString = "";
		int length = vc->sequence_x.size();

		for (int i = 0;i < length;i++) {
			stepString = to_string(vc->sequence_x[i]) + string(",") + to_string(vc->sequence_y[i]) + string(",") + to_string(vc->sequence_z[i]);
			//stepString = to_string(vc->sequence_x[i]);
			if (i == 0)
				result += stepString;
			else
				result += "," + stepString;
		}
		return result +",";
	}

	Gesture* fromStringToGesture(string gestureStr) {
		int lengthOfStr = gestureStr.length();
		int indexOfDirect = 0;
		Gesture *resultGesture = new Gesture();
		string currentNum = "";
		
		for (int i = 0; i < lengthOfStr; i++) {
			if (gestureStr[i] == ',') {

				switch (indexOfDirect)
				{
				case 0: 
					resultGesture->sequence_x.push_back(stoi(currentNum));
					break;
				case 1:
					resultGesture->sequence_y.push_back(stoi(currentNum));
					break;
				case 2:
					resultGesture->sequence_z.push_back(stoi(currentNum));
					break;
				default:
					cout << "error in func convert string to gesture";
					break;
				}
				currentNum = "";
				indexOfDirect = (indexOfDirect + 1) % 3;
			}
			else {
				currentNum = currentNum + gestureStr[i];
			}
		}

		return resultGesture;
	}
};


#endif // !GESTUREMANAGER