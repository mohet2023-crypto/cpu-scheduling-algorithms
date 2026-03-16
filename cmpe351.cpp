/* This is code developed by <Abdullahi and Abdullahi> */
#include "cmpe351.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

struct Procs {
    int bruust, proity, arrivaltime, queueID, order;
    int waitngtime;
    Procs* next;
};

int countQueue(Procs* head, int qid) {
    int c = 0;
    Procs* p = head;
    while (p != NULL) {
        if (p->queueID == qid) c++;
        p = p->next;
    }
    return c;
}

void getQueue(Procs* all, int qid, Procs* arr[], int& count) {
    count = 0;
    Procs* p = all;
    while (p != NULL) {
        if (p->queueID == qid) {
            arr[count] = p;
            count++;
        }
        p = p->next;
    }
}

void fcfs(Procs* arr[], int count, int wt[]) {
    if (count == 0) return;
    
    Procs* sorted[1000];
    for (int i = 0; i < count; i++) sorted[i] = arr[i];
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sorted[j]->arrivaltime > sorted[j+1]->arrivaltime ||
               (sorted[j]->arrivaltime == sorted[j+1]->arrivaltime && sorted[j]->order > sorted[j+1]->order)) {
                Procs* temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = temp;
            }
        }
    }
    
    int time = 0;
    for (int i = 0; i < count; i++) {
        if (time < sorted[i]->arrivaltime) time = sorted[i]->arrivaltime;
        sorted[i]->waitngtime = time - sorted[i]->arrivaltime;
        time += sorted[i]->bruust;
    }
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (sorted[j]->order == arr[i]->order) {
                wt[i] = sorted[j]->waitngtime;
                break;
            }
        }
    }
}

void sjf(Procs* arr[], int count, int wt[]) {
    if (count == 0) return;
    
    bool done[1000] = {false};
    int at[1000], bt[1000], ord[1000];
    
    for (int i = 0; i < count; i++) {
        at[i] = arr[i]->arrivaltime;
        bt[i] = arr[i]->bruust;
        ord[i] = arr[i]->order;
        wt[i] = 0;
    }
    
    int time = 0, doneCount = 0;
    
    while (doneCount < count) {
        int idx = -1;
        int minBurst = 1000000;
        int minArrival = 1000000;
        int minOrder = 1000000;
        
        for (int i = 0; i < count; i++) {
            if (!done[i] && at[i] <= time) {
                if (bt[i] < minBurst ||
                   (bt[i] == minBurst && at[i] < minArrival) ||
                   (bt[i] == minBurst && at[i] == minArrival && ord[i] < minOrder)) {
                    minBurst = bt[i];
                    minArrival = at[i];
                    minOrder = ord[i];
                    idx = i;
                }
            }
        }
         if (idx == -1) {
            time++;
            continue;
        }
        
        wt[idx] = time - at[idx];
        time += bt[idx];
        done[idx] = true;
        doneCount++;
    }
    
    int finalWT[1000];
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (arr[j]->order == ord[i]) {
                finalWT[j] = wt[i];
                break;
            }
        }
    }
    
    for (int i = 0; i < count; i++) wt[i] = finalWT[i];
}

void priority(Procs* arr[], int count, int wt[]) {
    if (count == 0) return;
    
    bool done[1000] = {false};
    int at[1000], bt[1000], pri[1000], ord[1000];
    
    for (int i = 0; i < count; i++) {
        at[i] = arr[i]->arrivaltime;
        bt[i] = arr[i]->bruust;
        pri[i] = arr[i]->proity;
        ord[i] = arr[i]->order;
        wt[i] = 0;
    }
    
    int time = 0, doneCount = 0;
    
    while (doneCount < count) {
        int idx = -1;
        int minPri = 1000000;
        int minArrival = 1000000;
        int minOrder = 1000000;
        
        for (int i = 0; i < count; i++) {
            if (!done[i] && at[i] <= time) {
                if (pri[i] < minPri ||
                   (pri[i] == minPri && at[i] < minArrival) ||
                   (pri[i] == minPri && at[i] == minArrival && ord[i] < minOrder)) {
                    minPri = pri[i];
                    minArrival = at[i];
                    minOrder = ord[i];
                    idx = i;
                }
            }
        }
        
        if (idx == -1) {
            time++;
            continue;
        }
        
        wt[idx] = time - at[idx];
        time += bt[idx];
        done[idx] = true;
        doneCount++;
    }
    
    int finalWT[1000];
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (arr[j]->order == ord[i]) {
                finalWT[j] = wt[i];
                break;
            }
        }
    }
    
    for (int i = 0; i < count; i++) wt[i] = finalWT[i];
}

