//
// Copyright (c) 2013 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


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
        // Choose 3 dimensions to make visualization easier.
        // But NUM_DIMENSIONS can be any dimension.
        NUM_DIMENSIONS = 3,
        NUM_POINTS = 10000,
        DEFAULT_RADIUS = 200, // vector units (pixels)
        DEFAULT_NEAREST_N = 200
    };

    /// \brief Define a data type in one place for convenience.
    typedef std::array<float, NUM_DIMENSIONS> DataType;

    /// \brief Our point collection.
    ///
    /// These points MUST be initialized BEFORE initing the hash.
    std::vector<DataType> points;

    /// \brief The spatial hash specialized for ofVec3f.
    ofx::KDTree<DataType> hash;

    /// \brief The search results specialized for ofVec3f.
    ofx::KDTree<DataType>::SearchResults searchResults;

    /// \brief A little firefly that moves around the NUM_DIMENSIONS space.
    DataType firefly;

    /// \brief The camera.
    ofEasyCam cam;

    /// \brief A mesh to make it easier to draw lots of points.
    ofMesh mesh;

    /// \brief The search modes in this example.
    enum Modes
    {
        MODE_RADIUS,
        MODE_NEAREST_N
    };

    /// \brief The current search mode.
    int mode = 0;

    /// \brief Radius used for radius search.
    int radius = 0;

    /// \brief Number of nearest neighbors to use for Nearest Nieghbor search.
    int nearestN = 0;

};
