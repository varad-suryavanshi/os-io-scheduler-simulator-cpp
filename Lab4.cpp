#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <climits>
#include <deque>
#include <string>
#include <memory> 
using namespace std;



struct IORequest {
    int arrival_time;
    int track_number;
    int start_time;
    int end_time;

    IORequest(int at, int tn) : arrival_time(at), track_number(tn), start_time(-1), end_time(-1) {}
};

// Base class for IO scheduler
class IOscheduler {
public:
    virtual void addRequest(IORequest* request) = 0;
    virtual IORequest* getNextRequest(int current_time, int current_track) = 0;
    virtual bool hasPendingRequests() = 0;
    virtual ~IOscheduler() {}
};

// FIFO Scheduler Class
class FIFO : public IOscheduler {
private:
    queue<IORequest*> io_queue;

public:
    void addRequest(IORequest* request) override {
        io_queue.push(request);
    }

    IORequest* getNextRequest(int current_time, int current_track) override {
        if (io_queue.empty()) return nullptr;
        IORequest* next_request = io_queue.front();
        io_queue.pop();
        return next_request;
    }

    bool hasPendingRequests() override {
        return !io_queue.empty();
    }
};

// SSTF Scheduler Class
class SSTF : public IOscheduler {
private:
    vector<IORequest*> io_requests;

public:
    void addRequest(IORequest* request) override {
        io_requests.push_back(request);
    }

    IORequest* getNextRequest(int current_time, int current_track) override {
        if (io_requests.empty()) return nullptr;

        size_t min_index = 0;
        int min_seek_time = INT_MAX;

        for (size_t i = 0; i < io_requests.size(); ++i) {
            int seek_time = abs(io_requests[i]->track_number - current_track);
            if (seek_time < min_seek_time) {
                min_seek_time = seek_time;
                min_index = i;
            }
        }

        IORequest* next_request = io_requests[min_index];
        io_requests.erase(io_requests.begin() + min_index);
        return next_request;
    }

    bool hasPendingRequests() override {
        return !io_requests.empty();
    }
};



// LOOK Scheduler Class
class LOOK : public IOscheduler {
private:
    vector<IORequest*> io_requests; 
    bool direction; 

public:
    LOOK() : direction(true) {}

    void addRequest(IORequest* request) override {
        io_requests.push_back(request);
    }

    IORequest* getNextRequest(int current_time, int current_track) override {
        if (io_requests.empty()) return nullptr;

        if (direction){
        sort(io_requests.begin(), io_requests.end(),
            [](IORequest* a, IORequest* b) {
                if (a->track_number == b->track_number) {
                    return a->arrival_time < b->arrival_time;
                }
                return a->track_number < b->track_number;
            });
        }
        else{
            sort(io_requests.begin(), io_requests.end(),
            [](IORequest* a, IORequest* b) {
                if (a->track_number == b->track_number) {
                    return a->arrival_time > b->arrival_time;
                }
                return a->track_number < b->track_number;
            });
        }
        IORequest* next_request = nullptr;

        if (direction) { // Moving upwards
            for (auto* req : io_requests) {
                if (req->track_number >= current_track) {
                    next_request = req;
                    break;
                }
            }
        } else { // Moving downwards
            for (auto it = io_requests.rbegin(); it != io_requests.rend(); ++it) {
                if ((*it)->track_number <= current_track) {
                    next_request = *it;
                    break;
                }
            }
        }

        if (!next_request) {
            direction = !direction;
            return getNextRequest(current_time, current_track);
        }

        io_requests.erase(remove(io_requests.begin(), io_requests.end(), next_request), io_requests.end());
        return next_request;
    }


    bool hasPendingRequests() override {
        return !io_requests.empty();
    }
};



// CLOOK Scheduler Class
class CLOOK : public IOscheduler {
private:
    vector<IORequest*> io_requests; 

public:
    void addRequest(IORequest* request) override {
        io_requests.push_back(request);
    }

    IORequest* getNextRequest(int current_time, int current_track) override {
        if (io_requests.empty()) return nullptr;

        sort(io_requests.begin(), io_requests.end(),
             [](IORequest* a, IORequest* b) {
                 if (a->track_number == b->track_number) {
                     return a->arrival_time < b->arrival_time;
                 }
                 return a->track_number < b->track_number;
             });

        IORequest* next_request = nullptr;

        for (auto* req : io_requests) {
            if (req->track_number >= current_track) {
                next_request = req;
                break;
            }
        }

        if (!next_request) {
            next_request = io_requests.front(); 
        }

        io_requests.erase(remove(io_requests.begin(), io_requests.end(), next_request), io_requests.end());

        return next_request;
    }

    bool hasPendingRequests() override {
        return !io_requests.empty();
    }
};



// FLOOK Scheduler Class
class FLOOK : public IOscheduler {
private:
    deque<IORequest*> active_queue;  
    deque<IORequest*> add_queue;     
    bool direction;                  

public:
    FLOOK() : direction(true) {} 

    void addRequest(IORequest* request) override {
        add_queue.push_back(request);
    }

