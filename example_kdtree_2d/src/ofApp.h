// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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


#pragma once


#include "ofMain.h"
#include "ofxSpatialHash.h"


class ofApp: public ofBaseApp
{
public:
    ofApp();
    void setup();
    void update();
    void draw();

    void keyPressed(int key);

    /// \brief A collection of default values.
    enum
    {
        NUM_POINTS = 20000,
        DEFAULT_RADIUS = 200, // vector units (pixels)
        DEFAULT_NEAREST_N = 200
    };

    /// \brief Our point collection.
    ///
    /// These points MUST be initialized BEFORE initing the hash.
    std::vector<ofVec2f> points;

    /// \brief The spatial hash specialized for ofVec2f.
    ofx::KDTree<ofVec2f> hash;

    /// \brief The search results specialized for ofVec2f.
    ofx::KDTree<ofVec2f>::SearchResults searchResults;

    /// \brief The current mouse position.
    ofVec2f mouse;

    /// \brief A mesh to make it easier to draw lots of points.
    ofMesh mesh;

    /// \brief The search modes in this example.
    enum Modes
    {
        MODE_RADIUS,
        MODE_NEAREST_N
    };

    /// \brief The current search mode.
    int mode;

    /// \brief Radius used for radius search.
    int radius;

    /// \brief Number of nearest neighbors to use for Nearest Nieghbor search.
    int nearestN;

};
