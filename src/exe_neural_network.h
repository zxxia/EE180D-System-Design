#ifndef __EXE_NEURAL_NETWORK_H__
#define __EXE_NEURAL_NETWORK_H__
#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include "floatfann.h"
#include "feature_detector.h"

#define TURN 0
#define WALK 1
#define STAIR 2
#define RUN 3
#define JUMP 4


#define LEFT_TURN 0
#define RIGHT_TURN 1

#define UP_STAIR 0
#define DOWN_STAIR 1

#define SLOW_WALK 0
#define MED_WALK 1
#define FAST_WALK 2

struct fann *global_ann;
struct fann *turn_ann;
struct fann *walk_ann;
struct fann *stair_ann;
struct fann *run_ann;

void init_networks();
void destroy_networks();
int exe_global_neural_network(const GlobalFeature *global_feature);
int exe_walk_neural_network(const WalkFeature *walk_feature);
int exe_stair_neural_network(const StairFeature *stair_feature);
int exe_turn_neural_network(const TurnFeature *turn_feature);
//int exe_jump_neural_network(char *jump_feature_file);
//int exe_run_neural_network(char *run_feature_file);


#endif