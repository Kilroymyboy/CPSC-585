#include "Util.h"

using namespace glm;
using namespace physx;

namespace Util {
	physx::PxVec3 g2p(glm::vec3 v) {
		return PxVec3(v.x, v.y, v.z);
	}

	glm::vec3 p2g(physx::PxVec3 v) {
		return vec3(v.x, v.y, v.z);
	}
}
