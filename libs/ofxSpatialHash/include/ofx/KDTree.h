// =============================================================================
//
// Copyright (c) 2010-2013 Christopher Baker <http://christopherbaker.net>
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


#include "nanoflann.hpp"


namespace ofx {


/// \brief A KDTree optimized for 2D/3D point clouds.
/// \tparam VectorType The internal VectorType used by this KDTree.
/// \tparam FloatType The internal floating point type used by this KDTree.
/// \tparam IndexType The internal index type used by this KDTree.
template<typename VectorType, typename FloatType = float, typename IndexType = std::size_t>
class KDTree
{
public:
    /// \brief A typedef for a vector of points.
    typedef std::vector<VectorType> Points;

    /// \brief A typedef for a vector of point indicies.
    typedef std::vector<IndexType> Indicies;

    /// \brief A typedef for a vector of distances squared.
    typedef std::vector<FloatType> DistancesSquared;

    /// \brief A typedef for an Index, DistanceSquared Pair.
    typedef std::pair<IndexType, FloatType> IndexDistanceSquaredPair;

    /// \brief A typedef for a vector of IndexDistanceSquaredPair searchresults.
    typedef std::vector<IndexDistanceSquaredPair> SearchResults;

    /// \brief A typedef for a simple spatial hash adapter.
    ///
    /// This SimpleAdapter calculates euclidian distances and is appropriate for
    /// low dimensional datasets, particularly 2D and 3D.
    typedef nanoflann::L2_Simple_Adaptor<FloatType,
                                         KDTree<VectorType, FloatType>,
                                         FloatType> L2_Adapter;

    /// \brief A typedef for a KDTreeSingleIndexAdaptor index adapter.
    typedef nanoflann::KDTreeSingleIndexAdaptor<L2_Adapter,
                                                KDTree<VectorType, FloatType>,
                                                VectorType::DIM,
                                                IndexType> KDTreeAdapter;

    /// \brief A typedef for a KDTreeSingleIndexAdaptorParams.
    typedef nanoflann::KDTreeSingleIndexAdaptorParams KDTreeParams;

    /// \brief Create a spatial hash with a reference to a vector or points.
    ///
    /// Users should initialize the KDTree with a const reference to a
    /// std::vector of the user's VectorType.  If the contents of this
    /// std::vector change, the KDTree index must be rebuild using the
    /// buildIndex() method, otherwise, search results will be invalid.
    ///
    /// The maximum leaf size must should be chosen to strike a balance between
    /// memory and reduced computational speed.  The optimal value can be found
    /// by experiment.  See the nanoflann documentaion for benchmarks and tips.
    ///
    /// \warning Ensure that the referenced points are initialized prior to
    ///          the construction of the KDTree.
    /// \param points A const reference to a std::vector or VectorType.
    /// \param maxLeafSize The maximum leaf size.
    /// \param autoBuildIndex Automatically build the index during construction.
    KDTree(const std::vector<VectorType>& points,
           std::size_t maxLeafSize = DEFAULT_MAX_LEAF_SIZE,
           bool autoBuildIndex = true):
        _points(points),
        _KDTree(VectorType::DIM,
                *this,
                KDTreeParams(maxLeafSize))
    {
        if (autoBuildIndex && !points.empty())
        {
            buildIndex();
        }
    }

    /// \brief Destroy the KDTree.
    virtual ~KDTree()
    {
    }

    /// \brief Rebuild the spatial hash index.
    ///
    /// If the internal data is ever
    inline void buildIndex()
    {
        _KDTree.buildIndex();
    }

    /// \brief Find the N closest points to the given point.
    /// \param point The seed point to search near.
    /// \param numPointsToFind the number of points to return.
    /// \param indices A collection of point indices for the nearby points.
    /// \param distancesSquared A collection of the point distances squared.
    void findNClosestPoints(const VectorType& point,
                            std::size_t numPointsToFind,
                            Indicies& indices,
                            DistancesSquared& distancesSquared)
    {
        // Ensure reasonable parameters.
        numPointsToFind = std::min(_points.size(), numPointsToFind);
        numPointsToFind = std::max((std::size_t)1, numPointsToFind);

        indices.resize(numPointsToFind);
        distancesSquared.resize(numPointsToFind);

        _KDTree.knnSearch(point.getPtr(),
                          numPointsToFind,
                          &indices[0],
                          &distancesSquared[0]);
    }

