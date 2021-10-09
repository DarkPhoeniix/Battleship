
#include "AI.h"



namespace Battleship {
    std::vector<Coordinate>
        AI::_tryToPlaceShip(const std::vector<Coordinate>& availableCoords,
            Coordinate startCoord,
            int sizeOfShip) const
    {
        std::vector<Coordinate> coords = { startCoord };
        auto currentCoord = startCoord;
        auto isFound = [&coords, &availableCoords](const auto& item, int i) -> bool
        { return std::binary_search(availableCoords.begin(),
            availableCoords.end(),
            item);
        };

        for (int i = 0; i < coords.size() && i < sizeOfShip; ++i) {
            std::vector<uint8_t> availableDirection = { 1,2,3,4 };
            while (!availableDirection.empty()) {
                uint8_t randomizer = rand() % availableDirection.size();
                switch (availableDirection[randomizer]) {
                case 1:
                    if (coords.size() < sizeOfShip &&
                        isFound(Coordinate(coords[i].x + 1, coords[i].y), i)) {
                        coords.emplace_back(coords[i].x + 1, coords[i].y);
                    }
                    break;
                case 2:
                    if (coords.size() < sizeOfShip &&
                        isFound(Coordinate(coords[i].x - 1, coords[i].y), i)) {
                        coords.emplace_back(coords[i].x - 1, coords[i].y);
                    }
                    break;
                case 3:
                    if (coords.size() < sizeOfShip &&
                        isFound(Coordinate(coords[i].x, coords[i].y + 1), i)) {
                        coords.emplace_back(coords[i].x, coords[i].y + 1);
                    }
                    break;
                case 4:
                    if (coords.size() < sizeOfShip &&
                        isFound(Coordinate(coords[i].x, coords[i].y - 1), i)) {
                        coords.emplace_back(coords[i].x, coords[i].y - 1);
                    }
                    break;
                default:
                    break;
                }
                availableDirection.erase(std::find(availableDirection.begin(), availableDirection.end(), availableDirection[randomizer]));
            }
        }

        return (coords.size() == sizeOfShip) ? coords : std::vector<Coordinate>{};
    }

    bool AI::_checkIsSearchingPhase(TileState prevTileStatus) {
        if (prevTileStatus == (TileState::Empty |
            TileState::WasShot |
            TileState::ShipAfloat)) {
            _isSearchingPhase = false;
        }
        else if (prevTileStatus == (TileState::Empty |
            TileState::WasShot |
            TileState::ShipAfloat |
            TileState::ShipSunk)) {
            _isSearchingPhase = true;
        }

        return _isSearchingPhase;
    }



    AI::AI()
        : _isSearchingPhase(true)
    {
        for (uint8_t i = 0; i < Map::MAP_SIZE; ++i) {
            for (uint8_t j = 0; j < Map::MAP_SIZE; ++j) {
                _availableCoords.push_back(Coordinate(i, j));
            }
        }
    }

    AI::~AI()
    {   }



    Coordinate AI::generateAttack(TileState prevTileStatus)
    {
        auto coord = _availableCoords.begin();
        if (!_prevShots.empty()) {
            _prevShots.rbegin()->second |= (prevTileStatus & (TileState::WasShot | TileState::ShipAfloat));
        }

        if (_checkIsSearchingPhase(prevTileStatus)) {

            for (auto& item : _prevShots) {
                if ((item.second) == (TileState::ShipAfloat | TileState::WasShot)) {
                    item.second |= TileState::ShipSunk;
                    auto toErase = std::find(_availableCoords.begin(), _availableCoords.end(), item.first);
                    if (toErase != _availableCoords.end()) {
                        _availableCoords.erase(toErase);
                    }
                    toErase = std::find(_availableCoords.begin(), _availableCoords.end(), Coordinate(item.first.x + 1, item.first.y));
                    if (toErase != _availableCoords.end()) {
                        _availableCoords.erase(toErase);
                    }
                    toErase = std::find(_availableCoords.begin(), _availableCoords.end(), Coordinate(item.first.x - 1, item.first.y));
                    if (toErase != _availableCoords.end()) {
                        _availableCoords.erase(toErase);
                    }
                    toErase = std::find(_availableCoords.begin(), _availableCoords.end(), Coordinate(item.first.x, item.first.y + 1));
                    if (toErase != _availableCoords.end()) {
                        _availableCoords.erase(toErase);
                    }
                    toErase = std::find(_availableCoords.begin(), _availableCoords.end(), Coordinate(item.first.x, item.first.y - 1));
                    if (toErase != _availableCoords.end()) {
                        _availableCoords.erase(toErase);
                    }
                    toErase = std::find(_availableCoords.begin(), _availableCoords.end(), Coordinate(item.first.x + 1, item.first.y + 1));
                    if (toErase != _availableCoords.end()) {
                        _availableCoords.erase(toErase);
                    }
                    toErase = std::find(_availableCoords.begin(), _availableCoords.end(), Coordinate(item.first.x + 1, item.first.y - 1));
                    if (toErase != _availableCoords.end()) {
                        _availableCoords.erase(toErase);
                    }
                    toErase = std::find(_availableCoords.begin(), _availableCoords.end(), Coordinate(item.first.x - 1, item.first.y + 1));
                    if (toErase != _availableCoords.end()) {
                        _availableCoords.erase(toErase);
                    }
                    toErase = std::find(_availableCoords.begin(), _availableCoords.end(), Coordinate(item.first.x - 1, item.first.y - 1));
                    if (toErase != _availableCoords.end()) {
                        _availableCoords.erase(toErase);
                    }
                }
            }
            int displacementFactor = rand() % (_availableCoords.size() - 1);

            coord = _availableCoords.begin() + displacementFactor;
        }
        else {
            std::vector<std::pair<Coordinate, TileState>>::iterator
                lastHitIter = std::find_if(_prevShots.begin(),
                    _prevShots.end(),
                    [](const auto& item) -> bool
                    { return (item.second) ==
                    (TileState::ShipAfloat | TileState::WasShot); });
            while (true) {
                coord = std::find(_availableCoords.begin(),
                    _availableCoords.end(),
                    Coordinate(lastHitIter->first.x + 1,
                        lastHitIter->first.y));
                if (coord != _availableCoords.end()) {
                    break;
                }
                coord = std::find(_availableCoords.begin(),
                    _availableCoords.end(),
                    Coordinate(lastHitIter->first.x - 1,
                        lastHitIter->first.y));
                if (coord != _availableCoords.end()) {
                    break;
                }
                coord = std::find(_availableCoords.begin(),
                    _availableCoords.end(),
                    Coordinate(lastHitIter->first.x,
                        lastHitIter->first.y + 1));
                if (coord != _availableCoords.end()) {
                    break;
                }
                coord = std::find(_availableCoords.begin(),
                    _availableCoords.end(),
                    Coordinate(lastHitIter->first.x,
                        lastHitIter->first.y - 1));
                if (coord != _availableCoords.end()) {
                    break;
                }
                lastHitIter = std::find_if(std::next(lastHitIter),
                    _prevShots.end(),
                    [](const auto& item)
                    { return (item.second) ==
                    (TileState::ShipAfloat | TileState::WasShot); });
            }
        }
        _prevShots.emplace_back(*coord, TileState::WasShot);
        auto toRet = *coord;
        _availableCoords.erase(coord);
        return toRet;
    }

