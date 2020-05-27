#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

#define PCADIM 20
struct Features{
	float dim[PCADIM];
};

struct Element{
	int idx; 
	float distance; 
};

struct CompareDistance { 
    bool operator()(Element const& p1, Element const& p2) 
    { 
        // return "true" if "p1" is ordered  
        // before "p2", for example: 
        return p1.distance < p2.distance; 
    } 
};

int main(){

	/*
	priority_queue<Element, vector<Element>, CompareDistance> Q;
	Element e1, e2, e3;
	e1.idx = 1; e2.idx = 2; e3.idx = 3;
	e1.distance = 1.1;
	e2.distance = 1.2;
	e3.distance = 1.3;	
	
	Q.push(e2);
	Q.push(e3);
	Q.push(e1);

 	while (!Q.empty()) { 
        	Element p = Q.top(); 
        	Q.pop(); 
        	cout << p.idx << " " << p.distance << "\n"; 
    	} 	

	return 0;
	*/

	std::ifstream ifs("/media/yulian/Data/rawfeaturesdata/pcafeatures.bin0", std::ifstream::in | std::ifstream::binary);	
	vector<Features> vec;

	bool firstEle = false;
	Features first_f;

	while (!ifs.eof()){
		Features f;
		ifs.read((char*)&f, sizeof(Features));
		if(firstEle == false){
			memcpy(&first_f, &f, sizeof(Features));
			firstEle = true;
		}

		if(ifs.eof())break;
		for(int i = 0; i < PCADIM; i++){
			if(f.dim[i] > 1.0 || f.dim[i] < -1.0){
				cout << "f: " << f.dim[i] << endl;			
			}
	
		}
		vec.push_back(f);
	}

	ifs.close();

	priority_queue<Element, vector<Element>, CompareDistance> Q;	



	cout << "vec.size(): " << vec.size() << endl;

	for(int i = 0; i < vec.size(); i++){
		float diff = 0.0;
		for(int j = 0; j < PCADIM; j++){
			float t = first_f.dim[j] - vec[i].dim[j];
			diff += t * t;
		}
		Element ele;
		ele.idx = i;
		ele.distance = diff;
		Q.push(ele);

		if(Q.size() >= 20){
			Q.pop();
		}
	}
	
 	while (!Q.empty()) { 
        	Element p = Q.top(); 
        	Q.pop(); 
        	cout << p.idx << " " << p.distance << "\n"; 
    	} 	

	return 0;
}
