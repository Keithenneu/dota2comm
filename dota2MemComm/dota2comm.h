#define CANT_FIND_PROCESS 1
#define GATHER_PROCESS_INFO_FAILED 2
#define REGION_READ_FAILED 3
#define INVALID_REGION_STATE 4
#define UNKNOWN_BASE_ADDRESS 5
#define CANT_READ_MEMORY 5

#define DLLEXPORT extern "C" __declspec(dllexport)

struct Cell
{
	int data;
	int tt;
};

/*
struct MTable
{
void *garbage1, *garbage2;
Cell *array;
void *garbage3, *garbage4, *garbage5, *garbage6;
int arraysize;
};
*/

//int is always 32-bit 
//pointers are 64-bit in MSVC x86
struct MTable
{
    int garbage1, garbage2;
    int array;
    int garbage3, garbage4, garbage5, garbage6;
    int arraysize;
};


struct Table
{
	int input_ptr, output_ptr;
};

DLLEXPORT const char *receiveMessage();
DLLEXPORT bool sendMessage(const char *text);
DLLEXPORT int init();
DLLEXPORT int getNrConnectedClients();
