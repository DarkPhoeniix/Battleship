
#include <QDebug>
#include "AI.h"



namespace Battleship {
    std::vector<Coordinate> AI::_placeShip(std::vector<Coordinate>& freeCoordinates, uint8_t shipSize) const {
        Coordinate generatedCoord(0, 0);
        std::vector<Coordinate> badCoords;
        badCoords.reserve(10);
        bool flag = true;

        generatedCoord = freeCoordinates[rand() % (freeCoordinates.size() - 1)];
        auto newShip = _tryToPlaceShip(freeCoordinates, generatedCoord, shipSize);
        if (newShip.size() == 0) {
            flag = false;
        }
        while (!flag) {
            badCoords.push_back(*freeCoordinates.erase(std::find(freeCoordinates.begin(),
                freeCoordinates.end(),
                generatedCoord)));
            generatedCoord = freeCoordinates[rand() % (freeCoordinates.size() - 1)];
            newShip = _tryToPlaceShip(freeCoordinates, generatedCoord, shipSize);
            if (newShip.size() == 0) {
                flag = false;
            }
            else {
                flag = true;
            }
            freeCoordinates.erase(std::lower_bound(freeCoordinates.begin(),
                freeCoordinates.end(),
                generatedCoord));
        }
        while (!badCoords.empty()) {
            freeCoordinates.insert(std::upper_bound(freeCoordinates.begin(),
                freeCoordinates.end(),
                *badCoords.rbegin()),
                *badCoords.rbegin());
            badCoords.pop_back();
        }

        return newShip;
    }

    std::vector<Coordinate>
        AI::_tryToPlaceShip(const std::vector<Coordinate>& availableCoords,
            Coordinate startCoord,
            int sizeOfShip) const
    {
        std::vector<Coordinate> coords = { startCoord };
        auto currentCoord = startCoord;

        auto tryToPlaceShipCoordinate = [this, &coords, &availableCoords, sizeOfShip](const Coordinate& coordinate)
        {
            if (coords.size() < sizeOfShip &&
                _isCoordAvailableForPlacement(availableCoords, coordinate) &&
                std::find(coords.begin(), coords.end(), coordinate) == coords.end()) {
                coords.push_back(coordinate);
            }
        };

        for (int i = 0; i < coords.size() && coords.size() < sizeOfShip && i < sizeOfShip; ++i) {
            std::vector<uint8_t> availableDirection = { 1,2,3,4 };
            while (!availableDirection.empty()) {
                uint8_t randomizer = rand() % availableDirection.size();

                switch (availableDirection[randomizer]) {
                case 1:
                    tryToPlaceShipCoordinate(Coordinate(coords[i].x + 1, coords[i].y));
                    break;
                case 2:
                    tryToPlaceShipCoordinate(Coordinate(coords[i].x - 1, coords[i].y));
                    break;
                case 3:
                    tryToPlaceShipCoordinate(Coordinate(coords[i].x, coords[i].y + 1));
                    break;
                case 4:
                    tryToPlaceShipCoordinate(Coordinate(coords[i].x, coords[i].y - 1));
                    break;
                default:
                    break;
                }

                availableDirection.erase(std::find(availableDirection.begin(),
                                                   availableDirection.end(),
                                                   availableDirection[randomizer]));
            }
        }

        return (coords.size() == sizeOfShip) ?
                    coords :
                    std::vector<Coordinate>{};
    }

    bool AI::_isCoordAvailableForPlacement(const std::vector<Coordinate>& coordinates,
                                           Coordinate coordinate) const {
        return std::binary_search(coordinates.begin(),
                                  coordinates.end(),
                                  coordinate);
    }

    void AI::_removeAdjacentCoordinates(std::vector<Coordinate>& container,
                                        Coordinate coordinate) {
        auto eraser = [&container](Coordinate coordinate) {
            auto toErase = std::find(container.begin(), container.end(), coordinate);
            if (toErase != container.end()) {
                container.erase(toErase);
            }
        };

//        eraser(Coordinate(coordinate.x, coordinate.y));
        eraser(Coordinate(coordinate.x + 1, coordinate.y));
        eraser(Coordinate(coordinate.x - 1, coordinate.y));
        eraser(Coordinate(coordinate.x, coordinate.y + 1));
        eraser(Coordinate(coordinate.x, coordinate.y - 1));
        eraser(Coordinate(coordinate.x + 1, coordinate.y + 1));
        eraser(Coordinate(coordinate.x + 1, coordinate.y - 1));
        eraser(Coordinate(coordinate.x - 1, coordinate.y + 1));
        eraser(Coordinate(coordinate.x - 1, coordinate.y - 1));
    }

    bool AI::_checkIsHuntingPhase(TileState prevTileStatus) {
        if (prevTileStatus == (TileState::WasShot |
                               TileState::ShipAfloat |
                               TileState::ShipSunk)) {
            _isHuntingPhase = false;
        } else if (prevTileStatus == (TileState::WasShot |
                                      TileState::ShipAfloat)) {
            _isHuntingPhase = true;
        }

        return _isHuntingPhase;
    }

    Coordinate AI::_generateRandomAttack() {
        qDebug().verbosity(QDebug::MaximumVerbosity) << "AI generates turn in mode: random";
        Coordinate attackCoordinate = Coordinate(255, 255);
        int displacementFactor = 0;

        if (_availableCoords.size() > 1) {
            displacementFactor = rand() % (_availableCoords.size() - 1);
        }
        attackCoordinate = *(_availableCoords.begin() + displacementFactor);

        return attackCoordinate;
    }

