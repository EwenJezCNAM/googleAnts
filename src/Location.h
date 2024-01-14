#ifndef LOCATION_H_
#define LOCATION_H_
/*
    struct for representing locations in the grid.
*/
struct Location
{
    int row, col;

    Location()
    {
        row = col = 0;
    };

    Location(int r, int c)
    {
        row = r;
        col = c;
    };

    // equality operator
    bool operator==(const Location &other) const
	{
		return row == other.row && col == other.col;
	};

    // inequality operator
    bool operator!=(const Location &other) const
	{
		return !(*this == other);
	};
};

#endif //LOCATION_H_
