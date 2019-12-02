#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CVS_MAX_LINE (20000)
#define LINE_MAX_LEN (1024)
#define WARN() do{printf("%s %d\n", __FUNCTION__, __LINE__);}while(0)

typedef struct{
	char name[1024]; //the tweeter name
	int count;       //the fre of tweeter
	int flag;        //just for sort
}tweeter_item_t;

typedef struct{
	//store every element of  title item
	/*
	for example :
	,tweet_id,airline_sentiment,airline_sentiment_confidence
	title_title[0] ="";
	title_title[1] ="tweet_id";
	title_title[3] ="airline_sentiment";
	title_title[4] ="airline_sentiment_confidence";
	*/		
	char item[LINE_MAX_LEN][LINE_MAX_LEN];  
	int posOfname;//the postion of name or names;
	int numberOfBankspace;
	int count;//the numbe of col
}row_item_t;

static int max_tweeter = 0;
static int total_tweeter_num = 0;
static tweeter_item_t tweeter_item[CVS_MAX_LINE];
static row_item_t  title_item = {{0}};

int found_commas_from_context(char* context, int len)
{
	int i;
	
	if (context == NULL || len <=0){
		printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
		exit(0);
	}

	for(i = 0; i < len; i++){
		char c = context[i];
		if (c == ','){
			return i;
		}
	}
	
	return -1;
}
void show_title_item()
{	
	return;
	int i = 0;
	for(i = 0; i < title_item.count; i++){
		printf("%d/%d:%s\n", i, title_item.count, title_item.item[i]);
	}
	//printf("---\n");
}
void clean_line_tail(char* context)
{
    if (context == NULL){
        return;        
    }

    if (context[strlen(context) - 1] == 0x0a){
        context[strlen(context) - 1] =0;
    }
    if (context[strlen(context) - 1] == 0x0d){
        context[strlen(context) - 1] =0;
    }
}
int parse_entry_line(char* context, int len)
{
	int i = 0;
	int k = 0;
	int h = 0;

	if (context == NULL || len <=0){
		return -1;
	}
    int found = 0;
	int numberOfcommas = 0;
	char tmp[LINE_MAX_LEN] = {0};
	char c = context[i];
	//printf("=3=>context:%s\n", context);
	while(i < LINE_MAX_LEN){
		if (c == ','){
			numberOfcommas ++;
			strcpy(title_item.item[h], tmp);
            //printf("%s,%d\n",title_item.item[h], numberOfcommas);
			if (strcmp(tmp, "name") == 0){
                found = 1;
				if (title_item.posOfname == 0){
					title_item.posOfname = numberOfcommas - 1;
				}else{
					printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
					exit(0);					
				}
			}else if (strcmp(tmp, "\"name\"") == 0){
			    found = 1;
				if (title_item.posOfname == 0){
					title_item.posOfname = numberOfcommas - 1; 
				}else{
					printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
					exit(0);
				}
			}else if (strlen(tmp) == 0){
				title_item.numberOfBankspace++;
				if (title_item.numberOfBankspace>1){
					printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
					exit(0);
				}
			}
			
			title_item.count++;
			memset(tmp, 0, sizeof(tmp));
			k = 0;
			h++;
		}
		else if (c == '\0'){
            if (tmp[strlen(tmp) - 1] == 0x0a){
                tmp[strlen(tmp) - 1] =0;
            }
            if (tmp[strlen(tmp) - 1] == 0x0d){
                tmp[strlen(tmp) - 1] =0;
            }            
			strcpy(title_item.item[h], tmp);            
			if (strcmp(tmp, "name") == 0){
                found = 1;
				if (title_item.posOfname == 0){
					title_item.posOfname = numberOfcommas; 
				}else{
					printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
					exit(0);
				}
			}else if (strcmp(tmp, "\"name\"") == 0){
			    found = 1;
				if (title_item.posOfname == 0){
					title_item.posOfname = numberOfcommas; 
				}else{
					printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
					exit(0);
				}
			}else if (strlen(tmp) == 0){
				title_item.numberOfBankspace++;
				if (title_item.numberOfBankspace>1){
					printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
					exit(0);
				}
			}
			
			title_item.count++;			
			break;

		}
		else{
			tmp[k] = c;		
			k++;
		}		
		i++;
		c = context[i];
	}	
    //printf("==>commas:%d\n",title_item.posOfname);
    if (found == 0){
        printf("Invalid input file.\n");
        exit(0);
    }
	return 0;
}

int parse_title_line(char* context, int len)
{
	int res = 0;
	if (context == NULL || len <=0){
		printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
		exit(0);
	}
    //printf("2:%s\n", context);
	//found split of ','
	res = found_commas_from_context(context, len);
	if (res < 0){
		printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
		exit(0);
	}

	res = parse_entry_line(context, len);
	if (res < 0){
		printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
		exit(0);
	}
	//found the name of 'name'		
	return res;
}

