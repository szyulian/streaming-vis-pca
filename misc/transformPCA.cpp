#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define DIM 64
#define PACDIM 20

struct Feature{
	float dim[DIM];
};

struct FeaturePCA{
	float dim[PACDIM];
};

int main(){
	std::ifstream ifs("/media/yulian/Data/Downloads/streaming-vis-pca/MeansNComponents.bin", std::ifstream::in | std::ifstream::binary);
	vector<float> means;
	for(int i = 0; i < DIM; i++){
		float f = 0.0;
		ifs.read((char*)&f, sizeof(float));
		means.push_back(f);
	}

	vector<vector<float>> matrix;
	for(int i = 0; i < DIM; i++){
		vector<float> row;
		for(int j = 0; j < PACDIM; j++){
			float f = 0.0;
			ifs.read((char*)&f, sizeof(float));
			if(ifs.eof()){ cout << "eof1" << endl; break;}
			row.push_back(f);
		}
		if(ifs.eof()){ cout << "eof2" << endl; break;}
		matrix.push_back(row);
	}

	ifs.close();

	std::ifstream ifsO("/media/yulian/Data/rawfeaturesdata/gpusurffeatures.bin0", std::ifstream::in | std::ifstream::binary);
	Feature originalF;
	ifsO.read((char*)&originalF, sizeof(Feature));

	for(int i = 0; i < DIM; i++){
		originalF.dim[i] = originalF.dim[i] - means[i];
	}

	std::ifstream ifsP("/media/yulian/Data/rawfeaturesdata/pcafeatures.bin0", std::ifstream::in | std::ifstream::binary);
	FeaturePCA res;
	ifsP.read((char*)&res, sizeof(FeaturePCA));

	FeaturePCA pcaF;
	for(int i = 0; i < PACDIM; i++){
		float sum = 0.0;
		for(int j = 0; j < DIM; j++){
			sum += originalF.dim[j] * matrix[j][i];
		}
		pcaF.dim[i] = sum;
		cout << sum << "\t" << res.dim[i] << endl;
	}
	return 0;
}
