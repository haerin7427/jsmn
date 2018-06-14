#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */
typedef enum {
	C_RICE = 0,
	C_NOODLE = 1
} mycategory_t;

typedef struct {
	mycategory_t cat;
	char name[128];
	char size[128];
	//char price[128];
	int price;
} mymenu_t;
typedef struct {
        char model[100];
        char maker[128];
        int year;
        char gastype[100];
} car_t;
char* readjsonfile(const char* filename);
int makecarlist(const char* json, jsmntok_t *t, int tokcount, car_t *list[]){
        int i;
        int count = 0;

        for(i = 1; i <tokcount; i++){
                //find key:value(key = model,maker,year,gas
                if((t[i].size == 1) && (t[i+1].size == 0)){
                        list[count] = (car_t*)malloc(sizeof(car_t));
                        // i = value index
                        i += 1;
                        strncpy(list[count]->model, json + t[i].start,t[i].end - t[i].start);

                        i += 2;
                        strncpy(list[count]->maker, json + t[i].start ,t[i].end - t[i].start);

                        i += 2;
                        //change integer
                        char year[10];
                        strncpy(year, json + t[i].start ,t[i].end - t[i].start);
                        int year_t;
                        year_t = atoi(year);
                        list[count]->year = year_t;

                        i += 2;
                        strncpy(list[count]->gastype, json + t[i].start ,t[i].end - t[i].start);
                        count++;

                }

        }

        return count;           //number of car list
}

void printcarlist(car_t *list[],int carcount){
        printf("번호        모델명      제조사  제조년도  연료타입\n");
        int h;
        //print
        for(h = 0;h < carcount;h++){
                printf("%d  %15s %15s %6d %10s\n",h+1,list[h]->model,list[h]->maker,list[h]->year, list[h]->gastype);
        }
}

int makemymenu(const char* json, jsmntok_t *t, int tokcount, mymenu_t *m[]){

        int i, count = 0;
	char category[10];
	mycategory_t some;
        for(i = 1; i <tokcount; i++){
		if((t[i].size ==1) && (t[i+1].size != 0)){
			strncpy(category, json + t[i].start, t[i].end-t[i].start);
			if(strcmp(category, "rice")==0)
				some = C_RICE;
			else if(strcmp(category, "noodle") == 0)
				some = C_NOODLE;
		}
                if(t[i].size == 0){
			m[count] = (mymenu_t*)malloc(sizeof(mymenu_t));
			m[count]->cat = some;
                        strncpy(m[count]->name, json + t[i].start, t[i].end-t[i].start);
                        strncpy(m[count]->size, json + t[i+2].start, t[i+2].end-t[i+2].start);
                        i += 4;
			char temp[128];
			strncpy(temp, json + t[i].start, t[i].end-t[i].start);
			int t;
			t = atoi(temp);
                        //strncpy(m[count]->price, json + t[i].start, t[i].end-t[i].start);
			// m[count]->price = json + t[i].start;
			m[count]->price = t;
                        count++;
                }
        }
        return count;
}

void printmenu(mymenu_t *m[],int mcount){
	printf("***** MENU *****\n");
	int h;
	char typename[2][20] = {"C_RICE","C_NOODLE"};
	for(h=0;h<mcount;h++){
		printf("[%s] : %s <%d원> (%s)\n",typename[m[h]->cat],m[h]->name,m[h]->price, m[h]->size);
	}
}
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
char*readjsonfile(const char* filename);
void printall(const char *json, jsmntok_t *t, int tokcount){
	char typename[5][20] = {"JSMN_UNDEFINED","JSMN_OBJECT","JSMN_ARRAY","JSMN_STRING","JSMN_PRIMITIVE"};
	int i =0;
	printf("***** All Tokens *****\n");
	for (i=1; i< tokcount ;i++){
	printf("[%2d] %.*s (size = %d, %d~%d, %s)\n",i, t[i].end - t[i].start, json + t[i].start, t[i].size, t[i].start, t[i].end, typename[t[i].type]);
	}
}
void printkeys(const char *json, jsmntok_t *t, int tokcount){
        int i =0;
	int count = 0;

	printf("***** All Keys *****\n");

        for (i=1; i< tokcount ;i++){
		if((t[i].size == 1)&&(t[i].type == 3)){
			count++;
			printf("[%2d] %.*s (%d)\n",count, t[i].end - t[i].start, json + t[i].start, i);
		}
        }
}

int findkeys(const char *json, jsmntok_t *t, int tokcount, int* keys){
        int i =0;
        int count = 0;

        for (i=1; i< tokcount ;i++){
                if((t[i].size == 1)&&(t[i].type == 3)){
                        keys[count] = i;
                        count++;
                }        
	}

        return count;
}

void printvalues(const char *json, jsmntok_t *t, int tokcount, int* keys){
        int i,j=0;
        int index = 0;

	printf("***** All Keys : Values *****\n");
        for (i=1; i< tokcount ;i++){
		
		if(i == keys[j]){
			index++;
			printf("\n");
			printf("[%2d] %.*s : ",index, t[i].end - t[i].start, json + t[i].start);
			j++;
		}
		else{
			printf("%.*s ",t[i].end - t[i].start, json + t[i].start);
		}

        }
	printf("\n");

}


int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	char* JSON_STRING = readjsonfile("cars.json");

#ifdef DEBUG_MODE
        printf("\n<JSON_STRING>\n");
        printf("%s",JSON_STRING);2
        printf("\n=============\n");
#endif

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

#ifdef DEBUG_MODE
        for ( i=1; i < r; i++){
        printf("[%2d] (%d) %d~%d, size:%d\n", i, t[i].type, t[i].start, t[i].end, t[i].size);
        }
#endif


	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}
	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
			/* We may use strndup() to fetch string value */
			printf("- User: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
			int j;
			printf("- Groups:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
		}
	}
	car_t* carlist[10];
        int carcount = makecarlist(JSON_STRING, t, r,carlist);
        printcarlist(carlist, carcount);
	
	return EXIT_SUCCESS;
}

char* readjsonfile(const char* filename){
	char* restr = (char*)malloc(1);
	FILE* rfile;
	rfile = fopen(filename,"r");
	char line[1024];
	while (fgets(line, sizeof(line), rfile) != NULL) {

		
		restr = (char *)realloc(restr, strlen(restr) + strlen(line));
		strcat(restr, line );
		
	}
	fclose(rfile);
	//printf("%s\n",restr);

	return restr;
}
/*
int makemymenu(const char* json, jsmntok_t *t, int tokcount, mymenu_t *m[]){

	int i, count = 0;
	for(i = 1; i <tokcount; i++){
		if(t[i].size == 0){
			strncpy(m[count]->name, json + t[i].start, t[i].end-t[i].start+1);
			strncpy(m[count]->size, json + t[i+2].start, t[i+2].end-t[i+2].start+1);
			i += 4;
			int temp[10];
			strncpy(temp, json + t[i].start, t[i].end-t[i].start+1);
			m[count]->price = temp;
			count++;
		}
	}
	return count;
}
*/
