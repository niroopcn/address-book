#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "types.h"
#include "common.h"

Status delete_contact(Contacts *contact)
{
    if (open_address_file(contact) == e_success)
    {
        int ret = search_contacts(contact, e_delete);
        if (ret != e_success)
            return ret; // return either e_failure or e_exit
        printf("--------------------------------\n");

        int index;
        if (contact->con_found_count > 1)
        {
        reenter_index:
            printf("Which contact do you want to delete?\nEnter index number: ");
            scanf("%d", &index);
            while (getchar() != '\n');
            if (index <= 0 || index > contact->con_found_count)
            {
                fprintf(stderr, "Please Enter valid index\n");
                goto reenter_index;
            }
            index = index - 1;
        }
        else
            index = 0;

        char confirm;
        printf("Confirm deletetion? (Y/y)");
        scanf("%c", &confirm);
        while(getchar() != '\n');

        if( confirm == 'Y' || confirm == 'y')
        {
        if (open_temp_file(contact) == e_success)
        {
            if (open_address_file(contact) == e_failure) return e_exit;

            fputc('#', contact->temp_fptr);

            fprintf(contact->temp_fptr, "%d", contact->total_contacts - 1);

            fputs("#      \n", contact->temp_fptr);

            char buffer[100];
            bzero(buffer, 100);

            for (int i = 0; i < contact->found_at[index] - 1; i++)
            {
                fgets(buffer, 100, contact->fptr);
                fputs(buffer, contact->temp_fptr);
            }

            //remove 1 contact
            fgets(buffer, 100, contact->fptr);

            while (1)
            {
                if(fgets(buffer, 100, contact->fptr) == NULL)
                break;

                fputs(buffer, contact->temp_fptr);
            }

            fclose(contact->fptr);
            fclose(contact->temp_fptr);

            remove("address_book.csv");
            rename("temp.csv","address_book.csv");

            return e_success;
        }
        else
            return e_exit;
        }
        else
        return -1;
    }
    else
        return e_exit;
}