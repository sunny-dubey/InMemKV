#include<iostream>
#include<unordered_map>
#include<string>
#include<optional> 
#include<queue>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<fstream>

class Database{
    private:
    // Global memory storage: an unordered_map to store all key-value pairs in the database
    // Private global memory storage for key-value pairs.
    // Direct access is restricted to ensure thread safety and prevent race conditions;
    // all access should be managed through the class's methods.
    std::unordered_map<std::string, std::string> global_memory;

    // queue to hold log entries for persistence
    std::queue<std::string> log_queue; 
    
    // Mutex to ensure thread safety when accessing or modifying shared resources,
    // such as the log_queue or global_memory. This prevents data races and ensures
    // that only one thread can access the protected section at a time, maintaining
    // data integrity in concurrent environments.
    // Note: Always lock this mutex before performing any operations that modify or
    // read from shared data structures to avoid undefined behavior.
    std::mutex queue_mutex;

    // Worker thread responsible for performing background tasks
    // that persist data, ensuring that all keys and their values
    // are reliably stored in the database. This helps prevent
    // data loss in the event of a crash or unexpected shutdown
    std::thread worker_thread;

    // Condition variable used to notify the worker thread when there is new data to process
    std::condition_variable cv;

    // Flag to signal the worker thread to stop running; used for graceful shutdown
    bool stop_worker = false;


    void persistence_worker(){
        std::ofstream file("appendonly.aof", std::ios::app); // Open the file in append mode; if it does not exist, create it.
        // while(true){
        //     std::unique_lock<std::mutex> lock(queue_mutex);
        //     // Wait until queue is not empty or we want to stop
        //     cv.wait(lock, [] { return !log_queue.empty() || stop_worker; });
        // }
    }

    public:
    Database(){
        worker_thread = std::thread(pesistence_worker, this);
    }







    // Use 'const' to prevent modification before inserting into global memory, and pass by reference to avoid unnecessary copies on the stack.
    void put(const std::string& key, const std::string& value){
        this->global_memory[key] = value;  // 'this->' is optional here since there is no naming conflict
    }

    // return the value of the key if it exists, otherwise return nothing
    // using optional to return the value or nothing
    std::optional<std::string> get(const std::string &key){
        auto it = global_memory.find(key);
        if (it != global_memory.end()){
            return it->second;
        }
        return std::nullopt;
    }

    bool remove(const std::string &key){
        return global_memory.erase(key) > 0;
    }
};

int main(){
    std::cout<<"starting database engine"<<std::endl;
    Database db;
    db.put("name", "sunny");
    std::optional<std::string> result = db.get("name");
    if (result.has_value()) {
        std::cout << "Value: " << result.value() << std::endl;
    }
    return 0;
}