#include "DogeeBase.h"

namespace Dogee
{
	ObjectKey objid;
	uint64_t data[4096 * 32];
	thread_local DObject* lastobject = nullptr;
}