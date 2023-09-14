#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "document.h"

/*Sahil Patel, 119012512, spatel88*/
int init_document(Document *doc, const char *name) {
    if(doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
        return FAILURE;
    }

    doc->number_of_paragraphs = 0;
    strcpy(doc->name, name);

    return SUCCESS;
}

int reset_document(Document *doc) {
    if(doc == NULL) {
        return FAILURE;
    }

    doc->number_of_paragraphs = 0;
    return SUCCESS;
}

int print_document(Document *doc) { 
    int i = 0, j = 0; 
    Paragraph *paragraph;
    if (doc == NULL) {
        return FAILURE;
    }

    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        if(i != 0) {
            printf("\n");
        } 
        paragraph = &doc->paragraphs[i];
        for (j = 0; j < paragraph->number_of_lines; j++) {
            printf("%s\n", paragraph->lines[j]);
        }
    }

    return SUCCESS;
}

int add_paragraph_after(Document *doc, int paragraph_number) {
    int i = 0;
    Paragraph new_paragraph;
    if(doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS 
    || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }

    for(i = doc->number_of_paragraphs; i > paragraph_number; i--) {
        doc->paragraphs[i] = doc->paragraphs[i - 1];
    }

    new_paragraph.number_of_lines = 0;
    doc->paragraphs[paragraph_number] = new_paragraph;
    doc->number_of_paragraphs++;

    return SUCCESS;
}

int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line) {
    Paragraph *paragraph;
    int i = 0;
    if(doc == NULL || paragraph_number > doc->number_of_paragraphs 
    || doc->paragraphs[paragraph_number - 1].number_of_lines == 
    MAX_PARAGRAPH_LINES 
    || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines + 1
    || new_line == NULL) {
        return FAILURE;
    }

    paragraph = &doc->paragraphs[paragraph_number - 1]; 
    

    for(i = paragraph->number_of_lines; i > line_number; i--) {
        strcpy(paragraph->lines[i], paragraph->lines[i - 1]);
    }

    strcpy(paragraph->lines[line_number], new_line);
    paragraph->number_of_lines++;

    return SUCCESS;
}

int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines) {
    if(doc == NULL || number_of_lines == NULL 
    || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }

    *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    return SUCCESS;
}

int append_line(Document *doc, int paragraph_number, const char *new_line) {
    Paragraph *paragraph; 
    if(doc == NULL || paragraph_number > doc->number_of_paragraphs 
    || doc->paragraphs[paragraph_number - 1].number_of_lines == 
    MAX_PARAGRAPH_LINES || new_line == NULL) {
        return FAILURE;
    }

    paragraph = &doc->paragraphs[paragraph_number - 1];
    return add_line_after(doc, paragraph_number, paragraph->number_of_lines,
     new_line);
}

int remove_line(Document *doc, int paragraph_number, int line_number) {
    Paragraph *paragraph;
    int i = 0;
    if(doc == NULL || paragraph_number > doc->number_of_paragraphs 
    || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
        return FAILURE;
    }
    paragraph = &doc->paragraphs[paragraph_number - 1];
    for (i = line_number - 1; i < paragraph->number_of_lines - 1; i++) {
        strcpy(paragraph->lines[i], paragraph->lines[i + 1]);
    }

    paragraph->number_of_lines--;

    return SUCCESS;
}

int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines) {
    int paragraph_counter = 0, i = 0; 
    if(doc == NULL || data == NULL || data == 0) {
        return FAILURE;
    }
    add_paragraph_after(doc, paragraph_counter++);
    for(i = 0; i < data_lines; i++) {
        if(strlen(data[i]) == 0) {
            add_paragraph_after(doc, paragraph_counter);
            paragraph_counter++;
        } else {
            append_line(doc, paragraph_counter, data[i]);
        }
    }

    return SUCCESS;
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    int p = 0, l = 0;
    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }

    for (p = 0; p < doc->number_of_paragraphs; p++) {
        Paragraph *paragraph = &doc->paragraphs[p];

        for (l = 0; l < paragraph->number_of_lines; l++) {
            char *line = paragraph->lines[l];
            char *occurrence = strstr(line, target);

            while (occurrence != NULL) {
                int target_length = strlen(target);
                int occurrence_length = strlen(occurrence);
                int replacement_length = strlen(replacement);

                int new_line_length = strlen(line) - target_length + replacement_length;

                if (new_line_length <= MAX_STR_SIZE) {
                    char temp_line[MAX_STR_SIZE + 1];
                    temp_line[0] = '\0';

                    strncat(temp_line, line, occurrence - line);

                    strncat(temp_line, replacement, replacement_length);

                    strncat(temp_line, occurrence + target_length, occurrence_length - target_length);

                    strcpy(line, temp_line);

                }

                occurrence = strstr(line, target);
            }
        }
    }

    return SUCCESS;
}

