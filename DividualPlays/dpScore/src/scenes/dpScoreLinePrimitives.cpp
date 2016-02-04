//
//  dpScoreLinePrimitives.cpp
//  dpScore
//
//  Created by YoshitoONISHI on 2/3/16.
//
//

#include "dpScoreLinePrimitives.h"
#include "dpScoreToolBox.h"
#include "dpScoreScoped.h"
#include "dpScoreStyle.h"

DP_SCORE_NAMESPACE_BEGIN

Line Line::create(const ofVec3f& p0, const ofVec3f& p1, int res)
{
    Line line;
    line.setup(p0, p1, res);
    return line;
}

void Line::setup(const ofVec3f& p0, const ofVec3f& p1, int res)
{
    mTypes.clear();
    const float step {1.f / (float)res};
    for (auto i : rep(res)) {
        const float t0 {i * step};
        const float t1 {(i + 1) * step};
        mTypes.push_back(LineType::make(p0.interpolated(p1, t0), p0.interpolated(p1, t1)));
    }
    mPoints.assign(mTypes.size(), Point());
    reset();
}

void Line::update(const ofVec3f& p0, const ofVec3f& p1)
{
    const auto res = mTypes.size();
    const float step {1.f / (float)res};
    for (auto i : rep(res)) {
        const float t0 {i * step};
        const float t1 {(i + 1) * step};
        mTypes.at(i).set(p0.interpolated(p1, t0), p0.interpolated(p1, t1));
    }
}

Box Box::create(const ofVec3f& p, float w, float h, float d)
{
    Box b;
    b.setup(p, w, h, d);
    return b;
}

void Box::setup(const ofVec3f& p, float w, float h, float d)
{
    const ofVec3f o(p.x + w * 0.5f, p.y + h * 0.5f, p.z + d * 0.5);
    const float x {w * 0.5f};
    const float y {h * 0.5f};
    const float z {d * 0.5f};
    const ofVec3f v0 {ofVec3f(-x, -y, -z) + o};
    const ofVec3f v1 {ofVec3f(x, -y, -z) + o};
    const ofVec3f v2 {ofVec3f(x, -y,  z) + o};
    const ofVec3f v3 {ofVec3f(-x, -y,  z) + o};
    const ofVec3f v4 {ofVec3f(-x,  y, -z) + o};
    const ofVec3f v5 {ofVec3f(x,  y, -z) + o};
    const ofVec3f v6 {ofVec3f(x,  y,  z) + o};
    const ofVec3f v7 {ofVec3f(-x,  y,  z) + o};
    
    mTypes.clear();
    mTypes.push_back(LineType::make(v0, v1));
    mTypes.push_back(LineType::make(v1, v2));
    mTypes.push_back(LineType::make(v2, v3));
    mTypes.push_back(LineType::make(v3, v0));
    
    mTypes.push_back(LineType::make(v0, v4));
    mTypes.push_back(LineType::make(v1, v5));
    mTypes.push_back(LineType::make(v2, v6));
    mTypes.push_back(LineType::make(v3, v7));
    
    mTypes.push_back(LineType::make(v4, v5));
    mTypes.push_back(LineType::make(v5, v6));
    mTypes.push_back(LineType::make(v6, v7));
    mTypes.push_back(LineType::make(v7, v4));
    
    mPoints.assign(mTypes.size(), Point());
    reset();
}

Cylinder Cylinder::create(const ofVec3f& p, float r, float h, int res)
{
    Cylinder c;
    c.setup(p, r, h, res);
    return c;
}

void Cylinder::setup(const ofVec3f& p, float r, float h, int res)
{
    for (auto i : rep(res)) {
        const float step {(float)TWO_PI / (float)res};
        const float rad0 {(i + 0) * step};
        const float rad1 {(i + 1) * step};
        const float x0 {::cosf(rad0) * r};
        const float z0 {::sinf(rad0) * r};
        const float x1 {::cosf(rad1) * r};
        const float z1 {::sinf(rad1) * r};
        
        // top circle
        mTypes.push_back(LineType::make(ofVec3f(x0, h * 0.5f, z0) + p, ofVec3f(x1, h * 0.5f, z1) + p));
        // bottom circle
        mTypes.push_back(LineType::make(ofVec3f(x0, -h * 0.5f, z0) + p, ofVec3f(x1, -h * 0.5f, z1) + p));
        // side face
        mTypes.push_back(LineType::make(ofVec3f(x0, -h * 0.5f, z0) + p, ofVec3f(x0, h * 0.5f, z0) + p));
    }
    
    mPoints.assign(mTypes.size(), Point());
    reset();
}

Rect Rect::create(const ofVec3f& p, float w, float h)
{
    Rect r;
    r.setup(p, w, h);
    return r;
}

void Rect::setup(const ofVec3f& p, float w, float h)
{
    const float x {w * 0.5f};
    const float y {h * 0.5f};
    const ofVec3f v0 {ofVec3f(-x, -y, 0.f) + p};
    const ofVec3f v1 {ofVec3f(x, -y, 0.f) + p};
    const ofVec3f v2 {ofVec3f(x,  y, 0.f) + p};
    const ofVec3f v3 {ofVec3f(-x,  y, 0.f) + p};
    
    mTypes.clear();
    mTypes.push_back(LineType::make(v0, v1));
    mTypes.push_back(LineType::make(v1, v2));
    mTypes.push_back(LineType::make(v2, v3));
    mTypes.push_back(LineType::make(v3, v0));
    
    mPoints.assign(mTypes.size(), Point());
    reset();
}

DP_SCORE_NAMESPACE_END