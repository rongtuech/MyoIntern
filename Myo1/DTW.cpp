#ifndef DTW_H
#define DTW_H

#include <cmath>
#include <vector>
#include "Gesture.cpp"
using namespace std;

#define ROWLENGTH 400
#define COLLENGTH 400 

class DTW {
public:
	float distanceArray[ROWLENGTH][COLLENGTH];
	float dtwArray[ROWLENGTH][COLLENGTH];

	float calDTW(Gesture examplar, Gesture currentAccl) {
		sizeExp = examplar.sequence_x.size();
		sizeCurAccl = currentAccl.sequence_x.size();
		initDistanceArray(examplar, currentAccl);
		for (int i = 0; i < sizeCurAccl; i++) {
			for (int j = 0; j < sizeExp; j++) {
				if (i == 0 || j == 0)
					dtwArray[i][j] = distanceArray[i][j];
				else
					dtwArray[i][j] = distanceArray[i][j] + getMinOf3Value(dtwArray[i - 1][j], dtwArray[i][j - 1], dtwArray[i - 1][j - 1]);
			}
		}
		resetDistanceArray();

		return dtwArray[sizeCurAccl - 1][sizeExp - 1];
	}



private:
	int sizeExp;
	int sizeCurAccl;

	float getMinOf3Value(float value1, float value2, float value3) {
		if (value1 <= value2 && value1 <= value3)
			return value1;
		else if (value2 <= value1 && value2 <= value3)
			return value2;
		else
			return value3;
	}
/*
	int getMinOf3Value(int value1, int value2, int value3) {
		if (value1 <= value2 && value1 <= value3)
			return value1;
		else if (value2 <= value1 && value2 <= value3)
			return value2;
		else
			return value3;
	}*/

	float calAbsDistance(Gesture examplar, Gesture currentAccl, int indexExamplar, int indexCurr) {
		return sqrtf(powf(examplar.sequence_x[indexExamplar] - currentAccl.sequence_x[indexCurr], 2) 
				   + powf(examplar.sequence_y[indexExamplar] - currentAccl.sequence_y[indexCurr], 2)
			       + powf(examplar.sequence_z[indexExamplar] - currentAccl.sequence_z[indexCurr], 2));
	}

	void initDistanceArray(Gesture examplar, Gesture currentAccl) {
		for (int i = 0; i < sizeCurAccl; i++) {
			for (int j = 0; j < sizeExp; j++) {
				distanceArray[i][j] = calAbsDistance(examplar,currentAccl,j,i);
			}
		}
	}

	void resetDistanceArray() {
		for (int i = 0; i < sizeCurAccl; i++) {
			for (int j = 0; j < sizeExp; j++) {
				distanceArray[i][j] = 0;
			}
		}
	}

};

#endif 
