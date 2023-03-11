#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "common.h"

Status print_all_contacts(Contacts *contact)
{
    if (open_address_file(contact) == e_success)
    {
        char temp_buffer[100];
        for (int i = 0; i < contact->total_contacts; i++)
        {
            if (feof(contact->fptr))
                break;

            fgets(temp_buffer, 100, contact->fptr);
            print_contacts(temp_buffer, i + 1);
        }
        fclose(contact->fptr);
        return e_success;
    }
    else
        return e_exit;
}

void print_contacts(char *printing_buffer, int count)
{
    int i = 0;
    for (int j = 0; j < 4; j++)
    {
        switch (j)
        {
        case e_name:
            printf("--------------------------------\n");
            printf("#%d# Name: ", count);
            break;
        case e_mobile:
            printf("Mobile No: ");
            break;
        case e_email:
            printf("Email ID: ");
            break;
        case e_location:
            printf("Location: ");
        }

        while (printing_buffer[i] != ',' && printing_buffer[i] != '\n')
            printf("%c", printing_buffer[i++]);
        printf("\n");
        i++;
    }
}

/*void strtok_print_contacts(char *printing_buffer, int count)
{
    char *name = strtok(printing_buffer, ",");
    if (name != NULL)
    {
        printf("--------------------------------\n");
        printf("#%d# Name: %s\n", count, name);
        char *mobile = strtok(NULL, ",");

        if (mobile != NULL)
        {
            printf("Mobile No: %s\n", mobile);
            char *email = strtok(NULL, ",");

            if (email != NULL)
            {
                printf("Email ID: %s\n", email);
                char *location = strtok(NULL, "\n");
                if (location != NULL)
                {
                    printf("Location: %s\n", location);
                }
            }
        }
    }
}*/