#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"

#define VALID_LINE_SIZE 1024
#define ADD_PARAGRAPH "add_paragraph_after"
#define ADD_LINE "add_line_after"
#define PRINT "print_document"
#define QUIT "quit"
#define EXIT "exit"
#define APPEND "append_line"
#define REMOVE_LINE "remove_line"
#define LOAD "load_file"
#define REPLACE "replace_text"
#define HIGHLIGHT "highlight_text"
#define REMOVE_TEXT "remove_text"
#define SAVE "save_document"
#define RESET "reset_document"

int proccess_command(Document *doc, char *line, int *done);
int run_add_paragraph(Document *doc, char *line);
int run_add_line(Document *doc, char *line);
int run_print(Document *doc, char *line);
int run_quit(char *line, int *done);
int run_append(Document *doc, char *line);
int run_remove_line(Document *doc, char *line);
int run_load(Document *doc, char *line);
int run_replace(Document *doc, char *line);
int run_highlight(Document *doc, char *line);
int run_remove_text(Document *doc, char *line);
int run_save(Document *doc, char *line);
int run_reset(Document *doc, char *line);
void check_type(const char line[], int *type);
void remove_newline(char line[]);

int main(int argc, char *argv[]) {
    FILE *input;
    Document doc; 
    int done = 0, line_type = 0;
    char line[VALID_LINE_SIZE]; 
    
    init_document(&doc, "main_document");

    /*Makes sure correct # of arguments are inputted*/
    if(argc == 1) {
        input = stdin;
    } else if(argc == 2) {
        input = fopen(argv[1], "r");
        if(input == NULL) {
            fprintf(stderr, "%s cannot be opened.\n", argv[1]);
            exit(EX_OSERR);   
        }
    } else {
        /*Prints correct usage*/
        fprintf(stderr, "Usage: user_interface\n");
        fprintf(stderr, "Usage: user_interface <filename>\n");
        exit(EX_USAGE);
    }

    
    if(input == stdin) {
        printf("> ");
    }
    fgets(line, VALID_LINE_SIZE, input);
    while(!feof(input)) {
        remove_newline(line);
        check_type(line, &line_type);
        if(line_type == 0) { /*Is it a command?*/
            proccess_command(&doc, line, &done);
        }
        if(done == 1) { /*Check if quit or exit*/
            break;
        }
        
        /*Next iteration*/
        if(input == stdin) {
            printf("> ");
        }
        fgets(line, VALID_LINE_SIZE, input); 
    }

    if(argc == 2) {
        fclose(input);
    }
 
    exit(EXIT_SUCCESS);
}


int proccess_command(Document *doc, char *line, int *done) {
    int success = 0;
    /*if-else ladder determining what command it is*/
    if(strstr(line, ADD_PARAGRAPH) != NULL) {
        success = run_add_paragraph(doc, line);
    } else if(strstr(line, ADD_LINE) != NULL) {
        success = run_add_line(doc, line);
    } else if(strstr(line, PRINT) != NULL) {
        success = run_print(doc, line);
    } else if(strstr(line, QUIT) != NULL || strstr(line, EXIT) != NULL) {
        success = run_quit(line, done);
    } else if(strstr(line, APPEND) != NULL) {
        success = run_append(doc, line);
    } else if(strstr(line, REMOVE_LINE) != NULL) {
        success = run_remove_line(doc, line);
    } else if(strstr(line, LOAD) != NULL) {
        success = run_load(doc, line);
    } else if(strstr(line, REPLACE) != NULL) {
        success = run_replace(doc, line);
    } else if(strstr(line, HIGHLIGHT) != NULL) {
        success = run_highlight(doc, line);
    } else if(strstr(line, REMOVE_TEXT) != NULL) {
        success = run_remove_text(doc, line);
    } else if(strstr(line, SAVE) != NULL) {
        success = run_save(doc, line);
    } else if(strstr(line, RESET) != NULL) {
        success = run_reset(doc, line);
    } else {
        success = 1;
    }

    /*if deemeed invalid command, than the invalid command message is printed*/
    if(success != 0) {
        printf("Invalid Command\n");
    }
    return SUCCESS;
}