    Battleship::Map AI::generateMap() const
    {
        Map generatedMap;
        std::vector<Coordinate> freeCoords(100);
        std::vector<int> ships = { 1, 2, 3, 4 };
        for (uint8_t i = 0; i < Map::MAP_SIZE; ++i) {
            for (uint8_t j = 0; j < Map::MAP_SIZE; ++j) {
                freeCoords[i * 10 + j] = Coordinate(i, j);
            }
        }

        Coordinate generatedCoord(0, 0);
        std::vector<Coordinate> badCoords;
        badCoords.reserve(10);
        bool flag = true;
        for (int i = 0; i < ships.size(); ++i) {
            for (int j = 0; j < ships[i]; ++j) {
                generatedCoord = freeCoords[rand() % (freeCoords.size() - 1)];
                auto newShip = _tryToPlaceShip(freeCoords, generatedCoord, 4 - i);
                if (newShip.size() == 0) {
                    flag = false;
                }
                while (!flag) {
                    badCoords.push_back(*freeCoords.erase(std::find(freeCoords.begin(),
                        freeCoords.end(),
                        generatedCoord)));
                    generatedCoord = freeCoords[rand() % (freeCoords.size() - 1)];
                    newShip = _tryToPlaceShip(freeCoords, generatedCoord, i + 1);
                    if (newShip.size() == 0) {
                        flag = false;
                    }
                    else {
                        flag = true;
                    }
                    freeCoords.erase(std::lower_bound(freeCoords.begin(),
                        freeCoords.end(),
                        generatedCoord));
                }

                for (auto& item : newShip) {
                    generatedMap.setTile(std::move(item), TileState::ShipAfloat);

                    auto toErase = std::find(freeCoords.begin(), freeCoords.end(), item);
                    if (toErase != freeCoords.end()) {
                        freeCoords.erase(toErase);
                    }
                    toErase = std::find(freeCoords.begin(), freeCoords.end(), Coordinate(item.x + 1, item.y));
                    if (toErase != freeCoords.end()) {
                        freeCoords.erase(toErase);
                    }
                    toErase = std::find(freeCoords.begin(), freeCoords.end(), Coordinate(item.x - 1, item.y));
                    if (toErase != freeCoords.end()) {
                        freeCoords.erase(toErase);
                    }
                    toErase = std::find(freeCoords.begin(), freeCoords.end(), Coordinate(item.x, item.y + 1));
                    if (toErase != freeCoords.end()) {
                        freeCoords.erase(toErase);
                    }
                    toErase = std::find(freeCoords.begin(), freeCoords.end(), Coordinate(item.x, item.y - 1));
                    if (toErase != freeCoords.end()) {
                        freeCoords.erase(toErase);
                    }
                    toErase = std::find(freeCoords.begin(), freeCoords.end(), Coordinate(item.x + 1, item.y + 1));
                    if (toErase != freeCoords.end()) {
                        freeCoords.erase(toErase);
                    }
                    toErase = std::find(freeCoords.begin(), freeCoords.end(), Coordinate(item.x + 1, item.y - 1));
                    if (toErase != freeCoords.end()) {
                        freeCoords.erase(toErase);
                    }
                    toErase = std::find(freeCoords.begin(), freeCoords.end(), Coordinate(item.x - 1, item.y + 1));
                    if (toErase != freeCoords.end()) {
                        freeCoords.erase(toErase);
                    }
                    toErase = std::find(freeCoords.begin(), freeCoords.end(), Coordinate(item.x - 1, item.y - 1));
                    if (toErase != freeCoords.end()) {
                        freeCoords.erase(toErase);
                    }
                }
            }
            while (!badCoords.empty()) {
                freeCoords.insert(std::upper_bound(freeCoords.begin(),
                    freeCoords.end(),
                    badCoords[0]),
                    badCoords[0]);
            }
        }
        return generatedMap;
    }
}
