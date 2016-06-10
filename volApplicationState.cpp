#include "volApplicationState.h"

#include "volReader.h"

volApplicationState::volApplicationState()
  : Superclass(),
    m_reader(new volReader)
{
}

volApplicationState::~volApplicationState()
{
  delete m_reader;
}
