
#include "../map.h"

#include <iterator>
#include <ctime>
#include <list>



namespace Battleship {
    class AI
    {
    private:
        std::vector<Coordinate> _availableCoords;
        std::vector<std::pair<Coordinate, TileState>> _prevShots;
        bool _isSearchingPhase;

        std::vector<Coordinate> _tryToPlaceShip(const std::vector<Coordinate>& availableCoords,
                                                Coordinate startCoord,
                                                int sizeOfShip) const;
        bool _checkIsSearchingPhase(TileState prevTileStatus);


    public:
        AI();
        ~AI();

        Coordinate generateAttack(TileState prevTileStatus = TileState::Empty);
        Battleship::Map generateMap() const;
    };
}

