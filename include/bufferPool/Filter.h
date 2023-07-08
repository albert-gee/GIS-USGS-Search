#ifndef GIS_FILTER_H
#define GIS_FILTER_H

#include <string>
#include <set>

using namespace std;

struct Filter{
public:
    Filter(){
        fillWater();
        fillPop();
        fillStructure();
    }

    const set<string> &getPop() const {
        return pop;
    }

    const set<string> &getWater() const {
        return water;
    }

    const set<string> &getStructure() const {
        return structure;
    }

private:
    set<string> pop;
    set<string> water;
    set<string> structure;


    void fillWater(){
        water.insert("Arroyo");
        water.insert("Bay");
        water.insert("Bend");
        water.insert("Canal");
        water.insert("Channel");
        water.insert("Falls");
        water.insert("Glacier");
        water.insert("Gut");
        water.insert("Harbor");
        water.insert("Lake");
        water.insert("Rapids");
        water.insert("Reservoir");
        water.insert("Sea");
        water.insert("Spring");
        water.insert("Stream");
        water.insert("Swamp");
        water.insert("Well");
    }

    void fillPop(){
        pop.insert("Populated Place");
    }

    void fillStructure(){
        structure.insert("Dam");
        structure.insert("Hospital");
        structure.insert("Levee");
        structure.insert("Park");
        structure.insert("Post Office");
        structure.insert("School");
        structure.insert("Tower");
        structure.insert("Tunnel");
    }
};

#endif //GIS_FILTER_H
