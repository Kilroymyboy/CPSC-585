#pragma once

struct FilterGroup {
	enum Enum {
		eAventador0 = (1 << 0),
		eAventador1 = (1 << 1),
		ePowerUp0 = (1 << 2),
		ePowerUp1 = (1 << 3),
	};
};


struct ContactModFlags {
	enum {
		eIGNORE_CONTACT = (1 << 0),
	};
};
