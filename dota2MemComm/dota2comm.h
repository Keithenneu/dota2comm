#include <stdint.h>


#define CANT_FIND_PROCESS 1
#define REGION_READ_FAILED 3
#define INVALID_REGION_STATE 4
#define UNKNOWN_BASE_ADDRESS 5
#define CANT_READ_MEMORY 5

#define DLLEXPORT extern "C" __declspec(dllexport)


struct Cell
{
	int32_t data;
	int32_t tt;
};


//use guaranteed 32bit datatypes
struct MTable
{
    int32_t garbage1, garbage2;
    int32_t array;
    int32_t garbage3, garbage4, garbage5, garbage6;
    int32_t arraysize;
};


struct Table
{
	int32_t input_ptr, output_ptr;
};

DLLEXPORT const char *receiveMessage();
DLLEXPORT bool sendMessage(const char *text);
DLLEXPORT int init();
DLLEXPORT int getNrConnectedClients();
