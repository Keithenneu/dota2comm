#include <windows.h>
#include <iostream>
#include <fstream>
#include <TlHelp32.h>
#include <vector>

#include "dota2comm.h"

using namespace std;

vector<Table> tables;
HANDLE phandle;

DWORD FindProcessId(const char *processname)
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    DWORD result = 0;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hProcessSnap) return(FALSE);

    pe32.dwSize = sizeof(PROCESSENTRY32); // <----- IMPORTANT

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return 0;
    }

    do
    {
        //printf("Checking process %s\n", pe32.szExeFile);
        if (0 == strcmp(processname, pe32.szExeFile))
        {
            result = pe32.th32ProcessID;
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    return result;
}

// check if the meory page exists
int checkPage(DWORD64 addr, HANDLE phandle, int *size)
{
	MEMORY_BASIC_INFORMATION mem;
	SIZE_T ret_size = VirtualQueryEx(phandle, (LPCVOID)addr, &mem, sizeof(mem));
	
	if (ret_size == 0)
	{
		return REGION_READ_FAILED;
	}
	
	if (mem.State != MEM_COMMIT)
	{
		return INVALID_REGION_STATE;
	}
	
	if ((DWORD64)mem.BaseAddress != addr)
	{
		return UNKNOWN_BASE_ADDRESS;
	}
	
	*size = mem.RegionSize;	
	
	return 0;
}

// search for a number of bytes in a given buffer
int search(char* buffer, int size, const char* needle, int needle_size, int start=0)
{
	bool found;
	for (int i = start; i<size; i++)
	{
		found = true;
		for (int j = 0; j < needle_size; j++)
		{
			if (buffer[i + j] != needle[j])
			{
				found = false;
				break;	
			}
		}
		if (found)
			return i;
	}
	return -1;
}

// find all occurances of a byte sequence in a buffer
vector<int> findall(char* buffer, int size, const char* needle, int needle_size)
{
	vector<int> result;
	int addr = 0;
	while (addr != -1)
	{
		addr = search(buffer, size, needle, needle_size, addr);
		if (addr != -1)
		{
			result.push_back(addr);
			addr++;
		}
	}
	return result;
}

// winapi wrapper
int readMemory(HANDLE phandle, char* buffer, int size, int base)
{
	int err = ReadProcessMemory(phandle, (LPCVOID)base, buffer, size, 0);
	if (err == 0)
	{
		return CANT_READ_MEMORY;
	}
	
	return 0;
}	

// find tables created and marked by the lua module
vector<Table> findTables(char* buffer, int size, int base)
{
	// find the marker
	vector<int> commtables = findall(buffer, size, "commtablecommtable", strlen("commtablecommtable"));
	
	vector<int> references;
	
	// find the according lua structures
	for(int i : commtables)
	{
		int iptr = i + base - 16;
		const char* ptr = (char*)&iptr;
		vector<int> temp = findall(buffer, size, ptr, 4);
		for (int t : temp)
			if (t >= base && t < base + size)
				references.push_back(t);
	}
	
	
	vector<Table> tables;
	
	// check if it was the key for a table
	for(int r : references)
	{
		int tbl_ptr = *((int*)&buffer[r-8]);
		if (tbl_ptr < base || tbl_ptr > size + base)
			continue;
		MTable *btable = (MTable*)&buffer[tbl_ptr-base];
		
		if ((int)btable->array < base || (int)btable->array > size + base)
			continue;
			
		// The array part of the marked table
		Cell *array = (Cell*)&buffer[(int)btable->array-base];
		
		int sptr = array[0].data;
		// First entry could be valid (is in memory range)
		if (sptr > base && sptr < size + base)
		{
			// string length, according to lua
			int length = *((int*) &buffer[sptr-base+12]);
			if (length > 0 && length < 100)
			{
				// check if it's the expected string
				if (!strncmp(&buffer[sptr-base+16], "commtable", length))
				{
					Table table;
					int input_tbl = array[1].data;
					int output_tbl = array[2].data;
					MTable *btable = (MTable*)&buffer[input_tbl-base];
					table.input_ptr = (int)btable->array;
					btable = (MTable*)&buffer[output_tbl-base];
					table.output_ptr = (int)btable->array;
					tables.push_back(table);
				}
			}
		}
	}
	
	return tables;
}

// read a string from memory
const char * getString(int array_ptr, int index, HANDLE phandle, int *length)
{
	int sptr;
	ReadProcessMemory(phandle, (LPCVOID)(array_ptr+index*8), &sptr, 4, 0);
	int s_length = 0;
	if (length == 0)
	{
		length = &s_length;
	}
	ReadProcessMemory(phandle, (LPCVOID)(sptr+12), length, 4, 0);
	char* sbuf = new char[*length+1];
	sbuf[*length] = 0;
	ReadProcessMemory(phandle, (LPCVOID)(sptr+16), sbuf, *length, 0);
	return sbuf;
}

// write a string to memory
void writeString(int array_ptr, int index, HANDLE phandle, const char *buffer, int length)
{
	int sptr;
	ReadProcessMemory(phandle, (LPCVOID)(array_ptr+index*8), &sptr, 4, 0);
	int max_length = 0;
	ReadProcessMemory(phandle, (LPVOID)(sptr+12), &max_length, 4, 0);
	if (max_length < length)
		length = max_length;
	WriteProcessMemory(phandle, (LPVOID)(sptr+16), buffer, length, 0);
}

DLLEXPORT int init()
{
    DWORD64 base = 0x10000;

	// find the process
	DWORD pid = FindProcessId("dota2.exe");
	if (pid == 0)
	{
		phandle = 0;
		return CANT_FIND_PROCESS;
	}
	
		
	// open it with some right
	phandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, 0, pid);
	int size;
	
	// check if the expected memory region is valid
	int state = checkPage(base, phandle, &size);
	if (state == 0)
	{
		char *buffer = new char[size];
		// make a memory snaphsot, since we have to do some searching
		int err = readMemory(phandle, buffer, size, base);
		if (err != 0)
		{
			delete buffer;
			return err;
		}
		// use it to find our tables
		tables = findTables(buffer, size, base);
		delete buffer;
	}
	else
	{
		phandle = 0;
		return state;
	}
	return 0;
}

