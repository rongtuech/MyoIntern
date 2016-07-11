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
	float traceArray[ROWLENGTH][COLLENGTH]; // 1: up 2:left 3:cross

	float calDTW(Gesture examplar, Gesture currentAccl) {
		sizeExp = examplar.sequence_x.size();
		sizeCurAccl = currentAccl.sequence_x.size();
		int way = 0;
		initDistanceArray(examplar, currentAccl);
		for (int i = 0; i < sizeCurAccl; i++) {
			for (int j = 0; j < sizeExp; j++) {
				if (i == 0 && j == 0) {
					dtwArray[i][j] = distanceArray[i][j];
					traceArray[i][j] = 0;
				} else if (i == 0) {
					dtwArray[i][j] = dtwArray[i][j-1] + distanceArray[i][j];
					traceArray[i][j] = 2;
				} else if (j == 0) {
					dtwArray[i][j] = dtwArray[i-1][j] + distanceArray[i][j];
					traceArray[i][j] = 1;
				}
				else {
					dtwArray[i][j] = distanceArray[i][j] + getMinOf3Value(dtwArray[i - 1][j], dtwArray[i][j - 1], dtwArray[i - 1][j - 1], way);
					traceArray[i][j] = way;
				}
			}
		}
		resetDistanceArray();

		return dtwArray[sizeCurAccl - 1][sizeExp - 1];
	}

	Gesture* calMeanOfGesutures(vector<Gesture*> cluster) {
		Gesture *meanExamplar = new Gesture(cluster[0]);
		int lenghtExamplar = meanExamplar->sequence_x.size();
		vector<int> countStepExamplar(lenghtExamplar);
		int indexExp = 0;
		int indexCurAccl = 0;
		int way = 1000;

		for (int i = 1; i < cluster.size(); i++) {
			calDTW(meanExamplar, cluster[i]);
			indexExp = meanExamplar->sequence_x.size() - 1;
			indexCurAccl = cluster[i]->sequence_x.size() - 1;

			while (way != 0) {
				meanExamplar->addData(cluster[i]->sequence_x[indexCurAccl], cluster[i]->sequence_y[indexCurAccl], cluster[i]->sequence_z[indexCurAccl], indexExp);
				countStepExamplar[indexExp] ++;
				way = traceArray[indexCurAccl][indexExp];
				switch (way) {
				case 1:
					indexCurAccl--;
					break;
				case 2:
					indexExp--;
					break;
				case 3:
					indexExp--;
					indexCurAccl--;
					break;
				default:
					break;
				}
			}
		}
		for (int i = 0;i < lenghtExamplar; i++) {
			meanExamplar->divideStep(i, countStepExamplar[i]);
		}

		return meanExamplar;
	}

private:
	int sizeExp;
	int sizeCurAccl;


	float getMinOf3Value(float value1, float value2, float value3, int &way) {
		if (value1 <= value2 && value1 <= value3) {
			way = 1;
			return value1;
		}
		else if (value2 <= value1 && value2 <= value3) {
			way = 2;
			return value2;
		}
		else {
			way = 3;
			return value3;
		}
	}

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
