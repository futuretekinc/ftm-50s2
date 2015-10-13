#include <stdio.h>

main()
{
     printf("Content-type: text/html\n\n");
          printf(" INPUT <HR>\n");
               printf("%s", getenv("QUERY_STRING"));
               }
