#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "common.h"

int main()
{
    int exitflag = 0;
    Contacts contact; // structure variable declaration
    while (!exitflag)
    {
        int choice;
        printf("Menu:\n1) Add Contact\n2) Edit Contact\n3) Delete Contact\n4) Search Contact(s)\n5) Print All Contacts\n6) Exit\nEnter choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice)
        {
        case 1:
            if (add_contact(&contact) == e_exit) exitflag = 1;
            break;
        case 2:
        {
            int ret = edit_contact(&contact);
            if( ret == e_exit) exitflag = 1;
            else if(ret == e_failure)
            printf("--------------------------------\nContact not found to edit\n--------------------------------\n");
            else if(ret == e_success)
            printf("--------------------------------\nContact Edited Successfully\n--------------------------------\n");
        }
            break;
        case 3:
        {
            int ret = delete_contact(&contact);
            if( ret == e_exit) exitflag = 1;
            else if(ret == e_failure)
            printf("--------------------------------\nContact not found to delete\n--------------------------------\n");
            else if(ret == e_success)
            printf("--------------------------------\nContact Deleted Successfully\n--------------------------------\n");
        }
        break;
        case 4:
        {
            int ret = search_contacts(&contact, e_search);
            if (ret == e_success)
                printf("--------------------------------\n%d contact(s) found\n--------------------------------\n", contact.con_found_count);
            else if (ret == e_failure)
                printf("--------------------------------\nContact not found\n--------------------------------\n");
            else if (ret == e_exit)
                exitflag = 1;
        }
        break;
        case 5:
        {
            int ret = print_all_contacts(&contact);
            if (ret == e_exit)
                exitflag = 1;
            else
                printf("--------------------------------\n%d contact(s) present in total\n--------------------------------\n", contact.total_contacts);
        }
        break;
        case 6:
            exitflag = 1;
            break;
        default:
            printf("--------------------------------\nInvalid choice, please re-enter\n--------------------------------\n");
        }
    }
    return 0;
}

Status open_address_file(Contacts *contact)
{
    contact->fptr = fopen("address_book.csv", "r+");
    if (contact->fptr == NULL)
    {
        perror("fopen");
        printf("Unable to open address_book.csv file, file is deleted\n");
        return e_failure;
    }

    fseek(contact->fptr, 1, SEEK_SET);
    char temp_buffer[100];
    fscanf(contact->fptr, "%[^\n]", temp_buffer);
    getc(contact->fptr);
    sscanf(temp_buffer, "%d", &contact->total_contacts);

    return e_success;
}

Status open_temp_file(Contacts *contact)
{
    contact->temp_fptr = fopen("temp.csv", "w");
    if (contact->temp_fptr == NULL)
    {
        perror("fopen");
        printf("Unable to open temp.csv file\n");
        return e_failure;
    }
    return e_success;
}
