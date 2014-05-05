#include "HelloWorldScene.h"

#include "ChessboardProtocol.h"
#include "Chessboard.h"
#include "ChessboardChecker.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    // test

    Chessboard* cb = Chessboard::create();
    cb->onMessage(BEGIN_MOVE_MSG);
    cb->onMessage(END_MOVE_MSG);
    CCLOG("%s", cb->getState());
    
    PIECE pieces1[][WIDTH] = {
        { BLACK, BLACK, BLACK, BLACK },
        { ZERO, ZERO, ZERO, ZERO },
        { ZERO, ZERO, ZERO, ZERO },
        { WHITE, ZERO, ZERO, ZERO }
    };
    cb->setPieces(pieces1);
    CCLOG("%i", checkWin(cb->getPieces(), cb->getCurrentMove()));

    PIECE pieces2[][WIDTH] = {
        { BLACK, BLACK, BLACK, BLACK },
        { ZERO, ZERO, ZERO, ZERO },
        { ZERO, ZERO, ZERO, ZERO },
        { WHITE, ZERO, ZERO, ZERO }
    };
    cb->setPieces(pieces2);
    Move move1(BLACK, ccp(0, 0), ccp(0,1) );
    CCLOG("%i", checkMove(cb->getPieces(), move1));
    
    PIECE pieces3[][WIDTH] = {
        { ZERO, WHITE, ZERO, ZERO },
        { ZERO, BLACK, ZERO, ZERO },
        { ZERO, BLACK, ZERO, ZERO },
        { ZERO, BLACK, ZERO, ZERO }
    };
    cb->setPieces(pieces3);
    cb->alterMove(Move(BLACK, ccp(1, 3), ccp(2, 3)));
    Move move2(BLACK, ccp(1, 3), ccp(2, 3));
    CCLOG("%i", cb->checkEat(move2));
    
// test case end 
    
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}