#define DEBUGGING 0
#define WIDTH 128
#define HEIGHT 64
#define THRESHOLD 200
#define SMOOTHNESS 8
#define IMGDATALEN 2+4*(HEIGHT+WIDTH)/SMOOTHNESS
double* imgextactdata(const char *path);