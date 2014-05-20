// =============================================================================
//
// Copyright (c) 2010-2014 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


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
    ofEnableDepthTest();

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (std::size_t i = 0; i < NUM_POINTS; ++i)
    {
        ofVec3f point(ofRandomWidth(), ofRandomHeight(), ofRandom(-500, 500));
        points.push_back(point);
        mesh.addVertex(point);
    }

    hash.buildIndex();

    firefly = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, 0);

}


void ofApp::update()
{
    // Random walk.
    int delta = 50;

    float time = ofGetElapsedTimef() / 10;

    firefly.x = ofMap(ofNoise(time +   0), 0, 1, -500, ofGetWidth() + 500);
    firefly.y = ofMap(ofNoise(time + 100), 0, 1, -500, ofGetHeight() + 500);
    firefly.z = ofMap(ofNoise(time + 500), 0, 1, -1000, 1000);


    if (MODE_RADIUS == mode)
    {
        // An estimate of the number of points we are expecting to find.

        // Estimate the volume of our seach envelope as a cube.
        // A cube already overestimates a spherical search space.
        float approxCubicSearchBoxSize = (radius * 2 * radius * 2 * radius * 2);

        // Calculate the volume of our total search space as a cube.
        float approxCubicSearchSpaceSize = (ofGetWidth() * ofGetHeight() * 2 * 500);

        // Determine the percentage of the total search space we expect to capture.
        float approxPercentageOfTotalPixels = approxCubicSearchBoxSize / approxCubicSearchSpaceSize;

        // Assuming an uniform distribution of points in our search space,
        // get a percentage of them.
        std::size_t approximateNumPointsToFind = points.size() * approxPercentageOfTotalPixels;

        searchResults.resize(approximateNumPointsToFind);
        
        hash.findPointsWithinRadius(firefly, radius, searchResults);
    }
    else
    {
        searchResults.resize(nearestN);

        // NOTE: this method signature is slower that the alternative method
        // signature as this method signature requires extra copies.  This is
        // done to simplify the example and use the same search results data
        // structure.
        hash.findNClosestPoints(firefly, nearestN, searchResults);
    }
}


void ofApp::draw()
{
    ofBackground(0);

    cam.begin();

    ofPushMatrix();
    ofTranslate(- ofGetWidth() / 2, - ofGetHeight() / 2, 0);

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

        ofDrawSphere(points[searchResults[i].first], 5);

        if (MODE_NEAREST_N == mode)
        {
            ofSetColor(255, 127);
            ofDrawBitmapString(ofToString(i), points[searchResults[i].first]);
        }
    }

    if (MODE_RADIUS == mode)
    {
        ofNoFill();
        ofSetColor(255, 0, 0, 50);
        ofDrawSphere(firefly, radius);
    }

    ofPopMatrix();

    cam.end();

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

    ofDrawBitmapStringHighlight(ss.str(), ofVec2f(30, 30));
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
            radius = std::max(0, radius + 10);
        }
        else
        {
            nearestN = std::max(0, nearestN + 10);
        }
    }
    else if ('-' == key)
    {
        if (MODE_RADIUS == mode)
        {
            radius = std::max(0, radius - 10);
        }
        else
        {
            nearestN = std::max(0, nearestN - 10);
        }
    }
}

