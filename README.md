ofxSpatialHash
==============

## Description

A collection of 2D/3D spatial hashing algorithms.  Useful for quick nearest neighbor calculations in 2-, 3-d and N-d.

Inspired by https://github.com/neilmendoza/ofxNearestNeighbour/ and others.

See class documentation and examples for more information.

![2-D Hash](https://github.com/bakercp/ofxSpatialHash/raw/master/docs/screen_2d.png)
![3-D Hash](https://github.com/bakercp/ofxSpatialHash/raw/master/docs/screen_3d.png)
![N-D Hash (example uses 3 dimensions for easy visualization)](https://github.com/bakercp/ofxSpatialHash/raw/master/docs/screen_3d.png)

## Features

- Allows for an all-in-one approach to spatially indexing existing point clouds for rapid 2D/3D search.
- Supports easy use of `ofVec2f`, `ofVec3f`, `ofVec2f`, `glm::vec2`, `glm::vec3`, `glm::vec4`.
- Supports `N` dimensional hash using `std::array<float, N>`.  See `example_kdtree_nd` for a 3d version.

## Getting Started

To get started, generate the example project files using the openFrameworks [Project Generator](http://openframeworks.cc/learning/01_basics/how_to_add_addon_to_project/).

## Documentation

API documentation can be found here.

## Build Status

Linux, macOS [![Build Status](https://travis-ci.org/bakercp/ofxSpatialHash.svg?branch=master)](https://travis-ci.org/bakercp/ofxSpatialHash)

Visual Studio, MSYS [![Build status](https://ci.appveyor.com/api/projects/status/7s76w2xstibc5w3x/branch/master?svg=true)](https://ci.appveyor.com/project/bakercp/ofxspatialhash/branch/master)

## Compatibility

The `stable` branch of this repository is meant to be compatible with the openFrameworks [stable branch](https://github.com/openframeworks/openFrameworks/tree/stable), which corresponds to the latest official openFrameworks release.

The `master` branch of this repository is meant to be compatible with the openFrameworks [master branch](https://github.com/openframeworks/openFrameworks/tree/master).

Some past openFrameworks releases are supported via tagged versions, but only `stable` and `master` branches are actively supported.

## Versioning

This project uses Semantic Versioning, although strict adherence will only come into effect at version 1.0.0.

## Licensing

See `LICENSE.md`.

## Contributing

Pull Requests are always welcome, so if you make any improvements please feel free to float them back upstream :)

1. Fork this repository.
2. Create your feature branch (`git checkout -b my-new-feature`).
3. Commit your changes (`git commit -am 'Add some feature'`).
4. Push to the branch (`git push origin my-new-feature`).
5. Create new Pull Request.

