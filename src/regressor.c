#include "parser.h"
#include "network.h"
#include "image.h"
#include "utils.h"
#include <sys/time.h>
#include <assert.h>
#ifdef OPENCV
#include "opencv2/highgui/highgui_c.h"
#endif

//image get_image_from_stream(CvCapture *cap);
int debuga=0;

#define ENUMSTR(var) #var
void train_regressor(char *datacfg, char *cfgfile, char *weightfile, int *gpus, int ngpus, int clear, int threads){}

void predict_regressor(char *cfgfile, char *weightfile, char *filename)
{
    network net = parse_network_cfg(cfgfile);
    if(weightfile){
        load_weights(&net, weightfile);
    }
    set_batch_network(&net, 1);
    srand(2222222);

    clock_t time;
    char buff[256];
    char *input = buff;
    while(1){
        if(filename){
            strncpy(input, filename, 256);
        }else{
            printf("Enter Image Path: ");
            fflush(stdout);
            input = fgets(input, 256, stdin);
            if(!input) return;
            strtok(input, "\n");
        }
        image im = load_image_color(input, 0, 0);
        image sized = letterbox_image(im, net.w, net.h);

        float *X = sized.data;
        time=clock();
        float *predictions = network_predict(net, X);
        printf("Predicted: %f R=%.3f X=%.3f Y=%.3f\n", predictions[0],predictions[1],predictions[2],predictions[3]);
        printf("%s: Predicted in %f seconds.\n", input, sec(clock()-time));
        free_image(im);
        free_image(sized);
        if (filename) break;
    }
}


void demo_regressor(char *datacfg, char *cfgfile, char *weightfile, int cam_index, const char *filename)
{
#ifdef OPENCV
    printf("Regressor Demo\n");
    network net = parse_network_cfg(cfgfile);
    if(weightfile){
        load_weights(&net, weightfile);
    }
    set_batch_network(&net, 1);

    srand(2222222);
    CvCapture * cap;

    FILE *fp_txt=NULL;
    if(filename){
        if(!strstr(filename,".txt")){   //add
            cap = cvCaptureFromFile(filename);
            if(!cap) error("Couldn't read image file.\n");
        }else{
            fp_txt = fopen(filename,"r");
            if(!fp_txt) error("Couldn't open image list.\n");
        }
    }else{
        cap = cvCaptureFromCAM(cam_index);
        if(!cap) error("Couldn't connect to webcam.\n");
    }

    cvNamedWindow("Regressor", CV_WINDOW_NORMAL); 
    cvResizeWindow("Regressor", 512, 512);
    float fps = 0;

    while(1){
        struct timeval tval_before, tval_after, tval_result;
        gettimeofday(&tval_before, NULL);

        if(filename){   //add
            char filejpg[1024];
            char *q = fgets(filejpg,1024,fp_txt);
            if(!q) break;
            char *p = strstr(filejpg,"\n");
            *p = '\0';
            cap = cvCaptureFromFile(filejpg);
            if(!cap) error("Couldn't read image file.\n");
        }
        image in = get_image_from_stream(cap);
        image in_s = letterbox_image(in, net.w, net.h);

        float *predictions = network_predict(net, in_s.data);
        box box;                              //add
        box.x = predictions[2];               //add
        box.y = predictions[3];               //add
        box.w = predictions[1];               //add
        box.h = predictions[1];               //add
        if(predictions[0]>-0.40)                //add
            draw_bbox(in, box, 2, 0, 255,255);    //add
        show_image(in, "Regressor");          //mod flow

        printf("\033[2J");
        printf("\033[1;1H");
        printf("\nFPS:%.0f\n",fps);

        printf("People: %f   %.3f (%.3f %.3f)\n", predictions[0],predictions[1],predictions[2],predictions[3]);

        free_image(in_s);
        free_image(in);

        if(filename){
            while(1){   //add
                int k=cvWaitKey(1000);
                if(k==-1) continue;
                if(k==27 || k==1048603) exit(1);
                if(k==32 || k==1048608) break;
                printf("%d\n",k);
            }
        }else{
            int k=cvWaitKey(30);
            if(k==27 || k==1048603) break;
        }

        gettimeofday(&tval_after, NULL);
        //timersub(&tval_after, &tval_before, &tval_result);
        //float curr = 1000000.f/((long int)tval_result.tv_usec);
        //fps = .9*fps + .1*curr;
    }
    if(fp_txt) fclose(fp_txt);  //add
#endif
}


void run_regressor(int argc, char **argv)
{
    if(argc < 4){
        fprintf(stderr, "usage: %s %s [train/test/valid] [cfg] [weights (optional)]\n", argv[0], argv[1]);
        return;
    }

    char *gpu_list = find_char_arg(argc, argv, "-gpus", 0);
    int *gpus = 0;
    int gpu = 0;
    int ngpus = 0;
    if(gpu_list){
        printf("%s\n", gpu_list);
        int len = strlen(gpu_list);
        ngpus = 1;
        int i;
        for(i = 0; i < len; ++i){
            if (gpu_list[i] == ',') ++ngpus;
        }
        gpus = calloc(ngpus, sizeof(int));
        for(i = 0; i < ngpus; ++i){
            gpus[i] = atoi(gpu_list);
            gpu_list = strchr(gpu_list, ',')+1;
        }
    } else {
        gpu = gpu_index;
        gpus = &gpu;
        ngpus = 1;
    }

    int cam_index = find_int_arg(argc, argv, "-c", 0);
    int threads = find_int_arg(argc, argv, "-t", 32);       //add threads option for opencv
    debuga  = find_int_arg(argc, argv, "-debuga", 0);   //add for augmentation
    int clear = find_arg(argc, argv, "-clear");
    char *data = argv[3];
    char *cfg = argv[4];
    char *weights = (argc > 5) ? argv[5] : 0;
    char *filename = (argc > 6) ? argv[6]: 0;
    fprintf(stderr,"load threads = %d\n",threads);
    if(0==strcmp(argv[2], "test"))      { predict_regressor(data, cfg, weights); }
    else if(0==strcmp(argv[2], "train")){ train_regressor(data, cfg, weights, gpus, ngpus, clear, threads); }
    else if(0==strcmp(argv[2], "demo")) { demo_regressor(data, cfg, weights, cam_index, filename); }
}


