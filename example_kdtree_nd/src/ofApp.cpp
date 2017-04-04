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
    ofEnableDepthTest();

    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    for (std::size_t i = 0; i < NUM_POINTS; ++i)
    {
        DataType point;

        // Fill each point with a random number.
        for (std::size_t j = 0; j < NUM_DIMENSIONS; ++j)
        {
            point[j] = ofRandom(-ofGetWidth(), ofGetWidth());
        }

        points.push_back(point);

        // Take the first three as x, y, z.
        glm::vec3 visiblePoint(point[0], point[1], point[2]);

        mesh.addVertex(visiblePoint);
    }

    hash.buildIndex();

    firefly[0] = ofGetWidth() / 2;
    firefly[1] = ofGetHeight() / 2;
    firefly[2] = 0;

}


void ofApp::update()
{
    // Random walk.
    float time = ofGetElapsedTimef() / 10;

    firefly[0] = ofMap(ofNoise(time +   0), 0, 1, -500, ofGetWidth() + 500);
    firefly[1] = ofMap(ofNoise(time + 100), 0, 1, -500, ofGetHeight() + 500);
    firefly[2] = ofMap(ofNoise(time + 500), 0, 1, -1000, 1000);

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
        std::size_t resultIndex = searchResults[i].first;
        float resultDistance = searchResults[i].second;

        DataType& result = points[resultIndex];

        glm::vec3 resultAsPoint(result[0], result[1], result[2]);

        float normalizedDistance = ofMap(resultDistance, radius * radius, 0, 0, 1, true);

        ofSetColor(255, 255, 0, normalizedDistance * 127);

        ofDrawSphere(resultAsPoint, 5);

        if (MODE_NEAREST_N == mode)
        {
            ofSetColor(255, 127);
            ofDrawBitmapString(ofToString(i), resultAsPoint);
        }
    }

    if (MODE_RADIUS == mode)
    {
        ofNoFill();
        ofSetColor(255, 0, 0, 50);

        glm::vec3 fireflyAsPoint(firefly[0], firefly[1], firefly[2]);

        ofDrawSphere(fireflyAsPoint, radius);
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

    ofDrawBitmapStringHighlight(ss.str(), glm::vec2(30, 30));
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

