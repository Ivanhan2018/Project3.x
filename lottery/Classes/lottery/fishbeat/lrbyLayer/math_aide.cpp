
#include "math_aide.h"
#include "math.h"

using namespace NS_lrbyFish;

// 直接初始11个阶乘的数值
static const int g_FactorialArr[11]={1,1,2,6,24,120,720,5040,40320,362880,3628800};
//  10 下面的组合
static const int g_CombinationArr[11][11] =
{{1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1},
{1,2,1,1,1,1,1,1,1,1,1},
{1,3,3,1,1,1,1,1,1,1,1},
{1,4,6,4,1,1,1,1,1,1,1},
{1,5,10,10,5,1,1,1,1,1,1},
{1,6,15,20,15,6,1,1,1,1,1},
{1,7,21,35,35,21,7,1,1,1,1},
{1,8,28,56,70,56,28,8,1,1,1},
{1,9,36,84,126,126,84,36,9,1,1},
{1,10,45,120,210,252,210,120,45,10,1}
};

////  10 下面的所有阶乘都计算好
//void TenFactorial()  
//{
//	//g_FactorialArr[0]=1;
//	//g_FactorialArr[1]=1;
//	//for (int i=2; i<=10; ++i)
//	//{
//	//	g_FactorialArr[i]=g_FactorialArr[i-1]*i;
//	//}
//	//for (int i=0; i<11; ++i)
//	//{
//	//	for (int j=0; j<=i; ++j)
//	//	{
//	//		g_CombinationArr[i][j]=MathAide::Combination(i, j);
//	//	}
//	//}
//	//int ix= 0;
//
//	// test
//	for (int i=0; i<11; i++)
//	{
//		if (g_FactorialArr[i] != MathAide::Factorial(i))
//		{
//			int err = 0;
//		}
//		for (int j=0; j<=i; ++j)
//		{
//			if (g_CombinationArr[i][j] != MathAide::Combination(i, j))
//			{
//				int err =0;
//			}
//		}
//	}
//}

//struct FctAuto
//{
//	char fa;
//	FctAuto()
//	{
//		TenFactorial();
//	}
//};
//static FctAuto g_FctAuto;

// 把10一下的阶乘先算好
MathAide::MathAide() {
}

int MathAide::Factorial(int number)
{
  int factorial = 1;
  int temp = number;
  for (int i = 0; i < number; ++i) 
  {
    factorial *= temp;
    --temp;
  }
  return factorial;
}

int MathAide::Combination(int count, int r)
{
    if (count<0 || r<0)
	{
		return 1;
	}
	if (count<11 && r<11)
	{
		return g_CombinationArr[count][r];
	}
  return Factorial(count) / (Factorial(r) * Factorial(count - r));
}

#define Calc_Distance(x1,y1,x2,y2)  (sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)))

