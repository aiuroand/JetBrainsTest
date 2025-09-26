#ifndef CMAPWRAPPER_H
#define CMAPWRAPPER_H

#include<map>
#include<mutex>
#include<optional>

// Definition for an array of bytes
using byteVector = std::vector<uint8_t>;

class CMapWrapper {
    std::map<byteVector, byteVector> m_Map;
    std::mutex m_Mutex;
  public:
    /** 
     * @brief Inserts a key in the map
     *  
     * A method for insertion, which guarantees exclusive access to m_Map member
     * 
     * @param key   - The key to insert
     * @param value - The value to associate with the key
     * @return      - true if the insertion succeeded
     *              - false otherwise
     */ 
    bool insert (const byteVector& key, const byteVector& value) {
      std::unique_lock<std::mutex> lock(m_Mutex);
      auto [iter, status] = m_Map.insert({key, value});
      return status;
    }

    /** 
     * @brief Searches for a key in the map
     *  
     * A method for search, which guarantees exclusive access to m_Map member 
     * 
     * @param key - The key to search for
     * @return    - The associated to a given key value if the key exists
     *            - std::nullopt if the key does not exist
     */    
    std::optional<byteVector> find (const byteVector& key) {
      std::unique_lock<std::mutex> lock(m_Mutex);
      auto iter = m_Map.find(key);
      if (iter != m_Map.end())
        return iter->second;
      return std::nullopt;
    }
};

#endif // CMAPWRAPPER_H