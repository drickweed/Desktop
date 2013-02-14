#include "textnode.h"

TextNode::TextNode():
		size(1) {
}

void TextNode::applyDeltaMatrix(QPointF delta) {
	QTransform t;
	t.fromTranslate(delta.x(), delta.y());
	applyMatrix(t);
}