    IORequest* getNextRequest(int current_time, int current_track) override {
        if (active_queue.empty()) {
            if (add_queue.empty()) return nullptr; 
            active_queue.swap(add_queue);
            direction = true; 
        }
        if (direction){
            sort(active_queue.begin(), active_queue.end(),
                 [](IORequest* a, IORequest* b) {
                     if (a->track_number == b->track_number) {
                         return a->arrival_time < b->arrival_time;
                     }
                     return a->track_number < b->track_number;
                 });
        }
        else{
            sort(active_queue.begin(), active_queue.end(),
                 [](IORequest* a, IORequest* b) {
                     if (a->track_number == b->track_number) {
                         return a->arrival_time > b->arrival_time;
                     }
                     return a->track_number < b->track_number;
                 });
        }
        

        IORequest* next_request = nullptr;

        if (direction) { // Moving upwards
            for (auto* req : active_queue) {
                if (req->track_number >= current_track) {
                    next_request = req;
                    break;
                }
            }
        } else { // Moving downwards
            for (auto it = active_queue.rbegin(); it != active_queue.rend(); ++it) {
                if ((*it)->track_number <= current_track) {
                    next_request = *it;
                    break;
                }
            }
        }

        if (!next_request) {
            direction = !direction;
            return getNextRequest(current_time, current_track);
        }

        active_queue.erase(remove(active_queue.begin(), active_queue.end(), next_request), active_queue.end());

        return next_request;
    }

    bool hasPendingRequests() override {
        return !active_queue.empty() || !add_queue.empty();
    }
};



int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        return 1;
    }

    string sched_algo = "N"; // Default scheduler (FIFO)
    string input_file;

    if (argc == 3) {
        string arg = argv[1];
        if (arg.rfind("-s", 0) == 0 && arg.size() > 2) {
            sched_algo = arg.substr(2); 
        } else {
            return 1;
        }
        input_file = argv[2];
    } else {
        input_file = argv[1];
    }

    vector<IORequest> requests;
    ifstream infile(input_file);
    if (!infile) {
        return 1;
    }

    string line;
    while (getline(infile, line)) {
        if (line[0] == '#') continue; 
        stringstream ss(line);
        int arrival, track;
        ss >> arrival >> track;
        requests.emplace_back(arrival, track);
    }

    IOscheduler* scheduler = new FLOOK();
    if (sched_algo == "N") {
        scheduler = new FIFO();
    } else if (sched_algo == "S") {
        scheduler = new SSTF();
    } else if (sched_algo == "L") {
        scheduler = new LOOK();
    } else if (sched_algo == "C") {
        scheduler = new CLOOK();
    } else if (sched_algo == "F") {
        scheduler = new FLOOK();
    } else {
        return 1;
    }

    IORequest* active_request = nullptr;
    int current_time = 0;
    int current_track = 0;

    int total_time = 0;
    int total_movement = 0;
    double total_wait_time = 0;
    double total_turnaround_time = 0;
    int max_wait_time = 0;
    int busy_time = 0; 

    size_t index = 0; 

    while (true) {
        while (index < requests.size() && requests[index].arrival_time == current_time) {
            scheduler->addRequest(&requests[index]);
            index++;
        }

        if (active_request && active_request->end_time == current_time) {
            total_turnaround_time += (active_request->end_time - active_request->arrival_time);
            total_wait_time += (active_request->start_time - active_request->arrival_time);
            max_wait_time = max(max_wait_time, active_request->start_time - active_request->arrival_time);

            current_track = active_request->track_number;
            active_request = nullptr; 
        }

        if (!active_request && scheduler->hasPendingRequests()) {
            active_request = scheduler->getNextRequest(current_time, current_track);
            if (active_request) {
                active_request->start_time = current_time;
                active_request->end_time = current_time + abs(active_request->track_number - current_track);

                busy_time += (active_request->end_time - active_request->start_time);
                total_movement += abs(active_request->track_number - current_track);

                if (active_request->start_time == active_request->end_time) {
                    total_turnaround_time += (active_request->end_time - active_request->arrival_time);
                    total_wait_time += (active_request->start_time - active_request->arrival_time);
                    max_wait_time = max(max_wait_time, active_request->start_time - active_request->arrival_time);

                    current_track = active_request->track_number;
                    active_request = nullptr;
                    continue;
                }
            }
        }

        if (!active_request && !scheduler->hasPendingRequests() && index == requests.size()) {
            break;
        }

        current_time++;
    }

    total_time = current_time;
    double io_utilization = static_cast<double>(busy_time) / total_time;
    double avg_turnaround = static_cast<double>(total_turnaround_time) / requests.size();
    double avg_wait_time = static_cast<double>(total_wait_time) / requests.size();

    for (size_t i = 0; i < requests.size(); ++i) {
        printf("%5zu: %5d %5d %5d\n", i, requests[i].arrival_time, requests[i].start_time, requests[i].end_time);
    }
    printf("SUM: %d %d %.4lf %.2lf %.2lf %d\n",
           total_time, total_movement, io_utilization, avg_turnaround, avg_wait_time, max_wait_time);

    return 0;
}


