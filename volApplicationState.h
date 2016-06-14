#ifndef VOLAPPLICATIONSTATE_H
#define VOLAPPLICATIONSTATE_H

#include <vvApplicationState.h>

#include <vtkTimeStamp.h>

#include <string>
#include <vector>

class volGeometry;
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

  /** File reader. */
  volReader& reader() { return *m_reader; }
  const volReader& reader() const { return *m_reader; }

private:
  // Not implemented:
  volApplicationState(const volApplicationState&);
  volApplicationState& operator=(const volApplicationState&);

  volGeometry *m_geometry;
  volReader *m_reader;
};

#endif // VOLAPPLICATIONSTATE_H
