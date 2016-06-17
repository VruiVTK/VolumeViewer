#ifndef VOLAPPLICATIONSTATE_H
#define VOLAPPLICATIONSTATE_H

#include <vvApplicationState.h>

#include <vtkTimeStamp.h>

#include <array>
#include <string>
#include <vector>

class volGeometry;
class volOutline;
class volReader;
class volVolume;

/**
 * @brief The volApplicationState class holds VolumeViewer application state.
 */
class volApplicationState : public vvApplicationState
{
public:
  using Superclass = vvApplicationState;
  using ColorMapType = std::array<double, 4 * 256>; //! 256 RGBA [0., 1.] values

  volApplicationState();
  ~volApplicationState();

  /** RGBA color map for geometry/volume rendering. See usage for details. */
  ColorMapType& colorMap() { return m_colorMap; }
  const ColorMapType& colorMap() const { return m_colorMap; }

  /** Geometry rendering */
  volGeometry& geometry() { return *m_geometry; }
  const volGeometry& geometry() const { return *m_geometry; }

  /** Outline rendering */
  volOutline &outline() { return *m_outline; }
  const volOutline &outline() const { return *m_outline; }

  /** File reader. */
  volReader& reader() { return *m_reader; }
  const volReader& reader() const { return *m_reader; }

  /** Volume rendering */
  volVolume& volume() { return *m_volume; }
  const volVolume& volume() const { return *m_volume; }

private:
  // Not implemented:
  volApplicationState(const volApplicationState&);
  volApplicationState& operator=(const volApplicationState&);

  ColorMapType m_colorMap;
  volGeometry *m_geometry;
  volOutline *m_outline;
  volReader *m_reader;
  volVolume *m_volume;
};

#endif // VOLAPPLICATIONSTATE_H
