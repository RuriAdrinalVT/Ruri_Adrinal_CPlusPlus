

#include <mutex>
#include <queue>
#include <condition_variable>
#include "RuriPID.h"
#include <memory>

using namespace std;

// Thread-safe queue
template <typename T>//ruri disini T sesuai dengan datatype Tqueue object
class TSDataExchange:MiniPID {
private:
    // Underlying queue
    std::queue<T> m_queue;//ruri: queue object instantiate

    // mutex for thread synchronization
    std::mutex m_mutex;

    // Condition variable for signaling
    std::condition_variable m_cond;

public:

    //ruri: get data from PID algorithm
    double getPIDdata(void){
        init();
        setP(4.0);
        setI(4.0);
        setD(7.0);
        setSetpoint(20.0);
        setMaxIOutput(6.0);
        return(getOutput());        
    }    
    // Pushes an element to the queue
    void initPID(){
        
    }
    void pushSensorData(T PIDdata)
    {
	
        std::unique_lock<std::mutex> lock(m_mutex);
        initPID();
        m_queue.push(PIDdata);//queue object method 
        m_cond.notify_one();
    }
    T popPIDdata()
    {

        std::unique_lock<std::mutex> lock(m_mutex);
        m_cond.wait(lock,
                    [this]() { return !m_queue.empty(); });

        T item = m_queue.front();
        m_queue.pop();
        return item;
    }
};

int main()
{
    TSDataExchange<double> Tqueue;//ruri nilai double harus sesuai dengan yang diinput oleh getsensor datatype karena T typename dimana T disini adalah double
    //std::shared_ptr<MiniPID> TestPID(new MiniPID);//ruri: sepertinya shared_ptr tidak perlu constructor, karena automatis

    //ruri: store data/push sensor data
    Tqueue.initPID();
    Tqueue.pushSensorData(Tqueue.getPIDdata());//push itu adalah data dari getSensorData sebagai passing SensorItem
    Tqueue.pushSensorData(Tqueue.getPIDdata());
    Tqueue.pushSensorData(Tqueue.getPIDdata());

    // ruri: pop and getsome sensor data
    std::cout <<"Ruri PID output achieved "<< Tqueue.popPIDdata() << std::endl;
    std::cout << Tqueue.popPIDdata() << std::endl;
    std::cout << Tqueue.popPIDdata() << std::endl;

    return 0;
}