#ifndef _TYPES
#define _TYPES

typedef struct
{
    FILE *fptr;
    FILE *temp_fptr;
    int total_contacts;
    int found_at[100];
    int con_found_count;
    int edit_choice;
} Contacts;

typedef enum
{
    e_success,
    e_failure,
    e_exit
} Status;

typedef enum
{
    e_name,
    e_mobile,
    e_email,
    e_location,
    e_search_name
} EditType;

typedef enum
{
    e_edit,
    e_search,
    e_delete
}OpType;

#endif