/*Runs the add paragraph function after parsing the correct info from string*/
int run_add_paragraph(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], additional_info[VALID_LINE_SIZE];
    int values_scanned = 0, para_num = 0;
    values_scanned = sscanf(line, "%s %d %s", command, &para_num, 
    additional_info); 
    /*Checking if valid command*/
    if(values_scanned == 2 && para_num >= 0) { 
        int did_it_work = add_paragraph_after(doc, para_num);
        if(did_it_work == FAILURE) {
            printf("add_paragraph_after failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Runs add line function*/
int run_add_line(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], new_line[MAX_STR_SIZE + 1];
    int para_num = 0, line_num = 0, values_scanned = 0, did_it_work = 10; 
    values_scanned = sscanf(line, "%s %d %d * %s", command, 
    &para_num, &line_num, new_line);
    if(values_scanned == 4 && para_num > 0 && line_num >= 0) {
        /*Copies entire string after * char */
        strcpy(new_line, strchr(line, '*') + 1);
        did_it_work = add_line_after(doc, para_num, line_num, new_line);
        if(did_it_work == FAILURE) {
            printf("add_line_after failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Runs print and makes sure that only the phrase print is there*/
int run_print(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], additional_info[VALID_LINE_SIZE];
    int values_scanned = 0;
    values_scanned = sscanf(line, "%s %s", command, additional_info);
    if(values_scanned == 1) {
        print_document(doc);
        return SUCCESS;
    }
    return FAILURE;
}


/*Changes while loop condition to done*/
int run_quit(char *line, int *done) {
    char command[VALID_LINE_SIZE], additional_info[VALID_LINE_SIZE];
    int values_scanned = 0;
    values_scanned = sscanf(line, "%s %s", command, additional_info);
    if(values_scanned == 1) {
        *done = 1;
        return SUCCESS;
    }
    return FAILURE;
}

/*Runs append command*/
int run_append(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], new_line[MAX_STR_SIZE + 1];
    int values_scanned = 0, para_num = 0, did_it_work = 0;
    values_scanned = sscanf(line, "%s %d * %s", command, &para_num, new_line);
    if(values_scanned >= 2 && para_num > 0 && strchr(line, '*') != NULL) {
        /*Makes sure conditions are met and copies everything after * char */
        strcpy(new_line, strchr(line, '*') + 1);
        did_it_work = append_line(doc, para_num, new_line);
        if(did_it_work == FAILURE) {
            printf("append_line failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Runs remove line command*/
int run_remove_line(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], additional_info[MAX_STR_SIZE + 1]; 
    int values_scanned = 0, para_num = 0, line_num = 0, did_it_work = 0;
    values_scanned = sscanf(line, "%s %d %d %s", command, &para_num, &line_num, additional_info);
    /*Checking conditions*/
    if(values_scanned == 3 && para_num > 0 && line_num > 0) {
        did_it_work = remove_line(doc, para_num, line_num);
        if(did_it_work == FAILURE) {
            printf("remove_line failed");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Runs load command and makes sure there is no additional info*/
int run_load(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], filename[MAX_STR_SIZE + 1], 
    additional_info[VALID_LINE_SIZE];
    int values_scanned = 0, did_it_work = 0;
    values_scanned = sscanf(line, "%s %s %s", command, filename, additional_info);
    if(values_scanned == 2) {
        did_it_work = load_file(doc, filename);
        if(did_it_work == FAILURE) {
            printf("load_file failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Runs replace command */
int run_replace(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], temp1[VALID_LINE_SIZE], 
    temp2[VALID_LINE_SIZE], target[MAX_STR_SIZE + 1], 
    replacement[MAX_STR_SIZE + 1], 
    *start_quote, *end_quote, *start_quote2, *end_quote2;

    int values_scanned = 0, did_it_work = 0;
    values_scanned = sscanf(line, "%s %s %s", command, temp1, temp2);
    start_quote = strstr(line, "\"");

    /*Finding references to quotation marks in order to copy the entire phrase*/
    if(values_scanned == 3 && start_quote != NULL) {
        end_quote = strstr(start_quote + 1, "\"");
        if(end_quote != NULL) {
            start_quote2 = strstr(end_quote + 1, "\"");
            if(start_quote2 != NULL) {
                end_quote2 = strstr(start_quote2 + 1, "\"");
                if(end_quote2 != NULL) {
                    /*Onces all 4 references are found, start calculating length
                    and coping over that many char to target and replacement*/
                    int length = end_quote - start_quote, 
                    length2 = end_quote2 - start_quote2;
                    strncpy(target, start_quote + 1, length);
                    target[length - 1] = '\0';
                    strncpy(replacement, start_quote2 + 1, length2);
                    replacement[length2 - 1] = '\0';

                    did_it_work = replace_text(doc, target, replacement);
                    if(did_it_work == FAILURE) {
                        printf("replace_text failed\n");
                    }
                    return SUCCESS;
                }
            }
        }
        
       
    }
    return FAILURE;

}

/*Runs highlight command*/
int run_highlight(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], temp[MAX_STR_SIZE + 1], 
    target[MAX_STR_SIZE + 1], *start_quote, *end_quote;
    int values_scanned = sscanf(line, "%s %s", command, temp);
    start_quote = strstr(line, "\"");

    /*Finds references to both quotations*/
    if(values_scanned == 2 && start_quote != NULL) {
        end_quote = strstr(start_quote + 1, "\"");
        if(end_quote != NULL) {
            /*Calculates length and copies over that many chars into target*/
            int length = end_quote - start_quote;
            strncpy(target, start_quote + 1, length);
            target[length - 1] = '\0';

            highlight_text(doc, target); /*No fail message*/
            return SUCCESS;
        }
      
    }
    return FAILURE;
}

/*Runs remove text command*/
int run_remove_text(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], temp[MAX_STR_SIZE + 1],
    target[MAX_STR_SIZE + 1], *start_quote, *end_quote;
    int values_scanned = sscanf(line, "%s %s", command, temp);
    start_quote = strstr(line, "\"");
    
    /*Finds references to quotations*/
    if(values_scanned == 2 && start_quote != NULL) {
        end_quote = strstr(start_quote + 1, "\"");
        if(end_quote != NULL) {
            /*Copies over char based on reference lengths*/
            int length = end_quote - start_quote;
            strncpy(target, start_quote + 1, length);
            target[length - 1] = '\0';
        
            remove_text(doc, target); /*No fail message*/
            return SUCCESS;
        }
    }
    return FAILURE;
}

/*Run save command and makes sure there is no additional info*/
int run_save(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], filename[MAX_STR_SIZE + 1], 
    additional_info[VALID_LINE_SIZE];
    int values_scanned = 0, did_it_work = 0;
    values_scanned = sscanf(line, "%s %s %s", command, filename, additional_info);
    if(values_scanned == 2) {
        did_it_work = save_document(doc, filename);
        if(did_it_work == FAILURE) {
            printf("save_document failed\n");
        }
        return SUCCESS;
    }
    return FAILURE;
}

/*Runs reset command and makes sure there is no additional info*/
int run_reset(Document *doc, char *line) {
    char command[VALID_LINE_SIZE], additional_info[MAX_STR_SIZE + 1];
    int values_scanned = 0;
    values_scanned = sscanf(line, "%s %s", command, additional_info);
    if(values_scanned == 1) {
        reset_document(doc);
        return SUCCESS;
    }
    return FAILURE;
}

/*Checks type of inputted line 
if it is a comment it will return 1 
if it is a blank line it will return 2
and if it a command it will return 0 so it can be processed further*/
void check_type(const char *line, int *type) {
    int i = 0;

    /*Finds first non-white space char*/
    while(isspace(line[i])) {
        i++;
    }

    if(line[i] == '#') {
        *type = 1; /*Comment*/
    } else if(strlen(line) == 0 || line[i] == '\0') {
        *type = 2; /*Blank line*/
    } else {
        *type = 0; /*Command*/
    }
}

/*Remove new line char*/
void remove_newline(char line[]) {
    int len = strlen(line);
    if(len > 0 && isspace(line[len - 1])) {
        line[len - 1] = '\0';
    }

}

