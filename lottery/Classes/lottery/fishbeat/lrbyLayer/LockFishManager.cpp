
#include "LockFishManager.h"
#include "FishManager.h"
//#include "math_aide.h"

const float kLockLineInterval = 64.f;

const float kRotateRadius = 10.f;
const float kRotateAngle = 10.f * M_PI / 180.f;

LockFishManager::LockFishManager() : 
	rotate_angle_(0.f)
{
  for (WORD i = 0; i < BY_GAME_PLAYER; ++i) {
    lock_fish_id_[i] = 0;
    lock_fish_kind_[i] = FISH_KIND_COUNT;
	lock_line_trace_[i] = Vec2::ZERO;
	lock_line_start[i] = Vec2::ZERO;
	lock_fish_is_enter[i] = false;
	m_LineLayer[i] = NULL;
	m_LockFlag[i] = NULL;
  }
}

LockFishManager::~LockFishManager() {
}

bool LockFishManager::init()
{
	return true;
}

bool LockFishManager::LoadGameResource() {
  //hgeResourceManager* resource_manager = GameManager::GetInstance().GetResourceManager();
  //ani_lock_flag_ = resource_manager->GetAnimation("lock_flag");
  //spr_lock_line_ = resource_manager->GetSprite("lock_line");
  //char file_name[MAX_PATH] = { 0 };
  //for (WORD i = 0; i < GAME_PLAYER; ++i) {
  //  _snprintf(file_name, arraysize(file_name), "lock_flag_%d", i + 1);
  //  spr_lock_flag_[i] = resource_manager->GetSprite(file_name);
  //}
  return true;
}

bool LockFishManager::OnFrame(float delta_time) {
  //CannonManager* cannon_manager = GameManager::GetInstance().GetCannonManager();
  //for (WORD i = 0; i < GAME_PLAYER; ++i) {
  //  if (lock_fish_id_[i] == 0) continue;
  //  FPoint cannon_pos = cannon_manager->GetCannonPos(i);
  //  FPoint mouse_pos = LockPos(i);
  //  float angle = MathAide::CalcAngle(mouse_pos.x, mouse_pos.y, cannon_pos.x, cannon_pos.y);
  //  cannon_manager->SetCurrentAngle(i, angle);
  //}

  //rotate_angle_ += kRotateAngle;
  //if (rotate_angle_ > M_PI * 2) {
  //  rotate_angle_ -= M_PI * 2;
  //}

  return false;
}

bool LockFishManager::OnRender(float offset_x, float offset_y, float hscale, float vscale) {
  /*for (WORD i = 0; i < GAME_PLAYER; ++i) {
    if (lock_fish_id_[i] == 0) continue;
    if (lock_line_trace_[i].size() == 0) continue;
    float rotate_x = (kPosLockFlag[i].x + kRotateRadius * cosf(rotate_angle_)) * hscale;
    float rotate_y = (kPosLockFlag[i].y + kRotateRadius * sinf(rotate_angle_)) * vscale;
    ani_lock_flag_->SetFrame(lock_fish_kind_[i]);
    ani_lock_flag_->RenderEx(rotate_x, rotate_y, kChairDefaultAngle[i], hscale, vscale);
    for (FPointVector::size_type j = 0; j < lock_line_trace_[i].size(); ++j) 
	{
      if (j == lock_line_trace_[i].size() - 1) 
	  {
        spr_lock_flag_[i]->RenderEx(offset_x + lock_line_trace_[i][j].x, offset_y + lock_line_trace_[i][j].y,
            kChairDefaultAngle[i], hscale, hscale);
      }
	  else 
	  {
        spr_lock_line_->RenderEx(offset_x + lock_line_trace_[i][j].x, offset_y + lock_line_trace_[i][j].y,
            kChairDefaultAngle[i], hscale, hscale);
      }
    }
  }*/

  return false;
}

void LockFishManager::UpdateLockTrace(WORD chair_id, float fish_pos_x, float fish_pos_y) {

}

void LockFishManager::ClearLockTrace(WORD chair_id) {
  lock_line_trace_[chair_id] = Vec2::ZERO;
  lock_line_start[chair_id] = Vec2::ZERO;
  lock_fish_id_[chair_id] = 0;
  lock_fish_kind_[chair_id] = FISH_KIND_COUNT;
  lock_fish_is_enter[chair_id] = false;
  setLockLineVisible(chair_id, false);
}

Vec2 LockFishManager::LockPos(WORD chair_id) {
  if (lock_line_trace_[chair_id] == NULL) {
    Vec2 pos = Vec2::ZERO;
    return pos;
  } else {
    return lock_line_trace_[chair_id];
  }
}

void LockFishManager::createLockLine(int chairId, Vec2 pStart, Vec2 pEnd)
{
	if (lock_fish_id_[chairId] == 0 || lock_fish_kind_[chairId] == FISH_KIND_COUNT)
	{
		this->setLockLineVisible(chairId, false);
		return;
	}
	lock_line_trace_[chairId] = pEnd;
	lock_line_start[chairId] = pStart;
	if (m_LineLayer[chairId] == NULL)
	{
		m_LineLayer[chairId] = LockLineLayer::create();
		this->addChild(m_LineLayer[chairId]);
	}
	m_LineLayer[chairId]->setPointStartAndEnd(pStart, pEnd);

	String name;
	name.initWithFormat("709/LockView/lock_flag_%d.png", chairId);
	if (m_LockFlag[chairId] == NULL)
	{
		m_LockFlag[chairId] = Sprite::create(name.getCString());
		this->addChild(m_LockFlag[chairId]);
	}
	else
	{
		m_LockFlag[chairId]->setTexture(name.getCString());
	}
	m_LockFlag[chairId]->setPosition(pEnd);

	this->setLockLineVisible(chairId, true);
}

void LockFishManager::setLockLineVisible(int chair_id, bool visible)
{
	if (chair_id >= BY_GAME_PLAYER)
	{
		return;
	}
	if (m_LockFlag[chair_id])
	{
		m_LockFlag[chair_id]->setVisible(visible);
	}
	if (m_LineLayer[chair_id])
	{
		m_LineLayer[chair_id]->setVisible(visible);
	}
}