float MathAide::CalcDistance(float x1, float y1, float x2, float y2)
{
  return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

float MathAide::CalcAngle(float x1, float y1, float x2, float y2)
{
  float distance = Calc_Distance(x1, y1, x2, y2);
  if (distance == 0.f) 
	  return 0.f;
  float sin_value = (x1 - x2) / distance;
  float angle = acosf(sin_value);
  if (y1 < y2)
	  angle= 2 * M_PI - angle;
  return angle;
}

void MathAide::BuildLinear(float init_x[], float init_y[], int init_count, std::vector<FPoint>& trace_vector, float distance) {
  trace_vector.clear();

  if (init_count < 2) return;
  if (distance <= 0.0f) return;

  float distance_total = Calc_Distance(init_x[init_count - 1], init_y[init_count - 1], init_x[0], init_y[0]);
  if (distance_total <= 0.0f) return;

  float cos_value = abs(init_y[init_count - 1] - init_y[0]) / distance_total;
  float angle = acosf(cos_value);

  FPoint point;
  point.x = init_x[0];
  point.y = init_y[0];
  trace_vector.push_back(point);
  float temp_distance = 0.f;

  std::vector<FPoint>::size_type size;
  while (temp_distance < distance_total)
  {
    size = trace_vector.size();

    if (init_x[init_count - 1] < init_x[0])
	{
      point.x = init_x[0] - sinf(angle) * (distance * size);
    } 
	else
	{
      point.x = init_x[0] + sinf(angle) * (distance * size);
    }

    if (init_y[init_count - 1] < init_y[0])
	{
      point.y = init_y[0] - cosf(angle) * (distance * size);
    } 
	else
	{
      point.y = init_y[0] + cosf(angle) * (distance * size);
    }

    trace_vector.push_back(point);
    temp_distance = Calc_Distance(point.x, point.y, init_x[0], init_y[0]);
  }

  FPoint& temp_point = trace_vector.back();
  temp_point.x = init_x[init_count - 1];
  temp_point.y = init_y[init_count - 1];
}

void MathAide::BuildLinear(float init_x[], float init_y[], int init_count, std::vector<FPointAngle>& trace_vector, float distance)
{
  trace_vector.clear();

  if (init_count < 2) 
	  return;
  if (distance <= 0.0f) 
	  return;

  float distance_total = Calc_Distance(init_x[init_count - 1], init_y[init_count - 1], init_x[0], init_y[0]);
  if (distance_total <= 0.0f) 
	  return;

  float cos_value = abs(init_y[init_count - 1] - init_y[0]) / distance_total;
  float temp_angle = acosf(cos_value);

  FPointAngle point;
  point.x = init_x[0];
  point.y = init_y[0];
  point.angle = 1.f;
  trace_vector.push_back(point);
  float temp_distance = 0.f;

  FPointAngle temp_pos = { point.x ,point.y, 0.f };

  std::vector<FPointAngle>::size_type size;
  while (temp_distance < distance_total) 
  {
    size = trace_vector.size();

    if (init_x[init_count - 1] < init_x[0])
	{
      point.x = init_x[0] - sinf(temp_angle) * (distance * size);
    } 
	else 
	{
      point.x = init_x[0] + sinf(temp_angle) * (distance * size);
    }

    if (init_y[init_count - 1] < init_y[0])
	{
      point.y = init_y[0] - cosf(temp_angle) * (distance * size);
    } 
	else 
	{
      point.y = init_y[0] + cosf(temp_angle) * (distance * size);
    }

    float temp_dis = Calc_Distance(point.x, point.y, temp_pos.x, temp_pos.y);
    if (temp_dis != 0.f)
	{
      float temp_value = (point.x - temp_pos.x) / temp_dis;
      if ((point.y - temp_pos.y) >= 0.f) point.angle = acosf(temp_value);
      else point.angle = -acosf(temp_value);
    } 
	else 
	{
      point.angle = 1.f;
    }
	point.angle = -CC_RADIANS_TO_DEGREES(point.angle);

    temp_pos.x = point.x;
    temp_pos.y = point.y;
    trace_vector.push_back(point);
    temp_distance = Calc_Distance(point.x, point.y, init_x[0], init_y[0]);
  }

  FPointAngle& temp_point = trace_vector.back();
  temp_point.x = init_x[init_count - 1];
  temp_point.y = init_y[init_count - 1];
}

void MathAide::BuildBezier(float init_x[], float init_y[], int init_count, std::vector<FPoint>& trace_vector, float distance)
{
  trace_vector.clear();

  int index = 0;
  FPoint temp_pos = { 0.f, 0.f };
  float t = 0.f;
  int count = init_count - 1;
  float temp_distance = distance;
  float temp_value = 0.f;

  while (t <= 1.0f) 
  {
    temp_pos.x = 0.f;
    temp_pos.y = 0.f;
    index = 0;
    while (index <= count) 
	{
      temp_value = pow(t, index) * pow(1.f - t, count - index) * Combination(count, index);
      temp_pos.x += init_x[index] * temp_value;
      temp_pos.y += init_y[index] * temp_value;
      ++index;
    }
    
    float pos_space = 0.f;
    if (trace_vector.size() > 0)
	{
      FPoint& back_pos = trace_vector.back();
      pos_space = Calc_Distance(back_pos.x, back_pos.y, temp_pos.x, temp_pos.y);
    }

    if (pos_space >= temp_distance || trace_vector.size() == 0)
	{
      trace_vector.push_back(temp_pos);
    }

    t += 0.00025f;
  }
}

void MathAide::BuildBezierFast(float init_x[], float init_y[], int init_count, std::vector<FPoint>& trace_vector, float distance)
{
  trace_vector.clear();

  int index = 0;
  FPoint temp_pos = { 0.f, 0.f };
  float t = 0.f;
  int count = init_count - 1;
  float temp_distance = distance;
  float temp_value = 0.f;

  while (t <= 1.0f)
  {
    temp_pos.x = 0.f;
    temp_pos.y = 0.f;
    index = 0;
    while (index <= count)
	{
      temp_value = pow(t, index) * pow(1.f - t, count - index) * Combination(count, index);
      temp_pos.x += init_x[index] * temp_value;
      temp_pos.y += init_y[index] * temp_value;
      ++index;
    }

    float pos_space = 0.f;
    if (trace_vector.size() > 0)
	{
      FPoint& back_pos = trace_vector.back();
      pos_space = Calc_Distance(back_pos.x, back_pos.y, temp_pos.x, temp_pos.y);
    }

    if (pos_space >= temp_distance || trace_vector.size() == 0) 
	{
      trace_vector.push_back(temp_pos);
    }

    t += 0.02f;
  }
}

void MathAide::BuildBezier(float init_x[], float init_y[], int init_count, std::vector<FPointAngle>& trace_vector, float distance)
{
  trace_vector.clear();
  FPointAngle pos1 = { init_x[0], init_y[0], 1.f };
  trace_vector.push_back(pos1);

  int index = 0;
  FPointAngle temp_pos0 = { 0.f, 0.f };
  float t = 0.f;
  int count = init_count - 1;
  float temp_distance = distance;
  FPointAngle temp_pos = { 0.f, 0.f };
  float temp_value = 0.f;

  while (t < 1.0f)
  {
    temp_pos.x = 0.f;
    temp_pos.y = 0.f;
    index = 0;
    while (index <= count)
	{
      temp_value = pow(t, index) * pow(1.f - t, count - index) * Combination(count, index);
      temp_pos.x += init_x[index] * temp_value;
      temp_pos.y += init_y[index] * temp_value;
      ++index;
    }

    float pos_space = 0.f;
    if (trace_vector.size() > 0)
	{
      FPointAngle& back_pos = trace_vector.back();
      pos_space = Calc_Distance(back_pos.x, back_pos.y, temp_pos.x, temp_pos.y);
    }

    if (pos_space >= temp_distance || trace_vector.size() == 0)
	{
      if (trace_vector.size() > 0) 
	  {
        float temp_dis = Calc_Distance(temp_pos.x, temp_pos.y, temp_pos0.x, temp_pos0.y);
        if (temp_dis != 0.f)
		{
          float temp_value = (temp_pos.x - temp_pos0.x) / temp_dis;
          if ((temp_pos.y - temp_pos0.y) >= 0.f) temp_pos.angle = acosf(temp_value);
          else temp_pos.angle = -acosf(temp_value);
        } 
		else
		{
          temp_pos.angle = 1.f;
        }
      } 
	  else 
	  {
        temp_pos.angle = 1.f;
      }
	  
	  temp_pos.angle = -CC_RADIANS_TO_DEGREES(temp_pos.angle);
      trace_vector.push_back(temp_pos);
      temp_pos0.x = temp_pos.x;
      temp_pos0.y = temp_pos.y;
    }

    t += 0.00025f;
  }
}

void MathAide::BuildCircle(float center_x, float center_y, float radius, FPoint* fish_pos, int fish_count) 
{
  if (fish_count <= 0) 
	  return;
  float cell_radian = 2 * M_PI / fish_count;

  // x = xo + r * cos(α)
  // y = yo + r * sin(α)
  float cell_radian_temp = 0;
  for (int i = 0; i < fish_count; ++i) 
  {
	  cell_radian_temp = -(i * cell_radian);  // 手机角度转换
    fish_pos[i].x = center_x + radius * cosf(cell_radian_temp);
    fish_pos[i].y = center_y + radius * sinf(cell_radian_temp);
  }
}

void MathAide::BuildCircle(float center_x, float center_y, float radius, FPointAngle* fish_pos, int fish_count, float rotate, float rotate_speed) 
{

  if (fish_count <= 0) 
	  return;
  float cell_radian = 2 * M_PI / fish_count;

  // x = xo + r * cos(α)
  // y = yo + r * sin(α)
  FPoint last_pos;
  float cell_radian_temp = 0;
  // 这里计算好像有问题
  for (int i = 0; i < fish_count; ++i) 
  {
	  cell_radian_temp = -i * cell_radian - rotate;
	  fish_pos[i].x = center_x + radius * cosf(cell_radian_temp);
	  fish_pos[i].y = center_y + radius * sinf(cell_radian_temp);

	  cell_radian_temp += rotate_speed;
    last_pos.x = center_x + radius * cosf(cell_radian_temp);
    last_pos.y = center_y + radius * sinf(cell_radian_temp);

   
    float temp_dis = Calc_Distance(fish_pos[i].x, fish_pos[i].y, last_pos.x, last_pos.y);
    if (temp_dis != 0.f)
	{
      float temp_value = (fish_pos[i].x - last_pos.x) / temp_dis;
      if ((fish_pos[i].y - last_pos.y) >= 0.f)
	  {
        fish_pos[i].angle = acosf(temp_value);
      } 
	  else
	  {
        fish_pos[i].angle = -acosf(temp_value);
      }
    } 
	else
	{
      fish_pos[i].angle = M_PI+M_PI_2;
    }
	fish_pos[i].angle = -CC_RADIANS_TO_DEGREES(fish_pos[i].angle);
  }
}