double average(int wt[], int count) {
    if (count == 0) return 0;
    double sum = 0;
    for (int i = 0; i < count; i++) sum += wt[i];
    return sum / count;
}

string formatAverage(double avg) {
    double truncated = floor(avg * 100) / 100.0;

    stringstream ss;
    ss << fixed << setprecision(2) << truncated;

    string s = ss.str();

    if (s.substr(s.size() - 3) == ".00") {
        s = s.substr(0, s.size() - 3);
    }

    return s;
}
int main(int argc, char* argv[]) {
    if (argc != 3) return 1;
    
    string input = argv[1];
    string output = argv[2];
    
    ifstream file(input);
    Procs* head = NULL;
    Procs* tail = NULL;
    int total = 0;
    string line;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        int vals[4];
        int pos = 0;
        
        for (int i = 0; i < 4; i++) {
            int colon = line.find(':', pos);
            string part = line.substr(pos, colon - pos);
            vals[i] = stoi(part);
            pos = colon + 1;
        }
        
        Procs* p = new Procs;
        p->bruust = vals[0];
        p->proity = vals[1];
        p->arrivaltime = vals[2];
        p->queueID = vals[3];
        p->order = total;
        p->waitngtime = 0;
        p->next = NULL;
        
        if (head == NULL) {
            head = tail = p;
        } else {
            tail->next = p;
            tail = p;
        }
        
        total++;
    }
    
    file.close();
    
    if (head == NULL) {
        ofstream outfile(output);
        outfile.close();
        return 0;
    }
    
    int qids[1000];
    int qcount = 0;
    Procs* p = head;
    
    while (p != NULL) {
        bool found = false;
        for (int i = 0; i < qcount; i++) {
            if (qids[i] == p->queueID) {
                found = true;
                break;
            }
        }
        if (!found) {
            qids[qcount] = p->queueID;
            qcount++;
        }
        p = p->next;
    }
    
    for (int i = 0; i < qcount - 1; i++) {
        for (int j = 0; j < qcount - i - 1; j++) {
            if (qids[j] > qids[j+1]) {
                int temp = qids[j];
                qids[j] = qids[j+1];
                qids[j+1] = temp;
            }
        }
    }
    
    ofstream out(output);
    
    for (int q = 0; q < qcount; q++) {
        int qid = qids[q];
        int count = countQueue(head, qid);
        
        Procs* queueArr[1000];
        getQueue(head, qid, queueArr, count);
        
        int fcfsWT[1000], sjfWT[1000], priWT[1000];
        double fcfsAvg, sjfAvg, priAvg;
        
        fcfs(queueArr, count, fcfsWT);
        fcfsAvg = average(fcfsWT, count);
        
        sjf(queueArr, count, sjfWT);
        sjfAvg = average(sjfWT, count);
        
        priority(queueArr, count, priWT);
        priAvg = average(priWT, count);
        
        for (int algo = 1; algo <= 3; algo++) {
            out << qid << ":" << algo << ":";
            
            int* wt;
            double avg;
            
            if (algo == 1) { wt = fcfsWT; avg = fcfsAvg; }
            else if (algo == 2) { wt = sjfWT; avg = sjfAvg; }
            else { wt = priWT; avg = priAvg; }
            
            if (count > 0) {
                for (int i = 0; i < count; i++) {
                    out << wt[i];
                    if (i < count - 1) out << ":";
                }
            } else {
                out << "0";
            }
            
            out << ":" << formatAverage(avg);
            
            if (!(q == qcount - 1 && algo == 3)) out << endl;
        }
    }
    
    out.close();
    
    while (head != NULL) {
        Procs* next = head->next;
        delete head;
        head = next;
    }
    
    return 0;
}
