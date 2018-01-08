//
// Copyright (c) 2010 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


ofApp::ofApp():
    hash(points),
    mode(MODE_RADIUS),
    radius(DEFAULT_RADIUS),
    nearestN(DEFAULT_NEAREST_N)
{
}


void ofApp::setup()
{
    ofEnableAlphaBlending();

    mesh.setMode(OF_PRIMITIVE_POINTS);

    for (std::size_t i = 0; i < NUM_POINTS; ++i)
    {
        Vec2 point(ofRandom(50, ofGetWidth() - 50),
                   ofRandom(50, ofGetHeight() - 50));

        points.push_back(point);
        mesh.addVertex(Vec3(point.x, point.y, 0));
    }

    hash.buildIndex();
}


void ofApp::update()
{
    mouse = Vec2(ofGetMouseX(), ofGetMouseY());

    searchResults.clear();

    if (MODE_RADIUS == mode)
    {
        // An estimate of the number of points we are expecting to find.
        searchResults.resize(NUM_POINTS / 4);

        hash.findPointsWithinRadius(mouse, radius, searchResults);
    }
    else
    {
        searchResults.resize(nearestN);

        // NOTE: this method signature is slower that the alternative method
        // signature as this method signature requires extra copies.  This is
        // done to simplify the example and use the same search results data
        // structure.
        hash.findNClosestPoints(mouse, nearestN, searchResults);
    }

}


void ofApp::draw()
{
    ofBackground(0);

    ofNoFill();
    ofSetColor(255);

    // Draw all of the points.
    mesh.draw();

    ofFill();
    ofSetColor(255, 255, 0, 80);

    for (std::size_t i = 0; i < searchResults.size(); ++i)
    {
        float normalizedDistance = ofMap(searchResults[i].second, radius * radius, 0, 0, 1, true);

        ofSetColor(255, 255, 0, normalizedDistance * 127);

        ofDrawCircle(points[searchResults[i].first], 5);

        if (points[searchResults[i].first].x == 0 || points[searchResults[i].first].y == 0)
        {

            std::cout << points[searchResults[i].first] << " i = " << i << " dist= " << searchResults[i].second << std::endl;
        }

        if (MODE_NEAREST_N == mode)
        {
            ofSetColor(255, 127);
            ofDrawBitmapString(ofToString(i), points[searchResults[i].first]);
        }
    }

    if (MODE_RADIUS == mode)
    {
        ofNoFill();
        ofSetColor(255, 127);//, 0, 0, 50);
        ofDrawCircle(mouse, radius);
    }

    std::stringstream ss;


    if (MODE_RADIUS == mode)
    {
        ss << "SEARCH MODE (space): RADIUS" << std::endl;
        ss << "       RADIUS (-/=): " << radius;
    }
    else
    {
        ss << "SEARCH MODE (space): NEAREST N" << std::endl;
        ss << "    NEAREST N (-/=): " << nearestN;
    }

    ofDrawBitmapStringHighlight(ss.str(), Vec2(30, 30));

}


void ofApp::keyPressed(int key)
{
    if (' ' == key)
    {
        if (MODE_RADIUS == mode)
        {
            mode = MODE_NEAREST_N;
        }
        else
        {
            mode = MODE_RADIUS;
        }
    }
    else if ('=' == key)
    {
        if (MODE_RADIUS == mode)
        {
            radius += 10;
        }
        else
        {
            nearestN += 1;
        }

        radius = std::max(0, radius);
    }
    else if ('-' == key)
    {
        if (MODE_RADIUS == mode)
        {
            radius -= 10;
        }
        else
        {
            nearestN -= 1;
        }

        radius = std::max(0, radius);
    }
}
