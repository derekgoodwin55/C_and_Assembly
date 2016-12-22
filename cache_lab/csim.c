/* Derek Goodwin goodw171 3862569
 * csim.c - A cache simulator that can replay traces from Valgrind
 *     and output statistics such as number of hits, misses, and
 *     evictions.  The replacement policy is MRU.
 *
 *
 * The function printSummary() is given to print output.
 * Please use this function to print the number of hits, misses and evictions.
 * This is crucial for the driver to evaluate your work. 
 */
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include "cachelab.h"

//#define DEBUG_ON 
#define ADDRESS_LENGTH 64

/* Type: Memory address */
typedef unsigned long long int mem_addr_t;

/* Type: Cache line
   MRU is a counter used to implement MRU replacement policy  */
typedef struct cache_line {
    char valid; //4 byte due to alignment
    mem_addr_t tag;  //8 bytes
    unsigned long long int mru;  //8 bytes
} cache_line_t;  //20 bytes

typedef cache_line_t* cache_set_t;
typedef cache_set_t* cache_t_temp;
typedef cache_t_temp* cache_3D_t; 

/* Globals set by command line args */
int verbosity = 0;
int s = 0; /* set index bits */
int b = 0; /* block offset bits */
int E = 0; /* associativity */
char* trace_file = NULL;

/* Derived from command line args */
int S; /* number of sets */
int B; /* block size (bytes) */

/* Counters used to record cache statistics */
int miss_count = 0;
int hit_count = 0;
int eviction_count = 0;
unsigned long long int mru_counter = 1;

/* The cache we are simulating */
cache_line_t ***cache;  


mem_addr_t set_index_mask;

int numTagBits;
/* 
 * initCache - Allocate memory, write 0's for valid and tag and MRU
 * also computes the set_index_mask
 */
void initCache()
{
	//Step 1 create 3D array cache
	//Important link: HOW TO CREATE 3D Array http://www.slideshare.net/chjungsik/malloc-for-multidimensional
	
	//array will look like cache[S][E][B]
	
	cache = (cache_line_t***)malloc(sizeof(cache_line_t**) * S);

	int i;
	int j;
	for(i = 0; i < S; i++)
	{
		cache[i] = (cache_line_t**)malloc(sizeof(cache_line_t*) * E);
		
		for(j = 0; j < E; j++)
		{
			cache[i][j] = (cache_line_t*)malloc(sizeof(cache_line_t) * B);
		}
		
	}
	//printf("init Cache1\n");
	
	//Step 2 Loop through 3D array and set each cache_line structs fields to zero.
	int index1;
	int index2;
	int index3;
	for(index1 = 0; index1 < S; index1++)
	{
		for(index2 = 0; index2 < E; index2++)
		{
			for(index3 = 0; index3 < B; index3++)
			{
				cache[index1][index2][index3].valid = '0';
				cache[index1][index2][index3].tag = 0;
				cache[index1][index2][index3].mru = 0;
			}
		}
	}
//printf("init Cache2\n");	
	
	//Step 3 create set_index_mask using bitwise operators
	set_index_mask = (1 << s) - 1;
	set_index_mask = (set_index_mask << b);
	//printf("set_index_mask= %llu\n",set_index_mask);
}


/* 
 * freeCache - free allocated memory
 */
void freeCache()
{
    int i;
    for (i=0; i<S; i++){
        free(cache[i]);
    }
    free(cache);
}

/* 
 * accessData - Access data at memory address addr.
 *   If it is already in cache, increast hit_count
 *   If it is not in cache, bring it in cache, increase miss count.
 *   Also increase eviction_count if a line is evicted.
 */
