#ifndef GESTUREMANAGER
#define GESTUREMANAGER

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Gesture.cpp"
using namespace std;

const string EXAMPLARSFILE = "examplars.txt";
const string GESTUREFILE = "gestureJab.txt";
const string GESTUREINDEXFILE = "gestureIndex.txt";

class GestureDataManager {
public :

	Gesture *examplars[5];
	vector<Gesture*> testGestures;
	vector<int> indexOfTestGestures;
	bool mode = 0; // 0 train, 1 load

	GestureDataManager(int type = 0) {
		if (type == 1) {
			mode = type;
			loadExamplars();
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
		if (indexCurrentInsertExamplar < 5) {
			Gesture *copyExamplar = new Gesture(insertExamplar);

			cout << indexCurrentInsertExamplar << endl;
			examplars[indexCurrentInsertExamplar] = copyExamplar;
			if (indexCurrentInsertExamplar == 4)
				saveExamplars();
			indexCurrentInsertExamplar++;
			return true;
		} else {
			return false;
		}
	}

	void addTestGestures(Gesture testSeq,int typeOfSeq) {
			Gesture *copyExamplar = new Gesture(testSeq);
			testGestures.push_back(copyExamplar);
			indexOfTestGestures.push_back(typeOfSeq);
	}

	void loadExamplars() {
		ifstream openfile(EXAMPLARSFILE);
		string line;
		int index = 0;

		if (openfile.is_open()) {
			while (!openfile.eof())
			{
				if (index >= 5) {
					break;
				}
				getline(openfile, line);
				examplars[index] = fromStringToGesture(line);
				index++;
			}
			openfile.close();
		}
		else
		{
			cout << "can't not open file";
		}
	}

	void loadTestGestures() {
		loadTestGesturesData();
		loadTestGesturesIndex();
	}

	void saveExamplars() {
		ofstream openfile(EXAMPLARSFILE);
		if (openfile.is_open())
		{
			for (int i = 0;i < 5;i++) {
				openfile << fromGestureToString(examplars[i]) + "\n";
			}
			openfile.close();
		}
		else cout << "Unable to open file";
	}

	void saveTestGestures() {
		saveTestGesturesData();
		saveTestGesturesIndex();
	}

private:
	int indexCurrentInsertExamplar = 0;


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

	void saveTestGesturesData() {
		ofstream openfile(GESTUREFILE, std::fstream::in | std::fstream::out | std::fstream::app);
		if (openfile.is_open())
		{
			for (int i = 0; i < testGestures.size(); i++)
				openfile << fromGestureToString(testGestures[i]) + "\n";
				//openfile << "test \n";
			openfile.close();
		}
		else cout << "Unable to open file";
	}

	void saveTestGesturesIndex() {
		ofstream openfile(GESTUREINDEXFILE, std::fstream::in | std::fstream::out | std::fstream::app);
		if (openfile.is_open())
		{
			for (int i = 0; i < indexOfTestGestures.size(); i++)
				openfile << indexOfTestGestures[i] + "\n";
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