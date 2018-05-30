#include "Scene/WelcomeScene.h"  
#include "Scene/GameScene.h"  
#include "ui\CocosGUI.h"
#define MAPX 5765
#define MAPY 5376
#define MINLENTH 15
#define SPEED 20

USING_NS_CC;
using namespace ui;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in WelcomeScene.cpp\n");
}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;

}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

    this->dataInit();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //===================Load map=========================
    _tileMap = TMXTiledMap::create("GameItem/Map/map1.tmx");
    this->addChild(_tileMap);

    _ground = _tileMap->getLayer("ground");

    /*update by czd*/
    Sprite* small_map = Sprite::create("GameItem/Map/small_map.png");
    small_map->setPosition(Point(visibleSize.width - 358 / 2, visibleSize.height - 334 / 2));
    this->addChild(small_map);

    auto _smallMapListener = EventListenerTouchOneByOne::create();
    _smallMapListener->onTouchBegan = [=](Touch* touch, Event* event) {
        Point pos2 = touch->getLocationInView();
        Point position = Director::getInstance()->convertToGL(pos2);
        if (position.x > visibleSize.width - 358 && position.y > visibleSize.height - 334) {
            auto X = (position.x - (visibleSize.width - 358)) / 358 * MAPX - visibleSize.width / 2;
            auto Y = (position.y - (visibleSize.height - 334)) / 334 * MAPY - visibleSize.width / 2;
            if (X < 0) X = 0;
            if (Y < 0) Y = 0;
            if (X > MAPX - visibleSize.width) X = MAPX - visibleSize.width;
            if (Y > MAPX - visibleSize.height) Y = MAPX - visibleSize.height;
            _tileMap->runAction(MoveTo::create(0.1, Point(-X, -Y)));
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_smallMapListener, this);



    /*update by czd */
    auto _mouseOutBoradListener = EventListenerMouse::create();
    _mouseOutBoradListener->onMouseMove = [&](Event* event) {
        EventMouse* pem = static_cast<EventMouse*>(event);
        _cursorPosition = Vec2(pem->getCursorX(), pem->getCursorY());
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_mouseOutBoradListener, 1);


    //auto backItem = MenuItemImage::create(
    //	"backNormal.png",
    //	"backSelected.png",
    //	CC_CALLBACK_1(GameScene::menuBackCallback, this));

    //if (backItem == nullptr ||
    //	backItem->getContentSize().width <= 0 ||
    //	backItem->getContentSize().height <= 0)
    //{
    //	problemLoading("'backNormal.png' and 'backSelected.png'");
    //}
    //else
    //{
    //	float x = origin.x + visibleSize.width - backItem->getContentSize().width / 2;
    //	float y = origin.y + visibleSize.height - backItem->getContentSize().height / 2;
    //	backItem->setPosition(Vec2(x, y));
    //}

    //// create menu, it's an autorelease object
    //auto menu = Menu::create(backItem, NULL);
    //menu->setPosition(Vec2::ZERO);
    //this->addChild(menu, 1);

    scheduleUpdate();


    _manager = Manager::createWithGameScene(this);

    _manager->retain();
    _manager->getMoveController()->retain();

    return true;
}

void GameScene::dataInit()
{
    // To Do: 数据合理
    _isPowerEnough = false;
    _money = 1000;
    _power = 0;
    
    _barracksNum = 0;
    _mineNum = 0;
    _powerPlantNum = 0;
    _carFactoryNum = 0;

    _carFactoryPosition = _barracksPosition = Vec2::ZERO;

    _isBaseExist = false;
}

void GameScene::menuBackCallback(Ref *pSender)
{
	//跳转到第一个场景，记得包含第一个场景的头文件：GameScene.h  
	//Director::getInstance()->replaceScene(MyFirstScene::createScene());  
	Director::getInstance()->popScene();
}

Vector<Unit*>* GameScene::getSelectedSoldiers()
{
    return &_selectedSoldiers;
}

Vector<Unit*>* GameScene::getSoldiers()
{
    return &_soldiers;
}

Vector<Building*>* GameScene::getBuildings()
{
    return &_buildings;
}

void GameScene::addMoney(int money)
{
    _money += money;
}

void GameScene::decreaseMoney(int money)
{
    _money -= money;
}

void GameScene::addPower(int power)
{
    _power += power;
    if (_power >= 0)
    {
        _isPowerEnough = true;
    }
    else
    {
        _isPowerEnough = false;
    }
}

void GameScene::decreasePower(int power)
{
    _power -= power;
    if (_power >= 0)
    {
        _isPowerEnough = true;
    }
    else
    {
        _isPowerEnough = false;
    }
}

void GameScene::addTotalPower(int power)
{
    _totalPower += power;
}

void GameScene::decreaseTotalPower(int power)
{
    _totalPower -= power;
}

void GameScene::update(float time)
{
    _manager->waitCreateBuilding();  
    _manager->waitCreateSoldier();

    _manager->attack();
    _manager->addMoneyUpdate();

    _manager->getMoveController()->moveSoldiers();

    scrollMap();

}

/*update by czd */
void GameScene::scrollMap() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto X = _cursorPosition.x;
    auto Y = _cursorPosition.y;
    if (X < MINLENTH) {
        if (_tileMap->getPositionX() + SPEED < 0) {
            _tileMap->runAction(MoveBy::create(0.1, Point(SPEED, 0)));
        }
        else {
            _tileMap->setPositionX(0);
        }
    }
    if (Y < MINLENTH) {
        if (_tileMap->getPositionY() + SPEED < 0) {
            _tileMap->runAction(MoveBy::create(0.1, Point(0, SPEED)));
        }
        else {
            _tileMap->setPositionY(0);
        }
    }
    if (X > visibleSize.width - MINLENTH) {
        if (_tileMap->getPositionX() - SPEED > -MAPX + visibleSize.width) {
            _tileMap->runAction(MoveBy::create(0.1, Point(-SPEED, 0)));
        }
        else {
            _tileMap->setPositionX(-MAPX + visibleSize.width);
        }
    }
    if (Y >visibleSize.height - MINLENTH) {
        if (_tileMap->getPositionY() - SPEED > -MAPY + visibleSize.height) {
            _tileMap->runAction(MoveBy::create(0.1, Point(0, -SPEED)));
        }
        else {
            _tileMap->setPositionY(-MAPY + visibleSize.height);
        }
    }
}

bool GameScene::isCollision(cocos2d::Vec2 position)
{
    // turn PixelPosition to TileCoord
    Size mapSize = _tileMap->getMapSize();
    Size tileSize = _tileMap->getTileSize();
    int x = position.x / tileSize.width;
    int y = (mapSize.height * tileSize.height - position.y) / tileSize.height;

    // get the GID of tile
    int tileGID = _ground->getTileGIDAt(Vec2(x, y));

    // get the properties
    auto properties = _tileMap->getPropertiesForGID(tileGID).asValueMap();

    return properties["moveable"].asInt();
}

float GameScene::getTileSize()
{
    return _tileMap->getTileSize().width;
}