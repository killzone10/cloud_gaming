#pragma once

#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <complex>
#include <sstream>
#include <string>
#include <iomanip>
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQ
{
private:
    std::queue<T> q; // no other data structures are allowed
    std::mutex m; // mutex
    std::condition_variable cond; // cond variable

public:
    void push(const T &value)
    {
        std::unique_lock<std::mutex> lock{m}; // locking queue - until pushing value
        q.push(value);
        notify();

    }
     void push(T &&value)
    {
        std::unique_lock<std::mutex> lock{m}; // locking queue - until pushing value
        q.push(std::move(value));
        notify();
    }

    void pop(T &value)
    {

        std::unique_lock<std::mutex> lock{m}; // locking queue
        
        while (q.empty()){
            cond.wait(lock); // unlocking queue - workers wait there

        }
        value = q.front();
        q.pop();
    }

    void notify(){
        cond.notify_one();

    }
    void notify_all(){ // notifies so the producer can do it from the outside
        cond.notify_all();
    }
    


    size_t size()
    {
        return q.size();
    }

    bool empty()
    {
        return q.empty();
    }
     std::queue<T> get_queue(){
        return q;
     }
};