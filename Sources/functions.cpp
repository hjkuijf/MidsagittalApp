#include "functions.h"

void normalize(const std::vector<MLuint>& histogram, std::vector<MLdouble>& probability, const MLuint& numberOfPixels)
{
  const MLuint histogramSize(histogram.size());
  probability.resize(histogramSize);
  for (MLuint i = 0; i < histogramSize; ++i) {
    probability[i] = (MLdouble)histogram[i] / numberOfPixels;
  }
}

MLdouble kullbackLeibler(const std::vector<MLdouble> &p, const std::vector<MLdouble> &q)
{
  MLdouble KL = 0;
  const MLuint pSize(p.size());
  for (MLuint i = 0; i < pSize; ++i) { //p and q should be equal in size!
    if (p[i] != 0 && q[i] != 0) {
      KL += p[i] * log(p[i] / q[i]);
    }
  }
  return KL;
}

ml::Plane transformPlane( const ml::Plane& inputPlane, const ml::Matrix4& matrix )
{
  const ml::Vector3 normal(inputPlane.getNormal());
  const MLdouble distanceFromOrigin(inputPlane.getDistanceFromOrigin());    

  const auto getVector = [&](MLdouble x, MLdouble y, const ml::Vector3& normal, MLdouble distanceFromOrigin) { return ml::Vector3(x, y, (x * normal.x + y * normal.y - distanceFromOrigin) / -normal.z); };

  // Get three points that lie on the plane
  ml::Vector3 point1(getVector(0, 0, normal, distanceFromOrigin));
  ml::Vector3 point2(getVector(0, 1, normal, distanceFromOrigin));
  ml::Vector3 point3(getVector(1, 0, normal, distanceFromOrigin));
  
  // Transform the points to the new world
  point1 = matrix.transformPoint(point1);
  point2 = matrix.transformPoint(point2);
  point3 = matrix.transformPoint(point3);

  // Compute a new normal vector for the plane
  ml::Vector3 newNormal((point2 - point1).cross(point2 - point3));
  newNormal.normalize();

  // Compute the new distance for the plane
  const ml::Vector3 point0(matrix.transformPoint(normal * distanceFromOrigin));
  const MLdouble newDistanceFromOrigin(point0.dot(newNormal));

  return ml::Plane(newNormal, newDistanceFromOrigin);
}
