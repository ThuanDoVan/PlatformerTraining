#ifndef __INFINITE_PARALLAX_NODE_H__
#define __INFINITE_PARALLAX_NODE_H__

#include "cocos2d.h"

USING_NS_CC;

//using namespace spine;

class InfiniteParallaxNode : public ParallaxNode
{
public:

	static InfiniteParallaxNode* create();
	void updatePosition();
};

#endif
