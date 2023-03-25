#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "types.h"
#include "common.h"

Status edit_contact(Contacts *contact)
{
    if (open_address_file(contact) == e_failure)
        return e_exit;

    int ret = search_contacts(contact, e_edit);
    if (ret != e_success)
        return ret; // return either e_failure or e_exit
    printf("--------------------------------\n");

    int index;
    if (contact->con_found_count > 1)
    {
    reenter_index:
        printf("Which contact do you want to edit?\nEnter index number: ");
        scanf("%d", &index);
        while (getchar() != '\n')
            ;
        if (index <= 0 || index > contact->con_found_count)
        {
            fprintf(stderr, "Please Enter valid index\n");
            goto reenter_index;
        }
        index = index - 1;
    }
    else
        index = 0;

    // reopen fptr, it was closed while searching
    if (open_address_file(contact) == e_failure)
        return e_exit;

    char new_data[50];
    bzero(new_data, 50);

    int edit_at = contact->found_at[index];

reenter_data:
    switch (contact->edit_choice)
    {
    case 1:
        printf("Enter New Name: ");
        break;
    case 2:
        printf("Edit New Mobile Number: ");
        break;
    case 3:
        printf("Edit New Email ID: ");
        break;
    case 4:
        printf("Enter New Location: ");
    }

    if (contact->edit_choice == 2)
        scanf("%10s", new_data);
    else
        scanf("%[^\n]", new_data);
    while (getchar() != '\n')
        ;

    if (strchr(new_data, ',') != NULL)
    {
        printf("Do not enter ',' character, please re-enter data\n");
        goto reenter_data;
    }

    if (contact->edit_choice == 1)
    {
        int ret1 = search_in_file(contact, new_data, e_name);

        // close and open again to reset positions
        fclose(contact->fptr);
        if (open_address_file(contact) == e_failure)
            return e_exit;

        if (ret1 == e_failure)
        {
            printf("----------------------------------------------------------------\nContact with name %s is already present, please re-enter\n----------------------------------------------------------------\n", new_data);
            goto reenter_data;
        }
    }
    else if (contact->edit_choice == 2)
    {
        int count = 0;
        for (int i = 0; i < 10; i++)
            if (new_data[i] >= '0' && new_data[i] <= '9')
                count++;

        if (count != 10)
        {
            printf("10 Digits Not Entered, please re-enter\n");
            goto reenter_data;
        }
    }
    else if (contact->edit_choice == 3)
    {
        if (email_pattern_check(new_data) != 0)
        {
            fprintf(stderr, "----------------------------------------------------------------------\nEmail Invalid:\nPlease enter <email(azA-Z0-9+_.-)>@<domain(azA-Z0-9-)>.<com>/<.edu>...\n----------------------------------------------------------------------\n");
            goto reenter_data;
        }
    }

    if (open_temp_file(contact) == e_failure)
        return e_exit;

    // reposition fptr
    fseek(contact->fptr, 0, SEEK_SET);

    char buffer[100];
    bzero(buffer, 100);

    for (int i = 0; i < edit_at; i++)
    {
        fgets(buffer, 100, contact->fptr);
        fputs(buffer, contact->temp_fptr);
    }

    // data to edit
    fgets(buffer, 100, contact->fptr);

    switch (contact->edit_choice)
    {
    case 1: // name
    {
        fputs(new_data, contact->temp_fptr);
        int i = 0;
        while (buffer[i] != ',')
            i++;
        fputs(buffer + i, contact->temp_fptr);
    }
    break;
    case 2: // mobile
    {
        int i = 0, count = 0;
        while (buffer[i] != ',')
        {
            fputc(buffer[i], contact->temp_fptr);
            i++;
        }
        fputc(',', contact->temp_fptr);
        fputs(new_data, contact->temp_fptr);
        fputc(',', contact->temp_fptr);

        i = 0;
        while (count != 2)
            if (buffer[i++] == ',')
                count++;
        fputs(buffer + i, contact->temp_fptr);
    }
    break;
    case 3: // email
    {
        int i = 0, count = 0;
        while (count != 2)
        {
            fputc(buffer[i], contact->temp_fptr);
            if (buffer[i] == ',')
                count++;
            i++;
        }
        fputs(new_data, contact->temp_fptr);
        fputc(',', contact->temp_fptr);

        i = 0, count = 0;
        while (count != 3)
            if (buffer[i++] == ',')
                count++;
        fputs(buffer + i, contact->temp_fptr);
    }
    break;
    case 4: // location
    {
        int i = 0, count = 0;
        while (count != 3)
        {
            fputc(buffer[i], contact->temp_fptr);
            if (buffer[i] == ',')
                count++;
            i++;
        }
        fputs(new_data, contact->temp_fptr);
        fputc('\n', contact->temp_fptr);
    }
    }

    while (1)
    {
        if (fgets(buffer, 100, contact->fptr) == NULL)
            break;

        fputs(buffer, contact->temp_fptr);
    }

    fclose(contact->fptr);
    fclose(contact->temp_fptr);

    remove("address_book.csv");
    rename("temp.csv", "address_book.csv");

    return e_success;
}