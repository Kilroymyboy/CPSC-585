#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PxPhysicsAPI.h"
#include "extensions\PxRigidBodyExt.h"
#include <vector>

namespace Util {
	physx::PxVec3 g2p(glm::vec3);
	glm::vec3 p2g(physx::PxVec3);
}
