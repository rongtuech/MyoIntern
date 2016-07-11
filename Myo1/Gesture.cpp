#ifndef EXAMPLAR
#define EXAMPLAR

#include <iostream>
#include <vector>;
using namespace std;
/*- Class construct examplar data for DTW-*/
class Gesture {
public:
	vector<int> sequence_x;
	vector<int> sequence_y; 
	vector<int> sequence_z;
	
	Gesture() {

	}

	Gesture(Gesture *copyGesture) {
		sequence_x = copyGesture->sequence_x;
		sequence_y = copyGesture->sequence_y;
		sequence_z = copyGesture->sequence_z;
	}

	void addStep(int x, int y, int z) {
		sequence_x.push_back(x);
		sequence_y.push_back(y);
		sequence_z.push_back(z);
	}

	void divideStep(int index, int denominator) {
		sequence_x[index] /= denominator;
		sequence_y[index] /= denominator;
		sequence_z[index] /= denominator;
	}

	void clearAll() {
		sequence_x.clear();
		sequence_y.clear();
		sequence_z.clear();
	}
	 
	void addData(int x, int y, int z, int index) {
		sequence_x[index] += x;
		sequence_y[index] += y;
		sequence_z[index] += z;
	}

	void print() {
		for (int i = 0; i < sequence_x.size(); i++)
			cout << sequence_x[i] << " " << sequence_y[i] << " " << sequence_z[i] << endl;
		cout << "----------------------------" << endl;
	}

	bool isIgnore() {
		if (sequence_x.size() <= 2)
			return true;
		return false;
	}
};
#endif