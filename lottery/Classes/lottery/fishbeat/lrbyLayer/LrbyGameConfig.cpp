#include "LrbyGameConfig.h"

using namespace NS_lrbyFish;

CLrbyGameConfig::CLrbyGameConfig(void)
{
}


CLrbyGameConfig::~CLrbyGameConfig(void)
{

}

void CLrbyGameConfig::setGameConfig(const struct CMD_S_GameConfig* config)
{
	m_gameConfig.exchange_ratio_userscore = config->exchange_ratio_userscore;
	m_gameConfig.exchange_ratio_fishscore = config->exchange_ratio_fishscore;
	m_gameConfig.exchange_count = config->exchange_count;

	m_gameConfig.min_bullet_multiple = config->min_bullet_multiple;
	m_gameConfig.max_bullet_multiple = config->max_bullet_multiple;
	//verysharp //m_gameConfig.cannonNumber = config->cannonNumber;

	m_gameConfig.bomb_range_width = config->bomb_range_width;
	m_gameConfig.bomb_range_height = config->bomb_range_height;

	for (int i = 0; i < FISH_KIND_COUNT; ++i)
	{
		m_gameConfig.fish_multiple[i] = config->fish_multiple[i];
		m_gameConfig.fish_speed[i] = static_cast<float>(config->fish_speed[i]);
		m_gameConfig.fish_bounding_box_width[i] = static_cast<float>(config->fish_bounding_box_width[i]);
		m_gameConfig.fish_bounding_box_height[i] = static_cast<float>(config->fish_bounding_box_height[i]);
		m_gameConfig.fish_hit_radius[i] = static_cast<float>(config->fish_hit_radius[i]);
	}
	for (int i = 0; i < BULLET_KIND_COUNT; ++i)
	{
		m_gameConfig.bullet_speed[i] = static_cast<float>(config->bullet_speed[i]);
		m_gameConfig.net_radius[i] = static_cast<float>(config->net_radius[i]);
	}

	//m_gameConfig.lock_percent = config->lock_percent;
	//m_gameConfig.lostlock_percent = config->lostlock_percent;
	//m_gameConfig.firetime_ = config->firetime_;
	//m_gameConfig.fish_time = config->fish_time;
	//m_gameConfig.max_bullet_num = config->max_bullet_num;

	//m_gameConfig.fishL_max_multiple = config->fishL_max_multiple;
	//m_gameConfig.fishLK_max_multiple = config->fishLK_max_multiple;

}

bool CLrbyGameConfig::IsSwitchingScene()
{
	return m_bSwitchingScene;
}
void CLrbyGameConfig::setSwitchingScene(bool bSwitch)
{
	m_bSwitchingScene = bSwitch;
}
