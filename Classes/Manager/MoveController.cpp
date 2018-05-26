/*
*  @file     MoveController.cpp
*  @brief    ¸÷ÖÖ±øµÄÒÆ¶¯¿ØÖÆ
*/

#include "MoveController.h"
#include <time.h>

USING_NS_CC;

MoveController* MoveController::createWithGameScene(GameScene* gameScene)
{
    MoveController* move_controller = new MoveController();
    if (move_controller->initWithGameScene(gameScene))
    {
        move_controller->autorelease();
        return move_controller;
    }

    CC_SAFE_DELETE(move_controller);
    return nullptr;
}

bool MoveController::initWithGameScene(GameScene* gameScene)
{
    _selectedSoldiers = gameScene->getSelectedSoldiers();
    _gameScene = gameScene;
    _isFirstMove = true;
}

void MoveController::selectSoldiersWithMouse(cocos2d::Vec2 mouseDownPoint, cocos2d::Vec2 mouseUpPoint)
{
    _selectedSoldiers->clear();
    float rect_width = fabs(mouseUpPoint.x - mouseDownPoint.x);
    float rect_height = fabs(mouseUpPoint.y - mouseDownPoint.y);
    Rect rect(MIN(mouseDownPoint.x, mouseUpPoint.x), MIN(mouseDownPoint.y, mouseUpPoint.y), rect_width, rect_height);
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (rect.containsPoint(soldier->getPosition()))
        {
            soldier->setIsSelected(true);
            _selectedSoldiers->pushBack(soldier);
        }
    }
}

void MoveController::selectSoldiersWithName(const std::string& name)
{
    _selectedSoldiers->clear();
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (soldier->getName() == name)
        {
            soldier->setIsSelected(true);
            _selectedSoldiers->pushBack(soldier);
        }
    }
}

void MoveController::setDestination(cocos2d::Vec2 position)
{
    // ¼ì²âÊÇ·ñÕÏ°­
    if (_gameScene->isCollision(position))
    {
        return;
    }

    for (auto& soldier : *_selectedSoldiers)
    {
        soldier->setDestination(position);
        soldier->setGetDestination(false);
    }

    _isFirstMove = true;
}

bool MoveController::willHitInFiveTiles(Vec2 nowPosition, Vec2 direction)
{
    float tile_size = _gameScene->getTileMap()->getTileSize().width;
    if (_isFirstMove)
    {
        for (int i = 0; i < 5; ++i)
        {
            nowPosition += direction * tile_size;
            if (_gameScene->isCollision(nowPosition))
            {
                return true;
            }
        }
    }
    else if (_gameScene->isCollision(nowPosition + direction * tile_size * 5))
    {
        return true;
    }
    
    return false;
}

Vec2 MoveController::changeDirection(cocos2d::Vec2 nowPosition, cocos2d::Vec2 direction)
{
    float tile_size = _gameScene->getTileMap()->getTileSize().width;
    bool store_is_first_move = _isFirstMove;
    _isFirstMove = true;

//================ To Do : ´Ë´¦µÄ×ªÏòbugÊ®·ÖÑÏÖØ,ÄÎºÎ±¾ÈËÄÜÁ¦ÓÐÏÞ,ÔÝÊ±Ïë²»³öºÃµÄ·½·¨ ==================
    Vec2 up(0, 1);
    Vec2 down(0, -1);
    Vec2 right(1, 0);
    Vec2 left(-1, 0);
    if (direction.x > 0)
    {
        if (direction.y > 0)                                         //right up
        {
            if (!willHitInFiveTiles(nowPosition, up))       // up
                return up;
            else if (!willHitInFiveTiles(nowPosition, right))   // right
                return right;
            else if (!willHitInFiveTiles(nowPosition, left))  //left
                return left;
            else if (!willHitInFiveTiles(nowPosition, down))   // down
                return down;
            else
                return -direction;
        }
        else if (direction.y < 0)                                   // right down
        {
            if (!willHitInFiveTiles(nowPosition, down))     // down
                return down;
            else if (!willHitInFiveTiles(nowPosition, right)) // right
                return right;
            else if (!willHitInFiveTiles(nowPosition, left)) // left
                return left;
            else if (!willHitInFiveTiles(nowPosition, up)) // up
                return up;
            else
                return -direction;
        }
        else                                                         // right
        {
            if (!willHitInFiveTiles(nowPosition, up))        // up
                return up;
            else if (!willHitInFiveTiles(nowPosition, down))  // down
                return down;
            else
                return -direction;
        }
    }
    else if (direction.x < 0)                                            
    {
        if (direction.y > 0)                                               // left up
        {
            if (!willHitInFiveTiles(nowPosition, up))     // up
                return up;
            else if (!willHitInFiveTiles(nowPosition, left)) // left
                return left;
            else if (!willHitInFiveTiles(nowPosition, right)) // right
                return right;
            else if (!willHitInFiveTiles(nowPosition, down)) // down
                return down;
            else
                return -direction;
        }
        else if (direction.y < 0)                                         //left down
        {
            if (!willHitInFiveTiles(nowPosition, down))     // down
                return down;
            else if (!willHitInFiveTiles(nowPosition, left)) // left
                return left;
            else if (!willHitInFiveTiles(nowPosition, right)) // right
                return right;
            else if (!willHitInFiveTiles(nowPosition, up)) // up
                return up;
            else
                return -direction;
        }
        else                                                            //left
        {
            if (!willHitInFiveTiles(nowPosition, down))     // down
                return down;
            else if (!willHitInFiveTiles(nowPosition, up)) // up
                return up;
            else
                return -direction;
        }
    }
    else                                               // up or down                          
    {
        if (!willHitInFiveTiles(nowPosition, right)) // right
            return right;
        else if (!willHitInFiveTiles(nowPosition, left)) // left
            return left;
        else
            return -direction;
    }

    _isFirstMove = store_is_first_move;
}

void MoveController::moveSoldiers()
{
    static clock_t preT = clock();
    clock_t nowT = clock();
    float interval = nowT - preT;
    preT = nowT;
    for (auto& soldier : *(_gameScene->getSoldiers()))
    {
        if (!soldier->getIfGetDestination())
        {
            Vec2 nowPosition = soldier->getPosition();
            Vec2 destination = soldier->getDestination();
            Vec2 direction = destination - nowPosition;
            direction.normalize();
            float distance = destination.distance(nowPosition);
//================ To Do : ´Ë´¦µÄ×ªÏòbugÊ®·ÖÑÏÖØ,ÄÎºÎ±¾ÈËÄÜÁ¦ÓÐÏÞ,ÔÝÊ±Ïë²»³öºÃµÄ·½·¨ ==================
            if (willHitInFiveTiles(nowPosition, direction))
            {
                if (soldier->getSecondDirection() != Vec2::ZERO)
                {
                    soldier->setSecondDirection(changeDirection(nowPosition, direction));
                }
                direction = soldier->getSecondDirection();
            }
            else
            {
                soldier->setSecondDirection(Vec2::ZERO);
            }

            Vec2 move = soldier->getUnitSpeed() * interval * direction;
            if (move.length > distance)
            {
                soldier->moveTo(destination);
                soldier->setGetDestination(true);
                _isFirstMove = true;
                return;
            }
            
            // ¼ì²âÅö×²
            if (_gameScene->isCollision(move + nowPosition))
            {
                return;
            }
            soldier->moveTo(move + nowPosition);

            _isFirstMove = false;
        }
    }
}