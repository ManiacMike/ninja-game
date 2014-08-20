#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static CCScene* scene();
    
    // a selector callback
//    void menuCloseCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    
    void addTarget();//增加一个怪兽
    
    void disappear(CCNode* who);//删除怪兽
    
    void addTargetUpdate(float duration);//增加怪兽，调度函数回调函数
    
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent); // 触摸事件响应函数
    
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent); // 触摸事件响应函数
    
    void update(float delta); //调度函数 delta = 1.0 / fps
    
    CCArray* _targets;//怪兽对象数组
    
    CCArray* _projs;//飞镖对象数组
    
    ~HelloWorld();//场景析构函数
};

#endif // __HELLOWORLD_SCENE_H__
