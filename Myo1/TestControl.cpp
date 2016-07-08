
#ifndef  TESTCONTROL
#define TESTCONTROL

#include "DTW.cpp"
#include "GestureDataManager.cpp"

const int THRESHOLDOFREALGESTURETEST = 10;

class TestControl {
public:

	TestControl() {
		DTW *dtwAlg = new DTW();
	}

	~TestControl() {
		delete dtwAlg;
	}

	vector<int> changeTypeIntArrayToVector(int *initArray, int lengthArray) {
		vector<int> *tempVector = new vector<int>();

		for (int i = 0; i < lengthArray; i++) {
			tempVector->push_back(initArray[i]);
		}

		return *tempVector;
	}

	bool testDTWAlg() {
		vector<int> examplarTestVector = changeTypeIntArrayToVector(examplarTest, 10);
		vector<int> currentAcclTestVector = changeTypeIntArrayToVector(currentAcclTest, 7);
		//int result = dtwAlg->calDTW(examplarTestVector, currentAcclTestVector);

		//return result == resultDTW;
		return false;
	}

	void testAllCode() {
		gestureData = new GestureDataManager(2);

		int lengthOfTest = gestureData->testGestures.size();
		for (int i = 0;i < lengthOfTest; i++) {
			int realType = gestureData->indexOfTestGestures[i];
			int resultType = getTypeOfGestures(gestureData->testGestures[i]);
			numOfResultType[realType][resultType]++;
		}

		for (int i = 1; i < 6; i++) {
			for (int j = 0; j < 5; j++) {
				cout << numOfResultType[i][j] << " ";
			}
			cout << endl;
		}

		delete gestureData;
	}


private:
	int numOfResultType[6][6] = {};
	int examplarTest[10] = { 0, 1, 3, 1, 5, -1, -15, -2, -1, 0 };
	int currentAcclTest[8] = { 1, 5, 4, -2, -13, -4, 0 };
	const int resultDTW = 7;
	GestureDataManager *gestureData;
	DTW *dtwAlg;

	int getTypeOfGestures(Gesture* testGesture) {
		int type = 0;
		int min = getDTWScore(gestureData->examplars[0], testGesture);
		int dtwScore = 0;
		for (int i = 1;i < 25;i++) {
			dtwScore = getDTWScore(gestureData->examplars[i], testGesture);
			if (min > dtwScore) {
				min = dtwScore;
				type = gestureData->indexOfExamplars[i];
			}
		}
		if (min > THRESHOLDOFREALGESTURETEST) {
			return type + 1;
		}
		else {
			return 0;
		}
	}

	int getDTWScore(Gesture* examplar, Gesture* testGesture) {
		return dtwAlg->calDTW(examplar,testGesture);
	}
};


#endif // ! TESTCONTROL