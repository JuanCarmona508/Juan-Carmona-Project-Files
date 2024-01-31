#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h> 

using namespace std;
const int ARR = 1;
const int DEP = 2;
float cl;// clock
int readyQueueCount;
bool serverIdle;
float s, a, r, at, st;
struct event{
    int type;
    float time;
    event* nextEvent;
};
event* eventQ = new event();
void sortedQueue(event** head,event* new_node){
    event* current;
    
    if (*head == NULL|| (*head)->time > new_node->time) {
        new_node->nextEvent = *head;
        *head = new_node;
    }
    else {
        current = *head;
        while (current->nextEvent != NULL && current->nextEvent->time < new_node->time) {
            current = current->nextEvent;
        }
        new_node->nextEvent = current->nextEvent;
        current->nextEvent = new_node;
    }
}
void sched_event(int AR,float t,event** eq_head){
    event* e = new event();
    e->time = t;
    e->type = AR;
    sortedQueue(eq_head, e);
}
void init(){
    cl = 0.0;
    serverIdle = true;
    readyQueueCount = 0;
    event* eq_head = NULL;
    float t = a;
    sched_event(ARR,t,&eq_head);
    eventQ = eq_head;
}
void arr_handler(event** e) {
    if (serverIdle) {
   
        sched_event(DEP,cl+s,e);
	serverIdle = false;

    }
    else
        readyQueueCount++;
    sched_event(ARR,cl+a,e);
    
}
void dep_handler(event** e) {
    if (readyQueueCount == 0)
        serverIdle = true;
    else {
        readyQueueCount--;
        sched_event(DEP,cl+s,e);
    }
}
void printList(event* head){
	event* current = head;
	int err = 0;
        while (current != NULL && err < 10) {
	    cout << "time = " << current->time << " type = " << current->type << endl;
            current = current->nextEvent;
	    err++;
        }
}
void run(){
    event* ready = new event();
    //int err = 0; // error
    int p = 0;
    int ra = 0;
    float task = 0;
    float ta = 0;
    float ast = 0;
    float w = 0;
    float old_cl;
    while(p < 10000 ){//&& err < 20000 ){
	
        r = (float) rand()/RAND_MAX;
        a = -1.0*(1.0/at)*log(r);
	r = (float) rand()/RAND_MAX;
        s = -1.0*st*log(r);
        event* e = eventQ;
        old_cl = cl;
        cl = e->time;
        ra = ra + readyQueueCount;
	
	
        switch(e->type){
            case ARR:
		//err++; // prevention of infinite loop
		sched_event(ARR, cl, &ready); // storing arrival timesi
                arr_handler(&e);
                break;
            case DEP:
		p++;
                ast = ast+s;
		
		ta = ta + cl - ready->time;
		event* temp = new event(); 
		temp = ready;
		ready = ready->nextEvent;
                dep_handler(&e);
		delete temp;
                break;
        }
        eventQ = e;
        eventQ = eventQ->nextEvent;
        delete e;
        
	}
     //cout << "processes " << p <<endl;
     ast = 1.0*ast/p;
     w = (float) ra/p;
     cout << "Avg Turnaround Time: " << ta/p << endl;
     cout << "Throughput: " << p/cl << endl;
     cout << "Utilization: " << (p/cl)*ast << endl;
     cout << "Average processes in the Ready Queue: " << w << endl;
     
}
int main(int argc, char *argv[]) {
    
    srand(123);
    at = atof(argv[1]);
    st = atof(argv[2]);
    r = (float) rand()/RAND_MAX;
    a = -1.0*(1.0/at)*log(r);
    s = -1.0*st*log(r);
    init();
    run();
    delete eventQ;
    return 0;
}