void accessData(mem_addr_t addr)
{

//mem_addr_t set_block_mask = ((1 << b) - 1);

	//Do stuff to addr passed in to extract tag bits
	unsigned long long int tagBitsPassedIn = (addr >> (s+b));
	
	unsigned long long int setBitsPassedIn = ((addr & set_index_mask) >> b);
	
	//printf("tag = %llu set = %llu\n", tagBitsPassedIn,setBitsPassedIn );
	
	//Do stuff to addr passed in to extract block bits
   	//unsigned long long int blockBitsPassedIn = addr & set_block_mask;
	

	//Based on the address passed in, we need to determine if hit, miss, or evict
	
	//There are three Cases: Case 1 = Hit, Case 2 = Evict, Case 3 = Miss
	int hadAHit = 0;
	
	//THIS IS TEMPLATE FOR CASE 1: HIT
	//iterate through all lines in the matching set and see if tags match.  If any of tags match, we have a hit and can break out of loop
	int line;
	for(line = 0; line < E; line++){
		//compare address passed in against things already in our cache
		if(cache[setBitsPassedIn][line][0].tag == tagBitsPassedIn && cache[setBitsPassedIn][line][0].valid == '1'){

			//Need to increment hit_counter and update mru_counter for all elements with matching set and line
			mru_counter++;
			hit_count++;
			hadAHit = 1;
			// increment mru counters for each element
			int block;
			for(block = 0; block < B; block++){
				cache[setBitsPassedIn][line][block].mru = mru_counter;
				cache[setBitsPassedIn][line][block].valid = '1';
				cache[setBitsPassedIn][line][block].tag = tagBitsPassedIn;
			}
			
			break;
		}
	}
	//if we did not find a hit in that loop above, need to loop through to see if cache is full
	//if there is one upon spot in cache (one of lines is empty), it is a miss, but we need to now put the information of addr passed in into cache
	if(hadAHit == 0){
		int hadAMiss = 0;
		int line2;
		for(line2 = 0; line2 < E; line2++){
			if(cache[setBitsPassedIn][line2][0].valid == '0'){
			mru_counter++;
			miss_count++;
			// increment mru counters for each element
			int block2;
			for(block2 = 0; block2 < B; block2++){
				cache[setBitsPassedIn][line2][block2].mru = mru_counter;
				cache[setBitsPassedIn][line2][block2].valid = '1';
				cache[setBitsPassedIn][line2][block2].tag = tagBitsPassedIn;
				}
				hadAMiss = 1;
				break;
			}
		}
		
		if (hadAMiss == 0){
		int line3;
		int maxMRU = cache[setBitsPassedIn][0][0].mru;
		int maxIndex = 0;
		for(line3 = 1; line3 < E; line3++){
			if(cache[setBitsPassedIn][line3][0].mru > maxMRU){
				maxMRU = cache[setBitsPassedIn][line3][0].mru;
				maxIndex = line3;
			}		
		}
		
			mru_counter++;
			miss_count++;
			eviction_count++;
			// increment mru counters for each element
			int block4;
			for(block4 = 0; block4 < B; block4++){
				cache[setBitsPassedIn][maxIndex][block4].mru = mru_counter;
				cache[setBitsPassedIn][maxIndex][block4].valid = '1';
				cache[setBitsPassedIn][maxIndex][block4].tag = tagBitsPassedIn;
				}		
		}
	}
	
	else{
		return;
	}
}


/*
 * replayTrace - replays the given trace file against the cache 
 */
void replayTrace(char* trace_fn){
    char buf[1000];
    mem_addr_t addr = 0;
    unsigned int len = 0;
    FILE* trace_fp = fopen(trace_fn, "r");

    if(!trace_fp){
        fprintf(stderr, "%s: %s\n", trace_fn, strerror(errno));
        exit(1);
    }

    while( fgets(buf, 1000, trace_fp) != NULL) {
        /* buf[Y] gives the Yth byte in the trace line */
		addr = 0;
		len = 0;
		
        /* Read address and length from the trace using sscanf */
         sscanf(buf+3, "%llx,%u", &addr, &len);
         if(buf[0] == 'I'){
			 continue;
		 }
         else{
			 if(buf[1] == 'M'){
				 hit_count++;
			 }
			 //printf("addr = %llx\n", addr);
			 accessData(addr);
         
		 //call the key function in the loop
		}   
	}
	fclose(trace_fp);
}

/*
 * printUsage - Print usage info
 */
void printUsage(char* argv[]){
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

/*
 * main - Main routine 
 */
int main(int argc, char* argv[]){
    char c;

    while( (c=getopt(argc,argv,"s:E:b:t:vh")) != -1){
        switch(c){
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            trace_file = optarg;
            break;
        case 'v':
            verbosity = 1;
            break;
        case 'h':
            printUsage(argv);
            exit(0);
        default:
            printUsage(argv);
            exit(1);
        }
    }

    /* Make sure that all required command line args were specified */
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        printUsage(argv);
        exit(1);
    }

    /* Compute S, E and B from command line args */
    
    //printf("main1\n");
    //S =  1 << (s); 
    //B =  1 << (b);
     S = pow(2,s);
     B = pow(2,b);
     //printf("main2\n");
 
    /* Initialize cache */
    initCache();

#ifdef DEBUG_ON
    printf("DEBUG: S:%u E:%u B:%u trace:%s\n", S, E, B, trace_file);
    printf("DEBUG: set_index_mask: %llu\n", set_index_mask);
#endif
    
	/* Read the trace and access the cache */
    replayTrace(trace_file);

    /* Free allocated memory */
    freeCache();

    /* Output the hit and miss statistics for the autograder */
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}
