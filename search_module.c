#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "common.h"

Status search_in_file(Contacts *contact, char *new_data, int mode)
{
    char *temp_buffer = calloc(100, sizeof(char));

    char printing_buffer[100];
    contact->con_found_count = 0;
    bzero(contact->found_at, 100*sizeof(*contact->found_at));

    for (int i = 0; i < contact->total_contacts; i++)
    {
        if (feof(contact->fptr))
            break;

        fgets(temp_buffer, 100, contact->fptr); // gets data until \n

        switch (mode)
        {
        case e_search_name:
        {
            strcpy(printing_buffer, temp_buffer);
            char name[30];
            sscanf(temp_buffer, "%[^,]", name);
            if (strcmp(name, new_data) == e_success || (strcasestr(name, new_data) != NULL))
            {
                contact->found_at[contact->con_found_count] = i + 1;
                print_contacts(printing_buffer, ++contact->con_found_count);
            }
        }
        break;
        case e_name:
        {
            char name[30];
            sscanf(temp_buffer, "%[^,]", name);
            
            if (strcasecmp(name, new_data) == e_success)
            {
                //file is closed in add_contact
                free(temp_buffer);
                return e_failure;
            }
        }
        break;
        case e_mobile:
        {
            strcpy(printing_buffer, temp_buffer);
            char *token = strtok(temp_buffer, ",");
            if (token != NULL)
            {
                token = strtok(NULL, ",");
                if (token != NULL)
                {
                    if (strcmp(token, new_data) == e_success || (strstr(token, new_data) != NULL))
                    {
                        contact->found_at[contact->con_found_count] = i + 1;
                        print_contacts(printing_buffer, ++contact->con_found_count);
                    }
                }
            }
        }
        break;
        case e_email:
        {
            strcpy(printing_buffer, temp_buffer);
            char *token = strtok(temp_buffer, ",");
            if (token != NULL)
            {
                token = strtok(NULL, ",");
                if (token != NULL)
                {
                    token = strtok(NULL, ",");
                    if (token != NULL)
                    {
                        if (strcmp(token, new_data) == e_success || (strcasestr(token, new_data) != NULL))
                        {
                            contact->found_at[contact->con_found_count] = i + 1;
                            print_contacts(printing_buffer, ++contact->con_found_count);
                        }
                    }
                }
            }
        }
        break;
        case e_location:
        {
            strcpy(printing_buffer, temp_buffer);
            char *token = strtok(temp_buffer, ",");
            if (token != NULL)
            {
                token = strtok(NULL, ",");
                if (token != NULL)
                {
                    token = strtok(NULL, ",");
                    if (token != NULL)
                    {
                        token = strtok(NULL, "\n");
                        if (token != NULL)
                        {
                            if (strcmp(token, new_data) == e_success || (strcasestr(token, new_data) != NULL))
                            {
                                contact->found_at[contact->con_found_count] = i + 1;
                                print_contacts(printing_buffer, ++contact->con_found_count);
                            }
                        }
                    }
                }
            }
        }
        }
    }

    free(temp_buffer);

    if (mode != e_name)
    {
        fclose(contact->fptr);
        if (contact->con_found_count == 0)
            return e_failure;
        else
            return e_success;
    }
    else
        return e_success;
}

Status search_contacts(Contacts *contact, OpType mode)
{
    if (open_address_file(contact) == e_success)
    {
        int foundflag = 0;
        int choice;
    reenter_menu:
        if (mode == e_search)
        {
            printf("--------------------------------\n");
            printf("Search Menu:\n1) Search Name\n2) Search Mobile Number\n3) Search Email ID\n4) Search Location\n5) Exit\n6) Previous Menu\nEnter Choice: ");
        }
        else if (mode == e_edit)
        {
            printf("--------------------------------\n");
            printf("Edit Menu:\n1) Edit Name\n2) Edit Mobile Number\n3) Edit Email ID\n4) Edit Location\n5) Exit\n6) Previous Menu\nEnter Choice: ");
        }
        else if (mode == e_delete)
        {
            printf("--------------------------------\n");
            printf("Delete Menu:\n1) Search by Name\n2) Search by Mobile Number\n3) Search by Email ID\n4) Search by Location\n5) Exit\n6) Previous Menu\nEnter Choice: ");
        }
        scanf("%d", &choice);
        while (getchar() != '\n');
        contact->edit_choice = choice;

        //for edit contact, search by name
        if(mode == e_edit && (choice > 0 && choice < 5)) choice = 1;

        switch (choice)
        {
        case 1:
        {
            char name[30];
            printf("Enter Name to search: ");
            scanf("%s", name);
            while (getchar() != '\n');
            return search_in_file(contact, name, e_search_name);
        }
        break;
        case 2:
        {
            char mobile[11];
            printf("Enter Mobile Number to search: ");
            scanf("%10s", mobile);
            while (getchar() != '\n');
            return search_in_file(contact, mobile, e_mobile);
        }
        break;
        case 3:
        {
            char email[30];
            printf("Enter Email ID to search: ");
            scanf("%s", email);
            while (getchar() != '\n')
                ;
            return search_in_file(contact, email, e_email);
        }
        break;
        case 4:
        {
            char location[30];
            printf("Enter Location to search: ");
            scanf("%s", location);
            while (getchar() != '\n')
                ;
            return search_in_file(contact, location, e_location);
        }
        break;
        case 5:
            return e_exit;
            break;
        case 6:
        printf("--------------------------------\n");
        return -1;
        default:
            printf("--------------------------------\nInvalid choice, please re-enter\n");
            goto reenter_menu;
        }
    }
    else
        return e_exit;
}
