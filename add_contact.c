#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "types.h"
#include "common.h"

Status add_contact(Contacts *contact)
{
    if (open_address_file(contact) == e_success)
    {
        char new_name[20];
    reenter_name:
        printf("Enter Name: ");
        scanf("%[^\n]", new_name);
        while (getchar() != '\n');

        if (strchr(new_name, ',') != NULL)
        {
            printf("Do not enter ',' character, please re-enter name\n");
            goto reenter_name;
        }

        if (search_in_file(contact, new_name, e_name) == e_success)
        {
            char mobile[11];

        mobile_reenter:
            printf("Enter Mobile Number: ");
            scanf("%10s", mobile);
            while (getchar() != '\n');

            int count = 0;
            for (int i = 0; i < 10; i++)
                if (mobile[i] >= '0' && mobile[i] <= '9')
                    count++;

            if (count != 10)
            {
                printf("10 Digits Not Entered, please re-enter\n");
                goto mobile_reenter;
            }
            else
            {
                char email[30];
            re_enter_email:
                printf("Enter Email ID: ");
                scanf("%s", email);
                while (getchar() != '\n');

                int ret = email_pattern_check(email);

                if (ret == e_success)
                {
                    char location[30];
                reenter_location:
                    printf("Enter Location: ");
                    scanf("%s", location);
                    while (getchar() != '\n');

                    if (strchr(location, ',') != NULL)
                    {
                        printf("Do not enter ',' character, please re-enter location\n");
                        goto reenter_location;
                    }

                    char confirm;
                    printf("Add contacts to record? Confirm(Y/y): ");
                    scanf("%c", &confirm);
                    while (getchar() != '\n')
                        ;

                    if (confirm == 'Y' || confirm == 'y')
                    {
                        fseek(contact->fptr, 0, SEEK_END);

                        fputs(new_name, contact->fptr);
                        fputc(',', contact->fptr);
                        fputs(mobile, contact->fptr);
                        fputc(',', contact->fptr);
                        fputs(email, contact->fptr);
                        fputc(',', contact->fptr);
                        fputs(location, contact->fptr);
                        fputc('\n', contact->fptr);

                        // fflush(contact->fptr);

                        fseek(contact->fptr, 1, SEEK_SET);
                        fprintf(contact->fptr, "%d", ++(contact->total_contacts));
                        fputc('#', contact->fptr);

                        fclose(contact->fptr);

                        printf("--------------------------------\nContact successfully added\n--------------------------------\n");
                    }
                    else
                        fclose(contact->fptr);
                }
                else
                {
                    fprintf(stderr, "----------------------------------------------------------------------\nEmail Invalid:\nPlease enter in format username@domain_name (Ex: john_cena@youcantsee-me.com)\nAllowed Characters: [a-zA-Z0-9_.+-] + @[a-zA-Z0-9-] + .[a-zA-Z0-9]\n----------------------------------------------------------------------\n");
                    goto re_enter_email;
                }
            }
        }
        else
        {
            fprintf(stderr, "---------------------------------------\nContact for %s already exists\n---------------------------------------\n", new_name);
        }
    }
    else
        return e_exit;
    return e_none;
}

int email_pattern_check(char *email)
{
    regex_t regex;

    int ret = regcomp(&regex, "^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\\.[a-zA-Z0-9]+$", REG_EXTENDED);
    if (ret) {
        fprintf(stderr, "[ERROR] Could not compile regular expression\n");
        return 1;
    }
    return regexec(&regex, email, 0, NULL, 0);
}
