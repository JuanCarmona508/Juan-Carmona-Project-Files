#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h> 
using namespace std;
struct Process{
    int proID; // process id
    float arrival; // arrival time 
    float service; // service time
    
};
int main() {
	srand(time(0));
    	Process process1[1000];
    	float r1, avgArr,avgSer;
	float arr = 0.0;
	float ser = 0.0;
    	for(int i = 1; i <= 1000; i++){
        	r1 = (float) rand()/RAND_MAX;
    		process1[i-1].proID = i;
		arr = arr + -1.0*(1.0/2.0)*log(r1); // arrival time
		process1[i-1].arrival = arr;
    		process1[i-1].service = -1.0*log(r1);
		ser  = ser + process1[i-1].service;
    	        cout << "<" << process1[i-1].proID << ", " << process1[i-1].arrival << "s, " << process1[i-1].service << "s>" << endl;                                                                         
    	}
	avgArr = 1000.0/arr; // arrivals per sec
	avgSer = ser/1000.0; // average service time
	cout << "Average Arrival rate: " << avgArr << " arrivals per second" <<endl;
	cout << "Average Service time: " << avgSer << "s" <<endl;
    	return 0;
}    

