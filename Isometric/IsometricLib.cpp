#include <iostream>

class isotile
{
private:
    int x, y, z;
    int sigma() const
    {
        return x + y;
    }
    int delta() const
    {
        return x - y;
    }
public:
    bool operator<(const isotile &compare_isotile)
    {
        //sigma determines the "row" that the tile is in, colum
        if(this->sigma() == compare_isotile.sigma())
        {
            return this->z < compare_isotile.z;
        }
        return this->sigma() > compare_isotile.sigma();
    }
};
