#include "wavefunctioncollapse/state.h"

namespace WFC {

State::State(int id, std::string name, std::unordered_set<int> possible_neighbours): id(id), name(name), possible_neighbours(possible_neighbours)
{

}

} // namespace WFC
