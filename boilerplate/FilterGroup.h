#pragma once

struct FilterGroup {
	enum Enum {
		eAventador = (1 << 0),
		ePowerUp = (1 << 1),
	};
};


struct ContactModFlags {
	enum {
		eIGNORE_CONTACT = (1 << 0),
		eTARGET_VELOCITY = (1 << 1),
	};
};
