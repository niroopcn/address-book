#ifndef _COMMON
#define _COMMON

Status add_contact(Contacts *contact);

int email_pattern_check(char *email);

Status open_address_file(Contacts *contact);

Status search_in_file(Contacts *contact, char *new_name, int mode);

Status search_contacts(Contacts *contact, OpType mode);

/*void strtok_print_contacts(char *printing_buffer, int count);*/

void print_contacts(char *printing_buffer, int count);

Status print_all_contacts(Contacts *contact);

Status edit_contact(Contacts *contact);

Status delete_contact(Contacts *contact);

Status open_temp_file(Contacts *contact);

#endif
