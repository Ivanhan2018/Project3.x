
#ifndef SCENE_FISH_TRACE_H_
#define SCENE_FISH_TRACE_H_
#pragma once

#include <vector>
#include "CMD_Fish.h"

// ArraySizeHelper是一个返回类型为char[N]的函数,其形参类型为 T[N].
// 函数没必要实现, 因为sizeof只需要类型.
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

// arraysize(arr)返回array数组的元素个数. 该表达式是编译时常量,
// 可以用于定义新的数组. 如果传递一个指针会报编译时错误.
#define arraysize(array) (sizeof(ArraySizeHelper(array)))

typedef std::vector<NS_lrbyFish::FPointAngle> TraceVector;

extern TraceVector scene_kind_1_trace_[100 + 17 + 17 + 30 + 30 + 15 + 1];
extern TraceVector scene_kind_2_trace_[200 + 7 * 2];
extern TraceVector::size_type scene_kind_2_small_fish_stop_index_[200];
extern TraceVector::size_type scene_kind_2_small_fish_stop_count_;
extern TraceVector::size_type scene_kind_2_big_fish_stop_index_;
extern TraceVector::size_type scene_kind_2_big_fish_stop_count_;
extern TraceVector scene_kind_3_trace_[(50 + 40 + 30 + 1) * 2];
extern TraceVector scene_kind_4_trace_[8 * 8];
extern TraceVector scene_kind_5_trace_[(40 + 40 + 24 + 13 + 1) * 2];

// 场景7.jpg
void BuildSceneKind1Trace(float screen_width, float screen_height, bool bViewChange);
// 场景5.jpg
void BuildSceneKind2Trace(float screen_width, float screen_height, bool bViewChange);
// 场景1.jpg
void BuildSceneKind3Trace(float screen_width, float screen_height, bool bViewChange);
// 场景6.jpg
void BuildSceneKind4Trace(float screen_width, float screen_height, bool bViewChange);
// 场景3.jpg
void BuildSceneKind5Trace(float screen_width, float screen_height, bool bViewChange);

float angle_range(float angle);

#endif  // SCENE_FISH_TRACE_H_