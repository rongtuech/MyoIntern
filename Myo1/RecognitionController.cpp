#ifndef MODE_H
#define MODE_H
enum Mode {
	RECOGNIZEKNN,
	RECOGNIZEKC,
	TRAINING,
	GETMEANDATA,
	TEST,
	COLLECTDATA
};
#endif // !MODE_H

#ifndef RECOGNITION
#define RECOGNITION

#include <cmath>
#include <iostream>
#include <vector>
#include "Gesture.cpp"
#include "GestureDataManager.cpp"
#include "DTW.cpp"
using namespace std;

const float HIGHPASSFACTOR = 0.3;
const float GESTUREDISTANCE = 0.17f; 
const int NUMSTEP =5;
const int THRESHOLDOFREALGESTURE = 100;
const int NUMDATAFOREACHSTEP = 5;
const int NUMDATABETWEENSTEPS = 3;
const float GAPFORHIGHVALUEACC = 0.15f;
class RecognitionController {
public:	
	RecognitionController() {
		examplarsData = new GestureDataManager(0);
		dtwAlg = new DTW();
	}

	RecognitionController(Mode mode) {
		cout << "mode : " << mode << endl;
		switch (mode) {
		case Mode::TRAINING:
		case Mode::COLLECTDATA:
			examplarsData = new GestureDataManager(0);
			break;
		case Mode::RECOGNIZEKNN:
			examplarsData = new GestureDataManager(1);
			break;
		case Mode::RECOGNIZEKC:
			examplarsData = new GestureDataManager(3);
			break;
		default:
			cout << "error in recognition controller construction";
			break;
		}
		
		dtwAlg = new DTW();
		modeOfProcess = mode;
	}

	~RecognitionController() {
		delete examplarsData;
		delete dtwAlg;
	}

	void processInitAccl(float accelX, float accelY, float accelZ) { 
		//filterHighPassFactor(accelX, accelY, accelZ); don't use it may be it affect on signal
		if (isInSegment(accelX, accelY, accelZ)) {
			processSegment(accelX, accelY, accelZ);
		}
	}

private: 
	Mode modeOfProcess;
	GestureDataManager *examplarsData;
	float previousAccel[3] = {};
	int numStepNonGapGesture = NUMSTEP+1;
	float sumAccInEachStep[3] = {};
	float dataInEachStep[5][3] = {};
	int indexForDataEachStep = 0;
	bool isDataEachStepFull = false;
	int countToStep = 0;
	bool isFinishSegment = false;
	bool startIgnore = true;
	Gesture currentGesture;
	DTW *dtwAlg;
	bool isCalibrated = false;
	float accAtCalibration = 0;
	float currAcc = 0;
	bool doneReset = true;

	int quantizeAccelerometerValue(float value) {
		if (value > 2)
			return 16;
		else if (value <= 2 && value > 1)
			return int((value - 1) / 0.2f) + 1;
		else if (value <= 1 && value > 0)
			return int(value / 0.1f) + 1;
		else if (value == 0)
			return 0;
		else if (value < 0 && value >= -1)
			return int(value / 0.1f) - 1;
		else if (value >= -2 && value < -1)
			return int((value + 1) / 0.2f) - 1;
		else
			return -16;
	}

	void filterHighPassFactor(float &accelX, float &accelY, float &accelZ) {
		static float lastX = 0, lastY = 0, lastZ = 0;
		lastX = accelX*HIGHPASSFACTOR + lastX*(1.0 - HIGHPASSFACTOR);
		lastY = accelY*HIGHPASSFACTOR + lastY*(1.0 - HIGHPASSFACTOR);
		lastZ = accelZ*HIGHPASSFACTOR + lastZ*(1.0 - HIGHPASSFACTOR);

		accelX = accelX - lastX;
		accelY = accelY - lastY;
		accelZ = accelZ - lastZ;
		
	}

	float calcGapCurrentAndPreviousAccl(float accelX, float accelY, float accelZ) {
		float result = sqrtf(powf(accelX - previousAccel[0], 2) + powf(accelY - previousAccel[1], 2) + powf(accelZ - previousAccel[2], 2));
		currAcc = sqrtf(powf(accelX , 2) + powf(accelY , 2) + powf(accelZ , 2));
		previousAccel[0] = accelX;
		previousAccel[1] = accelY;
		previousAccel[2] = accelZ;
		if (!isCalibrated) {
			accAtCalibration = result;
			isCalibrated = true;
			cout << "Is calibrated" << endl;
		}
		return result;
	}

	bool isStartGesture(float accelX, float accelY, float accelZ) {
		if (calcGapCurrentAndPreviousAccl(accelX, accelY, accelZ) >= GESTUREDISTANCE) {
				return true;
		}
		else {
			
			return false;
		}
	}

	bool isEndGesture() {
		if (numStepNonGapGesture > NUMSTEP) {
			float gap = abs(accAtCalibration - currAcc);
			if (isFinishSegment &&  gap > GAPFORHIGHVALUEACC) {
				cout << gap << endl;
				return false;
			}
			else
				return true;
		}
		else {
			return false;
		}
	}