int parse_context_line(char* context, int len, row_item_t* cxt)
{
	int i = 0;
	int k = 0;
	int h = 0;
	//printf("--->len:%u; %s\n", len, context);
	if (context == NULL || len <=1){
		printf("Invalid input file <%s %d>\n", __FUNCTION__, __LINE__);
		exit(0);
	}
	
	char tmp[LINE_MAX_LEN] = {0};
	char c = context[i];

	
	while(i < LINE_MAX_LEN){
		if (c == ','){			
			strcpy(cxt->item[h], tmp);			
			cxt->count++;
			memset(tmp, 0, sizeof(tmp));
			k = 0;
			h++;
		}
		else if (c == '\0'){			
			strcpy(cxt->item[h], tmp);			
			cxt->count++;			
			break;
		}
		else{
			tmp[k] = c;		
			k++;
		}		
		i++;
		c = context[i];
	}	
	//printf("%d <%s %d>\n", cxt->count, __FUNCTION__, __LINE__);
	return 0;

}
int compare_and_count(row_item_t* cxt)
{
	int i = 0;

	if (cxt->count != title_item.count){
		printf("Invalid input file %d %d<%s %d>\n",cxt->count, title_item.count, __FUNCTION__, __LINE__);
		exit(0);
	}
	
	for(i = 0; i < title_item.count; i++){
		int have_double_quote  = 0;
		char head = title_item.item[i][0];
		char tail = title_item.item[i][strlen(title_item.item[i]) -1];

		if (head == '"' && tail == '"'){
			have_double_quote = 1;	
		}
		if (have_double_quote){
			head = cxt->item[i][0];
			tail = cxt->item[i][strlen(cxt->item[i])-1];
			if (head == '"' && tail == '"'){
				;
			}else{
				
				printf("Invalid input file <%s %d>\n",__FUNCTION__, __LINE__);
				exit(0);
			}
		}else{
			head = cxt->item[i][0];
			tail = cxt->item[i][strlen(cxt->item[i])-1];
			
			if (head == '"' && tail == '"'){
				//printf("%s|%s|%c |%c\n", title_item.item[i], cxt->item, head, tail);
				printf("Invalid input file <%s %d>\n",__FUNCTION__, __LINE__);
				exit(0);
			}
		}
		
		if (i == title_item.posOfname){
			int k = 0;
			for(k = 0; k < CVS_MAX_LINE; k++){
				if (tweeter_item[k].name[0] == 0){
					total_tweeter_num++;
					strcpy(tweeter_item[k].name, cxt->item[i]);
					tweeter_item[k].count = 1;
					//printf("new tweeter:%s; %d\n", tweeter_item[i].name, tweeter_item[i].count);
					break;
				}
				if (strcmp(tweeter_item[k].name, cxt->item[i])==0){
					tweeter_item[k].count += 1;
					//printf("old tweeter:%s %d\n", tweeter_item[i].name, tweeter_item[i].count);
					if (max_tweeter < tweeter_item[k].count){					
						max_tweeter = tweeter_item[k].count;
					}				
					break;
				}
			}			
		}
	}	
    return 0;
}
int load_and_parse_csv(FILE* fp)
{
	int line = 0;
	int ret = 0;
	char* result = NULL;
	char  tmp[LINE_MAX_LEN] = {0};
	if (NULL == fp){
		return -1;
	}

	result = fgets(tmp, sizeof(tmp), fp);
	if (NULL == result){
		printf("Err:failed to read first line\n");
		return -1;
	}
    //tmp[strlen(tmp)-1] = 0;
	//find the pos of commas 
	clean_line_tail(tmp);
	ret = parse_title_line(tmp, strlen(tmp));
	if (ret < 0){
		return -1;
	}

	show_title_item();
	//return 0;
	//printf("pos:%d\n", ret);
	int empty = 0;
	while(!feof(fp)) {
		memset(tmp, 0, sizeof(tmp));
		result = fgets(tmp, sizeof(tmp), fp);

		if (result == NULL){
			break;
		}

		//find_name_and_record(tmp, strlen(tmp), ret);
		row_item_t cxt;
		memset(&cxt, 0, sizeof(row_item_t));
        
		clean_line_tail(tmp);
		if (strlen(tmp) == 1){
			empty++;
			if (empty == 2){
				printf("Invalid input file\n");
				exit(0);
			}
			continue;
		}
		empty = 0;
		parse_context_line(tmp, strlen(tmp), &cxt);
		
		compare_and_count(&cxt);
	}	
	return 0;
}
int sort_tweeter()
{
	int i = 0;
	int k = 0;
	int max = 0;
	int pos;
	for(i = 0; i < 10; i++){		
		pos = -1;
		max = -1;
		for(k = 0; k < CVS_MAX_LINE; k++){
			if (tweeter_item[k].name[0] == 0){
				break;
			}
			if (tweeter_item[k].flag == 1){
				continue;
			}
			
			if (tweeter_item[k].count > max){
				max= tweeter_item[k].count;
				pos = k;
				//printf("%s %d %d\n", tweeter_item[k].name,tweeter_item[k].count, max, pos);
			}
		}
		
		if (pos == -1){
			//printf("sort end,i :%d\n", i);
			break;
		}
        char tmp[1024] = {0};
        strcpy(tmp, tweeter_item[pos].name);
        if (tmp[0] == '"' && tmp[strlen(tmp) - 1] == '"'){
            char tmp2[1024] = {0};
            strcpy(tmp2,&tmp[1]);
            tmp2[strlen(tmp2)-1]=0;
            printf("%s: %d\n", tmp2, tweeter_item[pos].count);
        }else{
            printf("%s: %d\n", tweeter_item[pos].name, tweeter_item[pos].count);
        }
        
		
		tweeter_item[pos].flag = 1;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	int res = 0;
	FILE* fp = 0;
	if (argc != 2){
		printf("Usage: Tweeter filename\n");
		return 0;
	}

	fp = fopen(argv[1], "r");
	if (NULL == fp){
		printf("Err:failed to open cvs file,pls check\n");
		return 0;
	}
	
	/*
		calculates the top 10 tweeters (by volume of tweets) in a given CSV file of tweets.
		For valid inputs it should produce 10 lines of output:

		In the case of an invalid input the program should output :Invalid Input Format.
		line:max[1024]
		the numbe of line is less than 20000
	*/
	res = load_and_parse_csv(fp);
	if (res < 0){
		fclose(fp);
		return 0;
	}	
	sort_tweeter();	
	fclose(fp);
	return 0;
}

