//
//  ChessboardController.cpp
//  Four
//
//  Created by 王选易 on 14-4-24.
//
//

#include "ChessboardController.h"
#include "PieceView.h"
#include "LogoView.h"
#include "Chessboard.h"

ChessboardController::~ChessboardController(){
    CC_SAFE_RELEASE(chessboard);
}

bool ChessboardController::init(){
    using namespace cocos2d;
    
    CCLog("%s", "init");
    
    if (! BaseController::init()) {
        return false;
    }
    
    // 初始化Model
    this->chessboard = Chessboard::create();
    this->chessboard->retain();
    
    //初始化View
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            CCString* frame_name;
            if (chessboard->getPiece(ccp(j, i)) == BLACK )
                frame_name = new CCString("black.png");
            else if (chessboard->getPiece(ccp(j, i)) == WHITE)
                frame_name = new CCString("white.png");
            else
                continue;
            CCSprite* black = PieceView::create(&(this->chessboard->getCurrentMove()), this->chessboard, cache->spriteFrameByName(frame_name->getCString()), ccp(j, i));
            this->addChild(black);
        }
    }
    
    CCSprite* black_logo = LogoView::create(&(this->chessboard->getCurrentMove()), cache->spriteFrameByName("black_logo.png"), BLACK);
    CCSprite* white_logo = LogoView::create(&(this->chessboard->getCurrentMove()), cache->spriteFrameByName("white_logo.png"), WHITE);
    this->addChild(black_logo);
    this->addChild(white_logo);
    
    
//    // touch
    this->setTouchEnabled(true);

    return true;
}


void ChessboardController::onEnter()
{
    using cocos2d::CCDirector;
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChessboardController::tryEat), END_MOVE_MSG, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ChessboardController::tryWin), END_EAT_MSG, NULL);
    
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}

void ChessboardController::onExit()
{
    using cocos2d::CCDirector;
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool ChessboardController::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return true;
}

void ChessboardController::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    //    CCTouchDelegate
}

void logPoint(const cocos2d::CCPoint& p){
    CCLOG("x:%f y:%f", p.x, p.y);
}

void ChessboardController::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    tryMove(RealToLogic(pTouch->getStartLocation()), RealToLogic(pTouch->getLocation()));
}

void ChessboardController::tryMove(const CCPoint& src, const CCPoint& dest){
    if (dest.equals(src))
        return;
    CCPoint newDest = (dest - src) / (dest - src).getLength() + src;
    Move move(chessboard->getCurrentMove().currentRound,src, newDest);
    if(chessboard->checkMessage(BEGIN_MOVE_MSG) && chessboard->checkMove(move)){
        chessboard->alterMove(move);
    }
}

void ChessboardController::tryEat(){
    CCArray* children = this->getChildren();
    CCLog("children number is %d", children->count());
    
    Move move(chessboard->getCurrentMove());
    if(chessboard->checkMessage(BEGIN_EAT_MSG)){
        if (chessboard->checkEat(move)){
            chessboard->alterEat(move);
        }else{
            chessboard->onMessage(NEXT_ROUND_MSG);
        }
    }
}

void ChessboardController::tryWin(){
    if (chessboard->checkMessage(WIN_MSG)) {
        if (chessboard->checkWin(chessboard->getCurrentMove())){
            chessboard->alterWin();
        }else{
            chessboard->onMessage(NEXT_ROUND_MSG);
        }
    }
}