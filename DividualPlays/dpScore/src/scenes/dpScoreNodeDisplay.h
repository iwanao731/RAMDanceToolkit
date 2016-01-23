//
//  dpScoreNodeDisplay.h
//  dpScore
//
//  Created by YoshitoONISHI on 1/20/16.
//
//

#ifndef dpScoreNodeDisplay_h
#define dpScoreNodeDisplay_h

#include "dpScoreBaseNode.h"
#include "dpScoreObjects.h"


DP_SCORE_NAMESPACE_BEGIN

class NodeDisplay: public BaseNode {
public:
	NodeDisplay();
	virtual ~NodeDisplay();

	void customDraw() override;

private:

};

DP_SCORE_NAMESPACE_END

#endif /* dpScoreNodeDisplay_h */
