
#include "TT_SwitchSceneElement.h"

inline void FQueued_SwitchSceneElement::DoThreadedWork()
{
}

inline void FQueued_SwitchSceneElement::Abandon()
{
	delete this;
}
