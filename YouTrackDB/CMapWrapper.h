#ifndef CMAPWRAPPER_H
#define CMAPWRAPPER_H

#include<map>
#include<mutex>
#include<optional>

using byteVector = std::vector<uint8_t>;

class CMapWrapper {
    std::map<byteVector, byteVector> m_Map;
    std::mutex m_Mutex;
  public:
    bool insert (const byteVector& key, const byteVector& value) {
      std::unique_lock<std::mutex> lock(m_Mutex);
      auto [iter, status] = m_Map.insert({key, value});
      return status;
    }
    
    std::optional<byteVector> find (const byteVector& key) {
      std::unique_lock<std::mutex> lock(m_Mutex);
      auto iter = m_Map.find(key);
      if (iter != m_Map.end())
        return iter->second;
      return std::nullopt;
    }
};

#endif // CMAPWRAPPER_H