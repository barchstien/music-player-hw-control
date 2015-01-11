#ifndef _UTIL_
#define _UTIL_

#include <string>
#include <sstream>
#include <iostream>
#include <deque>
#include <memory>
#include <mutex>

/** pin map */
//many are useless, and will be remove with upcoming HW layout
#define GPIO_RELAY_0 17
#define GPIO_RELAY_1 27
#define GPIO_RELAY_POWER 22
#define GPIO_FRONT_LED 12

#define GPIO_AUX_1 24
#define GPIO_AUX_2 23
#define GPIO_ONOFF 4
#define GPIO_BUTTON_UP 25
#define GPIO_BUTTON_DOWN 16
#define GPIO_SWITCH_NET_MODE 5
#define GPIO_SWITCH_MODE 6


/** Utilities that cannot be classed in other files */
class Util {
public:
    /** execute a bash command and read output until the end
    WARNING : this may block execution if bash command never ends
    @param cmd the command to execute
    @return the string returned by the bash command or "ERROR" if Pipe couldn't be created */
    static std::string exec_read_bash(const char* cmd);
    
    /** execute a bash command and return the pid of the new process
    DOES NOT read the output of the forked process
    @param cmd the command to execute
    @return pid ofthe forked process*/
    static int fork_exec_bash(const char* cmd);
    
    static std::string get_time_string();
};

//#define LOG std::cout
#define LOG std::cout << Util::get_time_string() << " : "

struct Message{
    Message(int label) : label(label) {}
    Message(int label, std::string str) : label(label), data_string(str) {}
    ~Message() {}
    
    /** the command to execute */
    int label;
    /** data to be used for executing cmd */
    std::string data_string;
    
    static const int MESSAGE_TYPE = 0;
    virtual int message_type(){return MESSAGE_TYPE;};
    virtual std::string print(){
        std::stringstream ss;
        ss << label << " : " << data_string;
        return ss.str();
    };
};

template <class T> class ThreadSafeQ {
public:
    ThreadSafeQ(){};
    int enque(T m);
    T deque();
private:
    std::mutex m_mutex;
    std::deque<T> m_fifo;
};

template <class T> int ThreadSafeQ<T>::enque(T m){
    m_mutex.lock();
    m_fifo.push_back(m);
    m_mutex.unlock();
    return 0;
}

template <class T> T ThreadSafeQ<T>::deque(){
    m_mutex.lock();
    if (0 == m_fifo.size()){
        m_mutex.unlock();
        return 0;
    }
    T m = m_fifo.front();
    m_fifo.pop_front();
    m_mutex.unlock();
    return m;
}

#endif