	void resetData() {
		doneReset = false;
		numStepNonGapGesture = NUMSTEP + 1;
		indexForDataEachStep = 0;
		isDataEachStepFull = false;
		countToStep = 0;
		currentGesture.clearAll();
		for (int i = 0; i < 3;i++)
			sumAccInEachStep[i] = 0;
		for (int i = 0; i < 5;i++)
			for (int j = 0;j<3;j++)
				dataInEachStep[i][j] = 0;
		isFinishSegment = false;
		doneReset = true;
	}

	void initDataAtStartGesture() {
		isFinishSegment = true;
		numStepNonGapGesture = 0;
	}

	void processDataAtEndGesture() {
		cout << "already process" << endl;
		switch (modeOfProcess) {
		case Mode::RECOGNIZEKNN:
			//TODO : print symbol of gesture from compare function
			cout << getTypeOfGestures() << endl;
			break;
		case Mode::RECOGNIZEKC:
			cout << getTypeOfGesturesWithMean() << endl;
			break;
		case Mode::TRAINING:
			// TODO : get getsture and save into file
			if (!currentGesture.isIgnore()) {
				if (!startIgnore)
					bool result = examplarsData->addExamplars(currentGesture);
				else
					startIgnore = false;
			}
			else {
				cout << "ignore" << endl;
			}

			break				;
		case Mode::TEST:
			// TODO :: No collect data just pumd data to auto test and print result.
			break;
		case Mode::COLLECTDATA:
			// TODO: add collectData -> raw group 
			examplarsData->addTestGestures(currentGesture,0);
			break;
		}
		resetData();
	}

	bool isInSegment(float accelX, float accelY, float accelZ) {
		if (isStartGesture(accelX, accelY, accelZ)) {
			initDataAtStartGesture();
			return true;
		}
		else {
			if (isEndGesture()) {
				if (isFinishSegment) {
					processDataAtEndGesture();
				}
				return false;
			}
			else {
				numStepNonGapGesture++;
				return true;
			}
		}
	}

	void processSegment(float accelX, float accelY, float accelZ) {
		cout << "working" << endl;
		sumAccInEachStep[0] += accelX - dataInEachStep[indexForDataEachStep][0];
		sumAccInEachStep[1] += accelY - dataInEachStep[indexForDataEachStep][1];
		sumAccInEachStep[2] += accelZ - dataInEachStep[indexForDataEachStep][2];
		dataInEachStep[indexForDataEachStep][0] = accelX;
		dataInEachStep[indexForDataEachStep][1] = accelY;
		dataInEachStep[indexForDataEachStep][2] = accelZ;
		indexForDataEachStep = (indexForDataEachStep + 1) % NUMDATAFOREACHSTEP;
		if (indexForDataEachStep >= NUMDATAFOREACHSTEP - 1)
			isDataEachStepFull = true;
		countToStep++;
		if (countToStep >= NUMDATABETWEENSTEPS && isDataEachStepFull) {
			countToStep = 0;
			int acclStepX = quantizeAccelerometerValue(sumAccInEachStep[0] / float(NUMDATAFOREACHSTEP));
			int acclStepY = quantizeAccelerometerValue(sumAccInEachStep[1] / float(NUMDATAFOREACHSTEP));
			int acclStepZ = quantizeAccelerometerValue(sumAccInEachStep[2] / float(NUMDATAFOREACHSTEP));
			currentGesture.addStep(acclStepX, acclStepY, acclStepZ);
		}
	}

	int getTypeOfGestures() {
		int type = 0;
		float min = getDTWScore(examplarsData->examplars[0]);
		float dtwScore = 0;
		int sizeOfTraining = examplarsData->examplars.size();

		for (int i = 1;i < sizeOfTraining;i++) {
			dtwScore = getDTWScore(examplarsData->examplars[i]);
			if (min > dtwScore) {
				min = dtwScore;
				type = examplarsData->indexOfExamplars[i];
			}
		}
		cout << "Min value : " << min << endl;
		if (min < THRESHOLDOFREALGESTURE) {
			return type + 1;
		}
		else {
			return 0;
		}
	}

	int getTypeOfGesturesWithMean() {
		int type = 0;
		float min = getDTWScore(examplarsData->meanExamplar[0]);
		float dtwScore = 0;
		int sizeOfTraining = examplarsData->meanExamplar.size();

		for (int i = 1;i < sizeOfTraining;i++) {
			dtwScore = getDTWScore(examplarsData->meanExamplar[i]);
			if (min > dtwScore) {
				min = dtwScore;
				type = i;
			}
		}
		cout << "Min value : " << min << endl;
		if (min < THRESHOLDOFREALGESTURE) {
			return type + 1;
		}
		else {
			return 0;
		}
	}

	float getDTWScore(Gesture* examplar) {
			return dtwAlg->calDTW(currentGesture, examplar);
	}
};


#endif // !RECOGNITION