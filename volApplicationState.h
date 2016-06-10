#ifndef VOLAPPLICATIONSTATE_H
#define VOLAPPLICATIONSTATE_H

#include <vvApplicationState.h>

#include <vtkTimeStamp.h>

#include <string>
#include <vector>

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

  /** File reader. */
  volReader& reader() { return *m_reader; }
  const volReader& reader() const { return *m_reader; }

private:
  // Not implemented:
  volApplicationState(const volApplicationState&);
  volApplicationState& operator=(const volApplicationState&);

  volReader *m_reader;
};

#endif // VOLAPPLICATIONSTATE_H
