#ifndef EXTREMUM_VEHICLE_ARRAY
#define EXTREMUM_VEHICLE_ARRAY

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include "../info_structs/vehicle_info.hpp"

using namespace ::boost;
using namespace ::boost::multi_index;
using extremum::menu::info_structs::vehicle_dlc;
using extremum::menu::info_structs::vehicle_classes;
using extremum::menu::info_structs::vehicle_info;
namespace extremum::menu {
	namespace massives {
		class vehicles_array : boost::noncopyable {
		private:
			typedef multi_index_container <
				vehicle_info,
				indexed_by<
					ordered_unique<
						tag<vehicle_info::ByName>,
						member<vehicle_info, std::string, &vehicle_info::m_name>
					>,
					ordered_non_unique<
						tag<vehicle_info::ByDLC>,
						member<vehicle_info, vehicle_dlc, &vehicle_info::m_dlc>
					>,
					ordered_non_unique<
						tag<vehicle_info::ByClass>,
						member<vehicle_info, vehicle_classes, &vehicle_info::m_class>
					>,
					ordered_unique<
						tag<vehicle_info::IndexByName>,
						member<vehicle_info, std::string, &vehicle_info::m_name>
					>
				>
			> vehicle_array_t;
			static const vehicle_array_t m_vehicles;

			typedef vehicle_array_t::index<vehicle_info::ByName>::type	vehicle_array_by_name_t;
			typedef vehicle_array_t::index<vehicle_info::ByClass>::type	vehicle_array_by_class_t;
			typedef vehicle_array_t::index<vehicle_info::ByDLC>::type	vehicle_array_by_dlc_t;
		public:
			vehicles_array();
			~vehicles_array();

			const vehicle_array_by_name_t&		get_sorted_by_name() const;
			const vehicle_array_by_class_t&		get_sorted_by_class() const;
			const vehicle_array_by_dlc_t&		get_sorted_by_dlc() const;
		};

		vehicles_array* g_vehicles;
	};
};

#endif // !EXTREMUM_VEHICLE_ARRAY
