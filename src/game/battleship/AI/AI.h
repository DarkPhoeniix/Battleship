
#include "../map.h"

#include <iterator>
#include <ctime>
#include <list>



namespace Battleship {
    class AI
    {
    private:
        std::vector<Coordinate> _placeShip(std::vector<Coordinate>& freeCoordinates, uint8_t shipSize) const;
        std::vector<Coordinate> _availableCoords;
        std::vector<std::pair<Coordinate, TileState>> _prevShots;
        bool _isSearchingPhase;

        std::vector<Coordinate> _tryToPlaceShip(const std::vector<Coordinate>& availableCoords,
                                                Coordinate startCoord,
                                                int sizeOfShip) const;
        bool _isCoordAvailableForPlacement(const std::vector<Coordinate>& coordinates, Coordinate coordinate) const;
        void _removeAdjacentCoordinates(std::vector<Coordinate>& container, Coordinate coordinate);
        bool _checkIsSearchingPhase(TileState prevTileStatus);
        Coordinate _generateRandomAttack();
        Coordinate _generateDestroyingAttack();

    public:
        AI();
        AI(const AI& copy);
        AI(AI&& obj) noexcept;
        ~AI();

        AI& operator=(const AI& copy);
        AI& operator=(AI&& obj) noexcept;

        Coordinate generateAttack(TileState prevTileStatus = TileState::Empty);
        Battleship::Map generateMap();
    };
}
