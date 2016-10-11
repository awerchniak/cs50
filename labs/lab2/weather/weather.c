/* weather.c
 * 
 * crawls the NOAA website for weather data around a 4 letter station code argument
 * prints stdout
 *
 * Created by Andy Werchniak based on a template from Xia Zhou, CS50. July 2016
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include "mycurl.h"
#include <string.h>

char *findString(char *xml, char *tag1, char *tag2);

/* main 
 * arguments: argc (# of inputs from command line), *argv[] (array of inputs from command line)
 * return: 0 if all goes well (see README.txt)
 * checks arguments for validity and executes curl function
 */
int main(int argc, char *argv[])
{
    /* local variables */
	char APIurl[57];							//string with 4 letter code substituted will be 56 characters long + '\0'
	
    /* check args: there must be exactly 1 */
	if (argc == 2){
		/* Check to make sure the code is 4 characters long */
		if(strlen(argv[1]) == 4);
			//all is well; do nothing
		else{
			printf("Location code must be 4 characters.\n");
			exit(1);
		}
	}
	else{
		printf("%sweather needs one arg, a location code like KMWN.  To look up location codes, see http://w1.weather.gov/xml/current_obs/seek.php?state=\n", argv[0]);
		exit(1);
	}
	
    /* curl stuff */
    CURL *curl;
    CURLcode res;
    struct curlResponse s;

    /* allocate and initialize the output area */
    init_curlResponse(&s);

    /* create the URL from the format string */
    sprintf(APIurl, "http://w1.weather.gov/xml/current_obs/%s.xml", argv[1]);
	
    /* initialize curl */
    curl = curl_easy_init();

    if(curl) {
        /* set the URL we want to visit */
        curl_easy_setopt(curl, CURLOPT_URL, APIurl );
        
        /* weather.gov expects USERAGENT option */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "cs50");
                
        /* set the function curl should call with the result */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        
        /* set the buffer into which curl should place the data */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        
        /* make sure it worked*/
		if(res!=0){
			printf("An error occurred with curl_easy_perform function.\n");
			exit(2);
		}
		
        /* check: if the weather station code wasn't found,
         * we get back HTML instead of XML */

        /* otherwise, select the desired output from the results */
		//first check if it's xml. If not, html code wasn't found
		if(s.ptr[1] != '?' || s.ptr[2] != 'x' || s.ptr[3] != 'm' || s.ptr[4] != 'l'){
			printf("That station code <%s> could not be found\n", argv[1]);
			exit(1);
		} else{
			printf("credit: %s\n", findString(s.ptr,"<credit>","</credit>"));
			printf("location: %s\n", findString(s.ptr,"<location>","</location>"));
			printf("station_id: %s\n", findString(s.ptr,"<station_id>","</station_id>"));
			printf("observation_time: %s\n", findString(s.ptr,"<observation_time>","</observation_time>"));
			printf("weather: %s\n", findString(s.ptr,"<weather>","</weather>"));
			printf("temperature_string: %s\n", findString(s.ptr,"<temperature_string>","</temperature_string>"));
			printf("relative_humidity: %s\n", findString(s.ptr,"<relative_humidity>","</relative_humidity>"));
			printf("wind_string: %s\n", findString(s.ptr,"<wind_string>","</wind_string>"));
			printf("visibility_mi: %s\n", findString(s.ptr,"<visibility_mi>","</visibility_mi"));
		}
		
        /* always cleanup */
        free(s.ptr);					//get rid of the 2140 bytes "definitely lost" that curl_easy_cleanup misses
        curl_easy_cleanup(curl);
        curl_global_cleanup();			//easy cleanup sucks and left 101k bytes still reachable...
    }

    return (0);
}

/* findString
 * find the string that occurs between two other strings
 * arguments: xml (xml code to parse), tag1 (first tag), tag2 (second tag)
 * returns: the string held in between those tags
 * useful for parsing xml, html, or anything with tags to separate information
 */
char *findString(char *xml, char *tag1, char *tag2)
{
	static char str[100];				//declare variables; string can be 100 characters long
	char *begin,*end,*crawl;
	int i = 0;
	
	begin = strstr(xml,tag1);			//begin points to the first occurence of e.g. "<credit>"
	begin+=strlen(tag1);				//begin points to the first character after e.g. "<credit>"
	
	end = strstr(xml,tag2);				//end points to first occurence of e.g. </credit>
	
	for(crawl=begin; crawl<end; crawl++)//crawl from the end of the first tag to the beginning of the second
		str[i++] = *crawl;				//add the character at that point to an output string
		
	str[i] = '\0';						//add '\0' to the array so that it's a string!
	
	return str;
}