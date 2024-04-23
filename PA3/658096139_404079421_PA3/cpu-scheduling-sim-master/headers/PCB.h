//
// Created by Joseph on 10/7/2017.
//

#ifndef LAB2_PCB_H
#define LAB2_PCB_H

#include <iostream>

//data structure to hold process information
struct PCB{
    int pid, arrival, burst, priority, num_context;
    float time_left, resp_time, wait_time, finish_time;
    bool started;
    int io_burst_remaining, io_burst; 
    bool first_time_appear;
    PCB(){pid = arrival = burst = time_left = priority = resp_time = wait_time = num_context = finish_time = started = io_burst_remaining = io_burst =0; first_time_appear=1;}
    PCB(int id, int arr, int time, int prio, int io) : pid(id), arrival(arr), burst(time), time_left(time), priority(prio), io_burst(io){
        resp_time = wait_time = num_context = finish_time = started =0;
        first_time_appear=1;
        io_burst_remaining= io;
    }
    void print(){
        std::cout << pid << " " << arrival << " " << time_left << " " << priority << std::endl;
    }
};
#endif //LAB2_PCB_H
