#ifndef WFC_STATE_H
#define WFC_STATE_H

#include <string>
#include <unordered_set>

namespace WFC {

class State
{
public:
    State(int id, std::string name, std::unordered_set<int> possible_neighbours);
    int id;
    std::string name;
    std::unordered_set<int> possible_neighbours;
};

} // namespace WFC

#endif // WFC_STATE_H
