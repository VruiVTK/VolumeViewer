#ifndef VOLAPPLICATIONSTATE_H
#define VOLAPPLICATIONSTATE_H

#include <vvApplicationState.h>

#include <vtkTimeStamp.h>

#include <string>
#include <vector>

class volGeometry;
class volOutline;
class volReader;

/**
 * @brief The volApplicationState class holds VolumeViewer application state.
 */
class volApplicationState : public vvApplicationState
{
public:
  using Superclass = vvApplicationState;
  volApplicationState();
  ~volApplicationState();

  /** Geometry rendering */
  volGeometry& geometry() { return *m_geometry; }
  const volGeometry& geometry() const { return *m_geometry; }

  /** Outline rendering */
  volOutline &outline() { return *m_outline; }
  const volOutline &outline() const { return *m_outline; }

  /** File reader. */
  volReader& reader() { return *m_reader; }
  const volReader& reader() const { return *m_reader; }

private:
  // Not implemented:
  volApplicationState(const volApplicationState&);
  volApplicationState& operator=(const volApplicationState&);

  volGeometry *m_geometry;
  volOutline *m_outline;
  volReader *m_reader;
};

#endif // VOLAPPLICATIONSTATE_H
