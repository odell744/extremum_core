#pragma once
#ifndef EXTREMUM_VEHICLE_INFO_STRUCTURE
#define EXTREMUM_VEHICLE_INFO_STRUCTURE

#include "../common.hpp"

namespace extremum::menu {
	namespace info_structs {
		enum vehicle_classes {
			VEHICLE_CLASS_COMPACT,
			VEHICLE_CLASS_SEDAN,
			VEHICLE_CLASS_SUV,
			VEHICLE_CLASS_COUPES,
			VEHICLE_CLASS_MUSCLE,
			VEHICLE_CLASS_SPORT_CLASSIC,
			VEHICLE_CLASS_SPORT,
			VEHICLE_CLASS_SUPER,
			VEHICLE_CLASS_MOTORCYCLE,
			VEHICLE_CLASS_OFFROAD,
			VEHICLE_CLASS_INDUSTRIAL,
			VEHICLE_CLASS_UTILITY,
			VEHICLE_CLASS_VAN,
			VEHICLE_CLASS_CYCLE,
			VEHICLE_CLASS_BOAT,
			VEHICLE_CLASS_HELICOPTER,
			VEHICLE_CLASS_PLANE,
			VEHICLE_CLASS_SERVICE,
			VEHICLE_CLASS_EMERGENCY,
			VEHICLE_CLASS_MILITARY,
			VEHICLE_CLASS_COMMERCIAL,
			VEHICLE_CLASS_TRAIN,
			VEHICLE_CLASS_OPEN_WHEEL
		};
		enum vehicle_dlc {
			VEHICLE_DLC_NONE,
		};
		struct vehicle_info {
			std::uint32_t	m_hash;
			std::string		m_rage_name;
			std::string		m_name;
			vehicle_classes m_class;
			vehicle_dlc		m_dlc;

			// sort tags for boost::multi_index
			struct ByName; 
			struct ByClass;
			struct ByDLC;
		};
	};
};

#endif // !EXTREMUM_VEHICLE_INFO_STRUCTURE
