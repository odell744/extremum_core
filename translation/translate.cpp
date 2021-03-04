#include "translate.hpp"

extremum::core::translation::translation()
{
	try {
		LOG_INFO("parsing english");
		pt::read_json("C:\\EXFiles\\Translation\\English.json", m_english);
		LOG_INFO("parsing russian");
		pt::read_json("C:\\EXFiles\\Translation\\Russian.json", m_russian);
		LOG_INFO("parsing chinese");
		pt::read_json("C:\\EXFiles\\Translation\\Chinese.json", m_chinese);
		LOG_INFO("parsing korean");
		pt::read_json("C:\\EXFiles\\Translation\\Korean.json", m_korean);
	}
	catch (std::exception& ex) {
		LOG_ERROR(ex.what());
	}
	m_translation_file = &m_russian;
	m_translation = this;
}


extremum::core::eLanguage extremum::core::translation::get_current_language()
{
	return m_current_language;
}
void extremum::core::translation::set_current_language(eLanguage lang)
{
	m_current_language = lang;
	switch (m_current_language)
	{
	case extremum::core::English:
		m_translation_file = &m_english;
		break;
	case extremum::core::Russian:
		m_translation_file = &m_russian;
		break;
	case extremum::core::Chinese:
		m_translation_file = &m_chinese;
		break;
	case extremum::core::Korean:
		m_translation_file = &m_korean;
		break;
	default:
		m_current_language = English;
		m_translation_file = &m_english;
		break;
	}
}

std::string extremum::core::translation::find_translation(std::string translation_key)
{
	std::string ret = translation_key;
	if (m_translation_file->find(translation_key) != m_translation_file->not_found())
		ret = m_translation_file->get<std::string>(translation_key);
	return ret;
}

extremum::core::translation::~translation()
{
	m_translation = nullptr;
}