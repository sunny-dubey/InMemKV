#include<iostream>
#include<unordered_map>
#include<string>
#include<optional> 

class Database{
    private:
    // Global memory storage: an unordered_map to store all key-value pairs in the database
    // Private global memory storage for key-value pairs.
    // Direct access is restricted to ensure thread safety and prevent race conditions;
    // all access should be managed through the class's methods.
    std::unordered_map<std::string, std::string> global_memory;

    public:
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