#ifndef Tracker_MeasurementEstimator_H
#define Tracker_MeasurementEstimator_H

#include "DataFormats/GeometryVector/interface/Vector2DBase.h"
#include "DataFormats/GeometryVector/interface/LocalTag.h"
#include <utility>

class Plane;
class TrajectoryStateOnSurface;
class Surface;
class TrackingRecHit;

/** The MeasurementEstimator defines the compatibility of a 
 *  TrajectoryStateOnSurface and a RecHit, and of a 
 *  TrajectoryStateOnSurface and a Plane.
 *  It is used in the Det interface to obtain compatible measurements.
 */


class MeasurementEstimator {
public:

  struct OpaquePayload { virtual ~OpaquePayload(){} int tag=0;};

  using Local2DVector = Vector2DBase< float, LocalTag>;


  MeasurementEstimator() {}
  MeasurementEstimator(float maxSag, float minToll) :
     m_maxSagita(maxSag),
     m_minTollerance2(minToll*minToll){}

  virtual ~MeasurementEstimator() {}

  using HitReturnType     = std::pair<bool,double>;
  using SurfaceReturnType = bool;

  /** Returns pair( true, value) if the TrajectoryStateOnSurface is compatible
   *  with the RecHit, and pair( false, value) if it is not compatible.
   *  The TrajectoryStateOnSurface must be on the same Surface as the RecHit. 
   *  For an estimator where there is no value computed, e.g. fixed
   *  window estimator, only the first(bool) part is of interest.
   */
  virtual HitReturnType estimate( const TrajectoryStateOnSurface& ts, 
				  const TrackingRecHit& hit) const = 0;

  /* verify the compatibility of the Hit with the Trajectory based
   * on hit properties other than those used in estimate 
   * (that usually computes the compatibility of the Trajectory with the Hit)
   * 
   */
  virtual bool preFilter(const TrajectoryStateOnSurface&, OpaquePayload const &) const { return true;}


  /** Returns true if the TrajectoryStateOnSurface is compatible with the
   *  Plane, false otherwise.
   *  The TrajectoryStateOnSurface must be on the plane.
   */
  virtual SurfaceReturnType estimate( const TrajectoryStateOnSurface& ts, 
				      const Plane& plane) const = 0;

  virtual MeasurementEstimator* clone() const = 0;

  /** Returns the size of the compatibility region around the local position of the 
   *  TrajectoryStateOnSurface along the directions of local x and y axis.
   *  The TrajectoryStateOnSurface must be on the plane.
   *  This method allows to limit the search for compatible detectors or RecHits.
   *  The MeasurementEstimator should not return "true" for any RecHit or
   *  Plane which is entirely outside of the compatibility region defined 
   *  by maximalLocalDisplacement().
   */
  virtual Local2DVector 
  maximalLocalDisplacement( const TrajectoryStateOnSurface& ts,
			    const Plane& plane) const=0;

  float maxSagita() const { return m_maxSagita;}
  float	minTollerance2() const { return m_minTollerance2;}


private:
  float m_maxSagita=-1.; // maximal sagita for linear approximation
  float m_minTollerance2=100.; // square of minimum tollerance ot be considered inside a detector

};

#endif // Tracker_MeasurementEstimator_H
