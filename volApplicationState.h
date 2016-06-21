#ifndef VOLAPPLICATIONSTATE_H
#define VOLAPPLICATIONSTATE_H

#include <vvApplicationState.h>

#include <vtkTimeStamp.h>

#include <array>
#include <string>
#include <vector>

class volContours;
class volGeometry;
class volIsosurface;
class volOutline;
class volSlices;
class volReader;
class volVolume;

/**
 * @brief The volApplicationState class holds VolumeViewer application state.
 */
class volApplicationState : public vvApplicationState
{
public:
  using Superclass = vvApplicationState;
  using ColorMap = std::array<double, 4 * 256>; //! 256 RGBA [0., 1.] values

  volApplicationState();
  ~volApplicationState();

  /** RGBA color map for geometry/volume rendering. See usage for details. */
  ColorMap& colorMap() { return m_colorMap; }
  const ColorMap& colorMap() const { return m_colorMap; }
  void colorMapModified() { m_colorMapTimeStamp.Modified(); }
  unsigned long int colorMapTimeStamp() const;

  /** Contour rendering */
  volContours& contours() { return *m_contours; }
  const volContours& contours() const { return *m_contours; }

  /** Geometry rendering */
  volGeometry& geometry() { return *m_geometry; }
  const volGeometry& geometry() const { return *m_geometry; }

  /** Isosurface rendering */
  volIsosurface& isosurfaceA() { return *m_isosurfaces[0]; }
  const volIsosurface& isosurfaceA() const { return *m_isosurfaces[0]; }
  volIsosurface& isosurfaceB() { return *m_isosurfaces[1]; }
  const volIsosurface& isosurfaceB() const { return *m_isosurfaces[1]; }
  volIsosurface& isosurfaceC() { return *m_isosurfaces[2]; }
  const volIsosurface& isosurfaceC() const { return *m_isosurfaces[2]; }

  /** RGBA color map for geometry/volume rendering. See usage for details. */
  ColorMap& isosurfaceColorMap() { return m_isosurfaceColorMap; }
  const ColorMap& isosurfaceColorMap() const { return m_isosurfaceColorMap; }
  void isosurfaceColorMapModified() { m_isosurfaceColorMapTimeStamp.Modified(); }
  unsigned long int isosurfaceColorMapTimeStamp() const;

  /** Outline rendering */
  volOutline &outline() { return *m_outline; }
  const volOutline &outline() const { return *m_outline; }

  /** File reader. */
  volReader& reader() { return *m_reader; }
  const volReader& reader() const { return *m_reader; }

  /** Slice rendering. */
  volSlices& slices() { return *m_slices; }
  const volSlices& slices() const { return *m_slices; }

  /** RGBA color map for geometry/volume rendering. See usage for details. */
  ColorMap& sliceColorMap() { return m_sliceColorMap; }
  const ColorMap& sliceColorMap() const { return m_sliceColorMap; }
  void sliceColorMapModified() { m_sliceColorMapTimeStamp.Modified(); }
  unsigned long int sliceColorMapTimeStamp() const;

  /** Volume rendering */
  volVolume& volume() { return *m_volume; }
  const volVolume& volume() const { return *m_volume; }

private:
  // Not implemented:
  volApplicationState(const volApplicationState&);
  volApplicationState& operator=(const volApplicationState&);

  ColorMap m_colorMap;
  vtkTimeStamp m_colorMapTimeStamp;
  volContours *m_contours;
  volGeometry *m_geometry;
  std::array<volIsosurface*, 3> m_isosurfaces;
  ColorMap m_isosurfaceColorMap;
  vtkTimeStamp m_isosurfaceColorMapTimeStamp;
  volOutline *m_outline;
  volReader *m_reader;
  volSlices *m_slices;
  ColorMap m_sliceColorMap;
  vtkTimeStamp m_sliceColorMapTimeStamp;
  volVolume *m_volume;
};

inline unsigned long int volApplicationState::colorMapTimeStamp() const
{
  return m_colorMapTimeStamp.GetMTime();
}

inline unsigned long volApplicationState::isosurfaceColorMapTimeStamp() const
{
  return m_isosurfaceColorMapTimeStamp.GetMTime();
}

inline unsigned long int volApplicationState::sliceColorMapTimeStamp() const
{
  return m_sliceColorMapTimeStamp.GetMTime();
}

#endif // VOLAPPLICATIONSTATE_H
