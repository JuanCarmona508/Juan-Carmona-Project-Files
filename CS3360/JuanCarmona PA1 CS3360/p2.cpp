
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h> 
using namespace std;
struct Server{
	float fail;    
	float restored;
};
int server(Server server[]){
	float limit = 24.0*365.0*20.0;
	int i = 0;
	float t = 0.0;
	float r;
	while(t < limit){
		
		r = (float) rand()/RAND_MAX;
		t = t+-500.0*log(r);
		if(t < limit){
			server[i].fail = t;
			server[i].restored = t+10.0;
			t = t+10.0;
			cout << "Failure: " << server[i].fail << " hrs, "
			<< "Restored: " << server[i].restored << " hrs" << endl;                                                                                             
			i++;
		}
	}
	return i;
}
float total_failure(Server s1[], Server s2[], int count){
	int i = 0;
	int j = 0;
	float fail = 0.0;
	bool failure = false;
	while(i< count && !failure){
		if(s1[i].fail <= s2[j].fail){
			if(s1[i].restored > s2[j].fail){
                                cout << "Total Failure at: " << s2[j].fail << " hrs" << endl;
                                failure = true;
                                fail = s2[j].fail;
                        }
			else if( (i+1) < count && s1[i+1].fail < s2[j].fail){
				j--;
			}
			
		}
		else{
 			if(s2[j].restored > s1[i].fail){
                                cout << "Total Failure at: " << s1[i].fail << " hrs" << endl;
                                failure = true;
                                fail = s1[i].fail;
                        }
			else if( (j+1) < count && s2[j+1].fail < s1[i].fail){
				i--; 
			}
			
		}
		i++;
		j++;
	}
	if(i >= count && !failure){
		cout << "No Total Failure." << endl;
	}
	return fail;
}
float mirror(Server s1[], Server s2[]){
	cout << "Server 1: " << endl;
	int s1Fails = server(s1);
	cout << endl;
	cout << "Server 2: " << endl;
	int s2Fails = server(s2);
	cout << endl;
	if(s1Fails <= s2Fails)
		return total_failure(s1,s2,s1Fails);
	else
		return total_failure(s1,s2,s2Fails);
	}
int main() {
	Server s1[700];
	Server s2[700];
	float avg = 0.0;
	for(int i = 0; i < 5; i++){
		srand(i);
		avg = avg + mirror(s1,s2);
	}
	cout << "Average Total Failure Time: " << avg/5 << " hrs" << endl;
	return 0;
}                                                                                                                                                                                                                                                                                                                                                             