    Coordinate AI::_generateDestroyingAttack() {
        qDebug().verbosity(QDebug::MaximumVerbosity) << "AI generates turn in mode: hunt";
        std::vector<Coordinate>::iterator attackCoordinate = _availableCoords.end();

        auto searcher = [&container = _availableCoords](Coordinate coordinate)
        {
            return std::find(container.begin(),
                             container.end(),
                             coordinate);
        };

        auto isHitShip = [](std::pair<Coordinate, TileState> tile)
        {
            return tile.second == (TileState::WasShot | TileState::ShipAfloat);
        };

        std::vector<std::pair<Coordinate, TileState>>::iterator
            lastHitIter = std::find_if(_prevShots.begin(),
                                       _prevShots.end(),
                                       isHitShip);
        while (attackCoordinate == _availableCoords.end()) {
            std::vector<uint8_t> availableDirection = { 1,2,3,4 };
            while (!availableDirection.empty() && attackCoordinate == _availableCoords.end()) {
                uint8_t randomizer = rand() % availableDirection.size();
                switch (availableDirection[randomizer]) {
                case 1:
                    attackCoordinate = searcher(Coordinate(lastHitIter->first.x + 1,
                                                           lastHitIter->first.y));
                    break;
                case 2:
                    attackCoordinate = searcher(Coordinate(lastHitIter->first.x - 1,
                                                           lastHitIter->first.y));
                    break;
                case 3:
                    attackCoordinate = searcher(Coordinate(lastHitIter->first.x,
                                                           lastHitIter->first.y + 1));
                    break;
                case 4:
                    attackCoordinate = searcher(Coordinate(lastHitIter->first.x,
                                                           lastHitIter->first.y - 1));
                    break;
                default:
                    break;
                }
                availableDirection.erase(std::find(availableDirection.begin(),
                                                   availableDirection.end(),
                                                   availableDirection[randomizer]));
            }
            lastHitIter = std::find_if(std::next(lastHitIter),_prevShots.end(),isHitShip);
        }

        return *attackCoordinate;
    }



    AI::AI()
        : _isHuntingPhase(false)
    {
        for (uint8_t i = 0; i < Map::MAP_SIZE; ++i) {
            for (uint8_t j = 0; j < Map::MAP_SIZE; ++j) {
                _availableCoords.push_back(Coordinate(i, j));
            }
        }
    }

    AI::AI(const AI& copy) {
        if (this != &copy) {
            _availableCoords = copy._availableCoords;
            _prevShots = copy._prevShots;
            _isHuntingPhase = copy._isHuntingPhase;
        }
    }

    AI::AI(AI&& obj) noexcept {
        if (this != &obj) {
            _availableCoords = std::move(obj._availableCoords);
            _prevShots = std::move(obj._prevShots);
            _isHuntingPhase = obj._isHuntingPhase;
        }
    }

    AI::~AI()
    {   }



    AI& AI::operator=(const AI& copy) {
        if (this != &copy) {
            _availableCoords = copy._availableCoords;
            _prevShots = copy._prevShots;
            _isHuntingPhase = copy._isHuntingPhase;
        }
        return *this;
    }

    AI& AI::operator=(AI&& obj) noexcept {
        if (this != &obj) {
            _availableCoords = std::move(obj._availableCoords);
            _prevShots = std::move(obj._prevShots);
            _isHuntingPhase = obj._isHuntingPhase;
        }
        return *this;
    }



    void AI::approveAttack(Coordinate coordinate, TileState tileStatus)
    {
        _prevShots.emplace_back(coordinate, tileStatus);

        if(!_checkIsHuntingPhase(tileStatus)) {
            for (auto& item : _prevShots) {
                if ((item.second & (TileState::ShipAfloat | TileState::WasShot)) == (TileState::ShipAfloat | TileState::WasShot)) {
                    item.second |= TileState::ShipSunk;
                    _removeAdjacentCoordinates(_availableCoords, item.first);
                }
            }
        }

        auto toErase = std::find(_availableCoords.begin(), _availableCoords.end(), coordinate);
        if (toErase != _availableCoords.end()) {
            _availableCoords.erase(toErase);
        }
    }

    Coordinate AI::generateAttack(TileState prevTileStatus) noexcept
    {
        auto coord = _availableCoords.begin();
        auto toRet = *coord;
        try {
            if (!_prevShots.empty()) {
                _prevShots.rbegin()->second |= (prevTileStatus &
                                                (TileState::WasShot |
                                                 TileState::ShipAfloat));
            }

            if (!_isHuntingPhase) {
                coord = std::find(_availableCoords.begin(),
                                  _availableCoords.end(),
                                  _generateRandomAttack());
            }
            else {
                coord = std::find(_availableCoords.begin(),
                                  _availableCoords.end(),
                                  _generateDestroyingAttack());
            }

            toRet = *coord;
        } catch(...) {
            toRet = ErrCoord;
        }

        return toRet;
    }

    Battleship::Map AI::generateMap()
    {
        Map generatedMap;
        std::vector<Coordinate> freeCoords(100);
        std::vector<int> ships = { 1, 2, 3, 4 };
        for (uint8_t i = 0; i < Map::MAP_SIZE; ++i) {
            for (uint8_t j = 0; j < Map::MAP_SIZE; ++j) {
                freeCoords[i * 10 + j] = Coordinate(i, j);
            }
        }


        for (uint8_t i = 0; i < ships.size(); ++i) {
            for (uint8_t j = 0; j < ships[i]; ++j) {
                auto newShip = _placeShip(freeCoords, 5 - ships[i]);

                for (auto& item : newShip) {
                    generatedMap.setTile(std::move(item), TileState::ShipAfloat);
                    _removeAdjacentCoordinates(freeCoords, item);
                }
            }
        }
        return generatedMap;
    }
}
