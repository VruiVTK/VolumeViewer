#ifndef VOLCONTEXTSTATE_H
#define VOLCONTEXTSTATE_H

#include <vvContextState.h>

/**
 * @brief The volContextState class holds context-specific information for
 * VolumeViewer.
 *
 * At the moment, all functionality is carried out by the superclass. This
 * subclass does nothing.
 */
class volContextState : public vvContextState
{
public:
  volContextState();
};

#endif // VOLCONTEXTSTATE_H
