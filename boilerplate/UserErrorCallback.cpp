#include "Physics.h"
using namespace physx;


class UserErrorCallback : public PxErrorCallback
{
public:
	void reportError(PxErrorCode::Enum code, const char* message, const char* file,
		int line)
	{
		// error processing implementation
	}
};