int highlight_text(Document *doc, const char *target) {
    int p = 0, l = 0;
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    for (p = 0; p < doc->number_of_paragraphs; p++) {
        Paragraph *paragraph = &doc->paragraphs[p];

        for (l = 0; l < paragraph->number_of_lines; l++) {
            char *line = paragraph->lines[l];
            char *occurrence = strstr(line, target);

            while (occurrence != NULL) {
                int target_length = strlen(target);
                int highlighted_line_length = strlen(line) + strlen(HIGHLIGHT_START_STR) + strlen(HIGHLIGHT_END_STR) + 1;

                if (highlighted_line_length <= MAX_STR_SIZE) {
                    char temp_line[MAX_STR_SIZE + 1];
                    strncpy(temp_line, line, occurrence - line);
                    temp_line[occurrence - line] = '\0';

                    strcat(temp_line, HIGHLIGHT_START_STR);
                    strcat(temp_line, target);
                    strcat(temp_line, HIGHLIGHT_END_STR);
                    strcat(temp_line, occurrence + target_length);

                    strcpy(line, temp_line);

                }

                occurrence = strstr(occurrence + strlen(HIGHLIGHT_START_STR) + target_length + strlen(HIGHLIGHT_END_STR), target);
            }
        }
    }

    return SUCCESS;
}

int remove_text(Document *doc, const char *target) {
    int p = 0, l = 0, 
    target_length = 0, occurrence_length = 0;
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }


    for (p = 0; p < doc->number_of_paragraphs; p++) {
        Paragraph *paragraph = &doc->paragraphs[p];

        for (l = 0; l < paragraph->number_of_lines; l++) {
            char *line = paragraph->lines[l];
            char *occurrence = strstr(line, target);

            while (occurrence != NULL) {
                target_length = strlen(target);
                occurrence_length = strlen(occurrence);

                memmove(occurrence, occurrence + target_length, occurrence_length - target_length + 1);

                occurrence = strstr(line, target);
            }
        }
    }

    return SUCCESS;
}

/*Checks if the line is empty based on isspace*/
int check_line(const char* line) {
    while (*line) {
        if (!isspace(*line)) {
            return 0; 
        }
        line++;
    }
    return 1; 
}

/*Removes the new line character*/
void trim(char line[]) {
    int len = strlen(line);
    if(len > 0 && isspace(line[len - 1])) {
        line[len - 1] = '\0';
    }

}

int load_file(Document *doc, const char *filename) {
    FILE *input = NULL;
    int paragraph_count = 0;
    char line[MAX_STR_SIZE + 1];
    if(doc == NULL || filename == NULL ) {
        return FAILURE;
    } 

    input = fopen(filename, "r");
    if(input == NULL) {
        return FAILURE;
    }

    add_paragraph_after(doc, paragraph_count++); 
    while(fgets(line, MAX_STR_SIZE + 1, input) != NULL) {
        trim(line);
        if(check_line(line)) {
            add_paragraph_after(doc, paragraph_count++);
        } else {
            append_line(doc, paragraph_count, line);
        }
    }

    fclose(input);
    return SUCCESS;
}


int save_document(Document *doc, const char *filename) {
    FILE *output = NULL;
    int i = 0, j = 0;
    Paragraph *paragraph; 
    if(doc == NULL || filename == NULL) {
        return FAILURE;
    }

    output = fopen(filename, "w");
    if(output == NULL) {
        return FAILURE;
    }

    for (i = 0; i < doc->number_of_paragraphs; i++) {
        if(i != 0) {
            fputs("\n", output);
        } 
        paragraph = &doc->paragraphs[i];
        for (j = 0; j < paragraph->number_of_lines; j++) {
            fputs(paragraph->lines[j], output);
            fputs("\n", output);
        }
    }
  
    fclose(output);
    return SUCCESS;
}