    /// \brief Find the N closest points to the given point.
    /// \warning This method is a simplification of findNClosestPoints and
    /// involves an extra set of copies and therefore may be somewhat slower.
    /// It is a comprimize until nanoflann has a more unified search interface.
    /// \param point The seed point to search near.
    /// \param numPointsToFind the number of points to return.
    /// \param results A collection of point indices for the nearby points.
    void findNClosestPoints(const VectorType& point,
                            std::size_t numPointsToFind,
                            SearchResults& results)
    {
        // Ensure reasonable parameters.
        numPointsToFind = std::min(_points.size(), numPointsToFind);
        numPointsToFind = std::max((std::size_t)1, numPointsToFind);

        Indicies indices;
        DistancesSquared distancesSquared;

        indices.resize(numPointsToFind);
        distancesSquared.resize(numPointsToFind);

        findNClosestPoints(point, numPointsToFind, indices, distancesSquared);

        results.resize(numPointsToFind);

        // Copy the results
        for (std::size_t i = 0; i < numPointsToFind; ++i)
        {
            results[i] = std::make_pair(indices[i], distancesSquared[i]);
        }
    }

    /// \brief Find the all points within a radius of the given point.
    ///
    /// For best results, the memory in the passed results std::vector should be
    /// preallocated using .resize() in order to prevent
    /// std::vector resizes during the search process.
    ///
    /// \param point The seed point to search near.
    /// \param radius The radius to search within.
    /// \param results A collection of point indices for the nearby points.
    /// \param epsilon The epsilon used for calculating distance equality.
    /// \param sorted True iff the the output list should be returned sorted
    ///        by ascending distances.
    /// \returns The number of points discovered within the search radius.
    std::size_t findPointsWithinRadius(const VectorType& point,
                                       FloatType radius,
                                       SearchResults& results,
                                       float epsilon = 0,
                                       bool sorted = true)
    {
        nanoflann::SearchParams params;
        params.eps = epsilon;
        params.sorted = sorted;
        
        return _KDTree.radiusSearch(point.getPtr(),
                                    radius* radius,
                                    results,
                                    params);
    }


    /// \brief Get the number of data points.
    ///
    /// This method is an interface requirement for nanoflann point cloud.
    ///
    /// \returns the number of data points.
    inline std::size_t kdtree_get_point_count() const
    {
        return _points.size();
    }

    /// \brief Get distance between the given vector and one in the point cloud.
    ///
    /// This method is an interface requirement for nanoflann point cloud.
    ///
    /// \param pVector a pointer to the 0th element of a vector.
    /// \param index The index of a vector in our referenced point cloud.
    /// \param dim The number of dimensions in our VectorType.
    /// \returns The accumulated Euclidean distance between two vectors.
    inline FloatType kdtree_distance(const FloatType* pVector,
                                     const std::size_t index,
                                     const std::size_t dim) const
    {
        FloatType total = 0;

        for (std::size_t i = 0; i < dim; ++i)
        {
            const FloatType distance = pVector[i] - _points[index][i];

            total += (distance * distance);
        }

        return total;
    }

    /// \brief Get the dim'th component of the idx'th vector in the point cloud.
    ///
    /// This method is an interface requirement for nanoflann point cloud.
    ///
    /// \param index The index of the vector in the point cloud.
    /// \param dimension The dimension (i.e. 0 = x, 1 = y, 2 = z, etc) to get.
    /// \returns the FloatType value from the vector.
    inline FloatType kdtree_get_pt(const std::size_t index,
                                   int dimension) const
    {
        return _points[index][dimension];
    }

    /// \brief The adapter allows users to pre-compute bounding boxes.
    ///
    /// This method is an interface requirement for nanoflann point cloud.
    ///
    /// This feature is currently unimplemented.
    ///
    /// \tparam BoundingBox The bounding box type to fill.
    /// \param boundingBox The bounding box type container to fill.
    /// \returns true if a valid bounding box was returned.
    template <class BoundingBox>
    bool kdtree_get_bbox(BoundingBox& boundingBox) const
    {
        return false;
    }

    enum
    {
        /// \brief The default maximum leaf size.
        DEFAULT_MAX_LEAF_SIZE = 10,
    };


protected:
    /// \brief Const reference to the points.
    const std::vector<VectorType>& _points;

    /// \brief The KDTree structure.
    KDTreeAdapter _KDTree;

};


} // namespace ofx
