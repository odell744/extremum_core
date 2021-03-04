#ifndef EXTREMUM_TRANSLATE
#define EXTREMUM_TRANSLATE

#include "../common.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

namespace extremum
{
	namespace core {

		enum eLanguage : std::uint8_t
		{
			English = 0,
			Russian = 1,
			Chinese = 2,
			Korean = 3
		};

		class translation : public boost::noncopyable
		{
		private:
			// json parsers
			pt::ptree		m_english;
			pt::ptree		m_russian;
			pt::ptree		m_chinese;
			pt::ptree		m_korean;
			pt::ptree*		m_translation_file;
			// current language
			eLanguage	m_current_language{ Russian };
		public:

			translation();
			~translation();

			void		set_current_language(eLanguage lang);
			eLanguage   get_current_language();
			std::string find_translation(std::string translation_key);
		};

		inline translation* m_translation;
	}
}

#define GET_CURRENT_LANGUAGE				extremum::core::m_translation->get_current_language()
#define SET_CURRENT_LANGUAGE(lang)			extremum::core::m_translation->set_current_language(lang)
#define SET_CURRENT_LANGUAGE_ENG			SET_CURRENT_LANGUAGE(extremum::core::English)
#define SET_CURRENT_LANGUAGE_RUS			SET_CURRENT_LANGUAGE(extremum::core::Russian)
#define SET_CURRENT_LANGUAGE_CHN			SET_CURRENT_LANGUAGE(extremum::core::Chinese)
#define SET_CURRENT_LANGUAGE_KOR			SET_CURRENT_LANGUAGE(extremum::core::Korean)
#define TRANSLATE(translation_key)			extremum::core::m_translation->find_translation(translation_key)

#endif // !EXTREMUM_TRANSLATE
