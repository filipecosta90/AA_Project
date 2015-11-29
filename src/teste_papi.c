#include "papi.h"
#include <stdio.h>
#include <stdlib.h>

main()
{
int retval, EventSet = PAPI_NULL;
long_long values[7];
    unsigned counter;
    unsigned c;
    unsigned long fact;
    unsigned stoppoint;

int events[1];
	long long counts[1];
counts[0] = 0;

        /* Initialize the PAPI library */
      retval = PAPI_query_event(PAPI_LD_INS);
	if (retval != PAPI_OK) {
	printf("porra!!! %d\n", PAPI_LD_INS);	
}

	events[0]=PAPI_LD_INS;

	PAPI_start_counters(events,1);
int x = 0;
for ( x = 0; x <10000; x++ ){
x = x+1;
}

	PAPI_stop_counters(counts,1);
  retval = PAPI_library_init(PAPI_VER_CURRENT);



printf("counts %lld\t\n", counts[0]);
        if (retval != PAPI_VER_CURRENT) {
          fprintf(stderr, "PAPI library init error!\n");
          exit(1);
        }

        /* Create the Event Set */
        if (PAPI_create_eventset(&EventSet) != PAPI_OK)
            printf ("%s:%d\t ERROR 1\n", __FILE__, __LINE__);


        /* Add Total Instructions Executed to our EventSet */
        retval = PAPI_add_event(EventSet, 0x80000035); 
	if( retval != PAPI_OK)
            printf ("%s:%d\t %d ERROR 2\n", __FILE__, __LINE__, retval);
if( retval == PAPI_ENOEVNT )
            printf ("%s:%d\t %d PAPI_ENOEVNT \n", __FILE__, __LINE__, retval);
if( retval == PAPI_ENOEVST )
            printf ("%s:%d\t %d PAPI_ENOEVST \n", __FILE__, __LINE__, retval);
if( retval == PAPI_ESYS )
            printf ("%s:%d\t %d PAPI_ESYS \n", __FILE__, __LINE__, retval);

if( retval == PAPI_EINVAL )
            printf ("%s:%d\t %d PAPI_EINVAL \n", __FILE__, __LINE__, retval);

if( retval == PAPI_EISRUN )
            printf ("%s:%d\t %d PAPI_EINVAL \n", __FILE__, __LINE__, retval);

if( retval == PAPI_ECNFLCT )
            printf ("%s:%d\t %d PAPI_EINVAL \n", __FILE__, __LINE__, retval);







   srand ( time(NULL) );
   stoppoint = 50+(rand() % 100);
/* Do some computation here */
    for (counter = 0; counter < stoppoint; counter++)
    {
        /* Initialize the PAPI library */
        retval = PAPI_library_init(PAPI_VER_CURRENT);

        if (retval != PAPI_VER_CURRENT) {
          fprintf(stderr, "PAPI library init error!\n");
          exit(1);
        }


        /* Start counting */
        if (PAPI_start(EventSet) != PAPI_OK)
            printf ("%s:%d\t ERROR 9\n", __FILE__, __LINE__);


                fact = 1;
        for (c = 1; c <= counter; c++)
        {
                     fact = c * c;
        }




        if (PAPI_read(EventSet, values) != PAPI_OK)
            printf ("%s:%d\t ERROR 10\n", __FILE__, __LINE__);
        printf ("\t%lld \n", values[0]);
        /* Do some computation here */

        if (PAPI_stop(EventSet, values) != PAPI_OK)
            printf ("%s:%d\t ERROR 11\n", __FILE__, __LINE__);

            }
        /* End of computation */


}
