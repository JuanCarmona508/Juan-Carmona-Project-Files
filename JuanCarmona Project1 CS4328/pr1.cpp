#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h> 

using namespace std;
const int ARR = 1;
const int DEP = 2;
float cl;// clock
int readyQueueCount, diskQueueCount, ne;
bool serverIdle, diskIdle;
float s, a, r, at, st,dt;
struct event{
    int type;
    int id;
    float time;
    float arrTime;
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
void sched_event(int AR,int id, float t, float arr, event** eq_head){
    event* e = new event();
    e->id = id;
    e->time = t;
    e->type = AR;
    e->arrTime = arr;
    sortedQueue(eq_head, e);
}
void init(){
    cl = 0.0;
    serverIdle = true;
    diskIdle = true;
    readyQueueCount = 0;
    int id = 1;
    event* eq_head = NULL;
    float t = a;
    sched_event(ARR,id,t,t,&eq_head);
    eventQ = eq_head;
}
void arr_handler(event** e) {
    if (serverIdle) {
   
        
        sched_event(DEP,(*e)->id, cl+s,(*e)->arrTime, e);
	serverIdle = false;

    }
    else{
        	readyQueueCount++;
	}
    //sched_event(ARR,(*e)->id, cl+a,cl+a, e);
    //cout << "new event counter = " << ne <<endl;
    sched_event(ARR, ne, cl+a,cl+a, e);
    
}
void dep_handler(event** e) {
    if (readyQueueCount == 0)
        serverIdle = true;
    else {
        readyQueueCount--;
        //cout << "departure created" << endl;
         sched_event(DEP,(*e)->id, cl+s,(*e)->arrTime, e);
    }
}
void printList(event* head){
	event* current = head;
	int err = 0;
        while (current != NULL && err < 10) {
	    cout << "time = " << current->time << " type = " << current->type << " ID = " << current->id <<
	     "Arrival time = " << current->arrTime << endl;
            current = current->nextEvent;
	    err++;
        }
}
void goNext(event** e){
	event* temp = new event();
                temp = *e;
                (*e) = (*e)->nextEvent;
                delete temp;
}
void run(){
    event* ready = new event();
    event* disk = new event();
    
    //int err = 0; // error
    diskQueueCount = 0;
    ne = 1;
    int p = 0;
    int ra = 0;
    int stch = 0;
    int depEv = 0;
    int diskEv = 0;
    float daq = 0;
    float ta = 0;
    float ast = 0;
    float dast = 0;
    float w = 0;
    float dw = 0;
    float old_cl;
    while(p < 10000 ){//&& err < 20000 ){ // loop
	
        r = (float) rand()/RAND_MAX;
        a = -1.0*(1.0/at)*log(r);
	r = (float) rand()/RAND_MAX;
        s = -1.0*st*log(r);
        event* e = eventQ;
        old_cl = cl;
        cl = e->time;
	stch++; //state check
	/*if(readyQueueCount !=0){
		readyQueueCount--;
		ra = ra + readyQueueCount;
		readyQueueCount++;
	}else
	ra = ra + readyQueueCount;
	
	if(diskQueueCount !=0){
		diskQueueCount--;
		daq = daq + (float) diskQueueCount;
		diskQueueCount++;
	} else
	daq = daq + (float) diskQueueCount;*/
        /*/ra = ra + readyQueueCount; //queue count for cpu
	//daq 
		cout << "Before event processed" << endl;
	 	cout << "process id " << e->id << endl;
                cout << "disk id " << disk->id << " disk queue: " << diskQueueCount << endl;
		cout << "ready id " << ready->id <<" ready queue: " << readyQueueCount <<endl;
                cout << "disk list" << endl;
                printList(disk);
                cout << "e list " << endl;
                printList(e);
		cout << "ready list " << endl;
		printList(ready);*/
                
        switch(e->type){
            case ARR:
		//err++; // prevention of infinite loop
		/**/
		sched_event(ARR,e->id, cl,e->arrTime, &ready);
		/**/
		if(disk->id == e->id){
			//cout << "disk Queue: " << diskQueueCount << endl;
			if(diskQueueCount == 0){
				diskIdle = true;
				disk->id = 0;
			}
			else{
				diskQueueCount--;
				a = -1.0*dt*log(r);
				diskEv++;
				dast = dast + a;
				//cout << "disk departure" << endl << endl;
				 if(disk->nextEvent != NULL)
                               		goNext(&disk);
                         	else
                               		disk->id = 0;
				sched_event(ARR,disk->id, cl+a,disk->arrTime,&e);
				/**/
				
			}	
			 /**/
			
			readyQueueCount++;
			 
		} else {
			
			ne++;
			
		
                	arr_handler(&e);
	
		}
		
		if(ready->id == 0)
			goNext(&ready);
		
		/*cout << "Updated ready id " << ready->id << endl;
		cout << "ready list" << endl;	
		printList(ready);
		cout << "e list" << endl;
		printList(e);*/
                break;
            case DEP:
		depEv++;
		ast = ast+s;
		//cout << "departure from cpu" << endl;
		r = (float) rand()/RAND_MAX;
		
		/**/
		if( r <= 0.6){
			p++;
                	
			ta = ta + cl - ready->arrTime;
			//
			/**/
			
		//cout << "id =  " << e->id << " Arrival Time: "<< e->arrTime  <<  " Turnaround  =  " << cl - e->arrTime << endl;
				
		}else{
			//err++;
			sched_event(ARR,e->id, cl,e->arrTime,&disk);
			if(disk->id == 0)
                        	goNext(&disk);
			/**/
			
			if(diskIdle){
				r = (float) rand()/RAND_MAX;
				a = -1.0*dt*log(r);
				diskEv++;
				dast = dast + a;
				sched_event(ARR,e->id, cl+a,e->arrTime,&e);
				diskIdle = false;
			}
			else{
				
				diskQueueCount++;
		
			}
		}
		if(readyQueueCount != 0){
                        e->id = ready->nextEvent->id;
			e->arrTime = ready->nextEvent->arrTime;
		}
		/**/
		 if(ready->nextEvent != NULL)
                                        goNext(&ready);
                                else
                                        ready->id = 0;
                dep_handler(&e);
		//goNext(&ready);	
		//printList(ready);
		
		/**/
                break;
        }
	ra = ra + readyQueueCount; //queue count for cpu
	daq = daq + (float) diskQueueCount; // queue count for disk
	/*cout << "After event processed" << endl;
        cout << "process id " << e->id << endl;
        cout << "disk id " << disk->id << " disk queue: " << diskQueueCount << endl;
        cout << "ready id " << ready->id <<" ready queue: " << readyQueueCount <<endl;
        cout << "disk list" << endl;       //sched_event(ARR,e->id, cl,e->arrTime, &ready);
        printList(disk);
        cout << "e list " << endl;
        printList(e);
        cout << "ready list " << endl;
        printList(ready);*/ //prints lists after event is processed 
        eventQ = e;
        eventQ = eventQ->nextEvent;
        delete e;
        
	}
     //cout << "processes " << p <<endl;
     ast = 1.0*ast/depEv;
     dast = dast/diskEv;
     w = (float) ra/stch;
     dw = (float) daq/stch;
     cout << "Avg Turnaround Time: " << ta/p << endl;
     cout << "Throughput: " << p/cl << endl;
     cout << "CPU Utilization: " << (depEv/cl)*ast << endl;
     cout << "Disk Utilization: " << (diskEv/cl)*dast << endl;
     cout << "Average processes in the CPU Ready Queue: " << w << endl;
     cout << "Average processes in the disk Queue: " << dw << endl;
     
}
int main(int argc, char *argv[]) {
    
    srand(123);
    at = atof(argv[1]);
    st = atof(argv[2]);
    r = (float) rand()/RAND_MAX;
    a = -1.0*(1.0/at)*log(r);
    s = -1.0*st*log(r);
    dt = atof(argv[3]);
    init();
    run();
    delete eventQ;
    return 0;
}
