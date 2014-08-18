#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
    CCLayerColor *bg = CCLayerColor::create();
    bg->initWithColor(ccc4(255, 255, 255, 255));
    // add layer as a child to scene
    scene->addChild(layer,1);
    
    scene->addChild(bg,0);
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getVisibleSize();
    CCSprite *player = CCSprite::create("Player.png");
    player->setPosition(ccp(0+20,screenSize.height/2));
    this->addChild(player);
    this->schedule(schedule_selector(HelloWorld::gameLogic), 0.5);
    this->setTouchEnabled(true);
    this->schedule(schedule_selector(HelloWorld::update));
    _targets = new CCArray;
    _projs = new CCArray;
    
    return true;
}

HelloWorld::~ HelloWorld(){
    if(_targets != NULL){
        _targets->release();
    }
    if(_projs != NULL){
        _projs->release();
    }
}
void HelloWorld::gameLogic(float duration){
    this->addTarget();
}
//生成一个怪物
void HelloWorld::addTarget()
{
    CCSprite *target = CCSprite::create("Target.png");
    CCSize screenSize = CCDirector::sharedDirector()->getVisibleSize();
    int y = rand()%(int)screenSize.height;
    target->setPosition(ccp(screenSize.width-20, y));
    target->setTag(1);
    CCMoveTo *move = CCMoveTo::create(3, ccp(0, y));
    CCCallFuncN *disappear = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::disappear));
    CCSequence *actions = CCSequence::create(move,disappear,NULL);
    target->runAction(actions);
    this->addChild(target);
    _targets->addObject(target);
}

void HelloWorld::disappear(CCNode* who){
    int tag = who->getTag();
    if(1 == tag){
        _targets->removeObject(who);
    }else if(2 == tag){
        _projs->removeObject(who);
    }
    who->removeFromParentAndCleanup(true);
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCTouch* touch = (CCTouch *)pTouches->anyObject();
    CCPoint locInView = touch->getLocationInView();
    CCPoint loc = CCDirector::sharedDirector()->convertToGL(locInView);//从UI坐标变换到cococs2d中的坐标
    CCLOG("X:%f,Y:%f", loc.x,loc.y);
    
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) // 触摸事件响应函数
{
    CCTouch* touch = (CCTouch *)pTouches->anyObject();
    CCPoint locInView = touch->getLocationInView();
    CCPoint loc = CCDirector::sharedDirector()->convertToGL(locInView);//从UI坐标变换到cococs2d中的坐标
    CCLOG("X:%f,Y:%f", loc.x,loc.y);
    //    return;
    if(loc.x<20){//点击位置无效
        return;
    }
    
    CCSize screenSize = CCDirector::sharedDirector()->getVisibleSize();
    CCSprite* proj = CCSprite::create("Projectile.png");
    proj->setPosition(ccp(20, screenSize.height / 2.0));
    proj->setTag(2);
    this->addChild(proj); //创建飞镖精灵
    _projs->addObject(proj);
    
    double dx = loc.x - 20;
    double dy = loc.y - screenSize.height / 2.0;
    double d = sqrt(dx * dx + dy * dy); // 触摸点到出发点的距离
    
    double D = sqrt(screenSize.width * screenSize.width + screenSize.height * screenSize.height); // 总的运行距离
    
    double ratio = D / d;
    double endx = ratio * dx + 20; // 最终点的x坐标
    double endy = ratio * dy + screenSize.height / 2.0;// 最终点的y坐标
    
    CCMoveTo* move = CCMoveTo::create(D / 320, ccp(endx, endy));
    CCCallFuncN* moveFinish = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::disappear));
    CCSequence* actions = CCSequence::create(move,moveFinish,NULL);
    
    proj->runAction(actions);
}

void HelloWorld::update(float delta) // delta = 1.0 / fps
{
    CCArray* targetToDelete = new CCArray;
    CCArray* projToDelete = new CCArray;
    CCObject* itarget;
    CCObject* iproj;
    
    CCARRAY_FOREACH(_targets, itarget){
        CCSprite* target = (CCSprite *)itarget;
        CCRect targetZone = CCRectMake(target->getPositionX(), target->getPositionY(), target->getContentSize().width, target->getContentSize().height);
        
        CCARRAY_FOREACH(_projs, iproj){
            CCSprite* proj = (CCSprite *)iproj;
            CCRect projZone = CCRectMake(proj->getPositionX(), proj->getPositionY(), proj->getContentSize().width, proj->getContentSize().height);
            
            if(projZone.intersectsRect(targetZone)){
                targetToDelete->addObject(itarget);
                projToDelete->addObject(iproj);
            }
        }
    }
    
    CCARRAY_FOREACH(projToDelete, iproj){
        _projs->removeObject(iproj);
        CCSprite* proj = (CCSprite *)iproj;
        proj->removeFromParentAndCleanup(true);
    }
    CCARRAY_FOREACH(targetToDelete, itarget){
        _targets->removeObject(itarget);
        CCSprite* target = (CCSprite *)itarget;
        target->removeFromParentAndCleanup(true);
    }
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
