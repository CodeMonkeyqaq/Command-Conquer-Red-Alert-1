#ifndef __Game_SCENE_H__
#define __Game_SCENE_H__

#include "cocos2d.h"
#include "Manager\GameManager.h"
#include "Data\Building.h"
#include "Data\UnitData.h"

typedef enum
{
    BASE_TAG,
    POWER_PLANT_TAG,
    MINE_TAG,
    BARRACKS_TAG,
    CAR_FACTORY_TAG,
    INFANTRY_TAG,
    DOG_TAG,
    TANK_TAG
}Tag;

class GameScene : public cocos2d::Layer
{
private:
    cocos2d::TMXTiledMap* _tileMap;
    cocos2d::TMXLayer* _ground;
    cocos2d::Point _cursorPosition{ 0,0 };  // C++ 11 允许这样初始化
    void scrollMap();

    cocos2d::Vector<Unit*> _selectedSoldiers;
    cocos2d::Vector<Unit*> _soldiers;
    cocos2d::Vector<Building*> _buildings;

    Manager* _manager;

public:
    CC_SYNTHESIZE(int, _money, Money);

    CC_SYNTHESIZE(int, _power, Power);

    CC_SYNTHESIZE(int, _barracksNum, BarracksNum);

    CC_SYNTHESIZE(int, _mineNum, MineNum);

    CC_SYNTHESIZE(int, _powerPlantNum, PowerPlantNum);

    CC_SYNTHESIZE(int, _carFactoryNum, CarFactoryNum);

    CC_SYNTHESIZE(cocos2d::Vec2, _carFactoryPosition, CarFactoryPosition);

    CC_SYNTHESIZE(cocos2d::Vec2, _barracksPosition, BarracksPosition);

	static cocos2d::Scene* createScene();

	virtual bool init();

    void update(float time);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void menuBackCallback(Ref *pSender);

    /**
    * @brief getSelectedSoldiers
    * @return the address of _selectedSoldiers
    */
    cocos2d::Vector<Unit*>* getSelectedSoldiers();

    /**
    * @brief getSoldiers
    * @return the address of _soldiers
    */
    cocos2d::Vector<Unit*>* getSoldiers();

    /**
    * @brief getBuildings
    * @return the address of _buildings
    */
    cocos2d::Vector<Building*>* getBuildings();
   
    /**
    * @brief addMoney
    * @param 增加的钱
    * @return void
    */
    void addMoney(int money);

    /**
    * @brief decreaseMoney
    * @param 减少的钱
    * @return void
    */
    void decreaseMoney(int money);

    /**
    * @brief addPower 增加100电
    * @return void
    */
    void addPower();

    /**
    * @brief decreasePower
    * @param 减少的电
    * @return void
    */
    void decreasePower(int power);

    /*
    * @brief 电厂数量加一
    * @return void
    */
    void addPowerPlant() { _powerPlantNum++; }

    /*
    * @brief 电厂数量减一
    * @return void
    */
    void decreasePowerPlant() { _powerPlantNum--; }

    /*
    * @brief 车厂数量加一
    * @return void
    */
    void addCarFactory() { _carFactoryNum++; }

    /*
    * @brief 车厂数量减一
    * @return void
    */
    void decreaseCarFactory() { _carFactoryNum--; }

    /*
    * @brief 兵营数量加一
    * @return void
    */
    void addBarracks() { _barracksNum++; }

    /*
    * @brief 兵营数量减一
    * @return void
    */
    void decreaseBarracks() { _barracksNum--; }

    /*
    * @brief 矿厂数量加一
    * @return void
    */
    void addMine() { _mineNum++; }

    /*
    * @brief 矿产数量减一
    * @return void
    */
    void decreaseMine() { _mineNum--; }

    /*
    * @brief isCollision
    * @param the position
    * @return bool
    */
    bool isCollision(cocos2d::Vec2 position);

    /*
    * @brief getTileSize
    * @return the size of tile
    */
    float getTileSize();

};

#endif // __Welcome_SCENE_H__