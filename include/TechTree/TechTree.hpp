#ifndef TECHTREE_H
#define TECHTREE_H

#include <vector>
#include "sc2api/sc2_unit.h"

namespace suboo {

    using UnitId = sc2::UNIT_TYPEID;

    /**
     * @brief 
     * 
     */
    struct Unit {
        UnitId type;                            // Unit id
        std::string name;                       // Unit name
        int mineral_cost;                       // Unit mineral costs
        int vespene_cost;                       // Unit vespene costs
        int food_cost;                          // Unit food costs (can be positive)

        UnitId builder;                         // Unit required to build
        std::vector<UnitId> requirements;       // Tech requirement

        int production_time;                    // Unit creation time   
        int travel_time;                        // Unit travel time
        
        enum Status {
            BUSY,
            TRAVELLING,
            CONSUMING
        };
        Status acton_status;                    // Unit action behavior
    };

    /**
     * @brief 
     * 
     */
    struct UnitInstance {
        Unit unittype;
        enum UnitState {
            BUILDING,
            BUSY,
            MINING_MINERALS,
            MINING_VESPENE,
            FREE
        };
        UnitState state;
        int time_to_free;
    };

    /**
     * @brief Singleton class used to query information about units
     */
    class TechTree {
        TechTree();
        std::unordered_map<UnitId, Unit> units;

        public:

        /**
         * @brief Get the Tech Tree object
         * @return const TechTree& 
         */
        static TechTree & getTechTree();
        void Test();
    };
} // namespace suboo

#endif // TECHTREE_H