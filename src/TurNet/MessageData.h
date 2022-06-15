#pragma once
#include <vector>

// store plain old data in a vector.
class MessageData
{
public:
    void operator<<(const std::vector<char>& data);
    void operator>>(std::vector<char>& data);
private:
    std::vector<int> myData;
};
