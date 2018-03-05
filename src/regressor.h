#include "image.h"  //add
#include <sys/time.h>
#include <assert.h>

void train_regressor(char *datacfg, char *cfgfile, char *weightfile, int *gpus, int ngpus, int clear, int threads);

void predict_regressor(char *cfgfile, char *weightfile, char *filename);

void demo_regressor(char *datacfg, char *cfgfile, char *weightfile, int cam_index, const char *filename);

void run_regressor(int argc, char **argv);