DLLEXPORT int getNrConnectedClients()
{
	return tables.size();
}

DLLEXPORT bool sendMessage(const char *text)
{
	int length = strlen(text);
	bool success = true;
	
	// write to each table
	for (Table table : tables)
	{
		// read ring buffer boundaries
	    int b_start = atoi(getString(table.input_ptr, 0, phandle, 0)+2);
		int b_end = atoi(getString(table.input_ptr, 1, phandle, 0)+2);
		if ((b_end+1)%10 == b_start)
		{
			success = false; // buffer is full
			continue;
		}
		// write and adjust the buffer
		char *mbuf = new char[1+3+length+1];
		snprintf(mbuf, 1+3+length+1, "i%03d%s", length, text);
		writeString(table.input_ptr, b_end+2, phandle, mbuf, 1+3+length+1);
		b_end = (b_end+1)%10;
		char *sbuf = new char[2+3+1];
		snprintf(sbuf, 2+3+1, "ie%03d", b_end);
		writeString(table.input_ptr, 1, phandle, sbuf,2+3+1);
	}
	return success;
}

DLLEXPORT const char *receiveMessage()
{
	// read from all tables. first success gets returned
	for (Table table : tables)
	{
		// read ring buffer boundaries
		int b_start = atoi(getString(table.output_ptr, 0, phandle, 0)+2);
		int b_end = atoi(getString(table.output_ptr, 1, phandle, 0)+2);
		if (b_start == b_end) 
			continue; // buffer is empty
		const char *msg = getString(table.output_ptr, b_start+2, phandle, 0);
		b_start = (b_start+1)%10;
		char *sbuf = new char[2+3+1];
		snprintf(sbuf, 2+3+1, "os%03d", b_start);
		writeString(table.output_ptr, 0, phandle, sbuf, strlen(sbuf));
		return msg;	
	}
	return 0;
}

