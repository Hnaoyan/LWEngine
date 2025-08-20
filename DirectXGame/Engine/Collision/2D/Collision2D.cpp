#include "Collision2D.h"

bool Collision2D::IsCollision(const Rectangle2D& rectA, const Rectangle2D& rectB)
{
    //bool isXOver = (rectA.min_.x >= rectB.min_.x && rectA.min_.x <= rectB.max_.x) ||
    //    (rectA.max_.x >= rectB.min_.x && rectA.max_.x <= rectB.max_.x) ||
    //    (rectB.min_.x >= rectA.min_.x && rectB.min_.x <= rectA.max_.x) ||
    //    (rectB.max_.x >= rectA.min_.x && rectB.max_.x <= rectA.max_.x);
    //bool isYOver = (rectA.min_.y >= rectB.min_.y && rectA.min_.y <= rectB.max_.y) ||
    //    (rectA.max_.y >= rectB.min_.y && rectA.max_.y <= rectB.max_.y) ||
    //    (rectB.min_.y >= rectA.min_.y && rectB.min_.y <= rectA.max_.y) ||
    //    (rectB.max_.y >= rectA.min_.y && rectB.max_.y <= rectA.max_.y);
    //if (isXOver && isYOver) {
    //    return true;
    //}
    //return false;

    bool isXOverlap = (rectA.min_.x <= rectB.max_.x && rectA.max_.x >= rectB.min_.x);
    bool isYOverlap = (rectA.min_.y <= rectB.max_.y && rectA.max_.y >= rectB.min_.y);
    return isXOverlap && isYOverlap;
}
