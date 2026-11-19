#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialise all buckets in a hash table to NULL
void initialise_hash_table(HashTable *ht)
{
    // validate given pointer
    if (ht != NULL)
        for (int i = 0; i < HASH_TABLE_SIZE; i++)
            ht->buckets[i] = NULL;
}

// Put a user in a bucket
int get_bucket_for_user(int user_id)
{
    // validate input
    if (user_id >= 1 && user_id <= 2139)
        // generate and return hash table index
        return user_id % HASH_TABLE_SIZE;

    // if input is invalid
    return -1;
}

// Return pointer to user in a hash table or NULL
User *find_user(const HashTable *ht, int user_id)
{
    // validate hash table pointer
    if (ht != NULL)
    {
        // get user's bucket and validate user_id
        int uBucket = get_bucket_for_user(user_id);

        // find user in bucket
        if (uBucket != -1 && ht->buckets[uBucket] != NULL)
        {
            // set current user pointer
            User *current = ht->buckets[uBucket];

            while (current != NULL)
            {
                // if user exists return pointer
                if (current->user_id == user_id)
                    return current;

                // increment current user
                current = current->next;
            }
        }
    }
    // if there are any invalid inputs
    return NULL;
}

// Register new user or modify a current user
int register_user(HashTable *ht, int user_id, char password[])
{
    // validate hash table pointer
    if (ht != NULL)
    {
        // validate password
        int n = 0;
        char current = password[n];

        // loop until end of password (or end of character limit)
        while (current != '\0')
        {
            current = password[n];

            if (n > MAX_PASSWORD_LEN)
                return ERROR;
            n++;
        }


        // get user's bucket and validate user_id
        int uBucket = get_bucket_for_user(user_id);

        if (uBucket != -1)
        {
            // allocate memory for new user
            User *pUser = (User*)malloc(sizeof(User));
            if (pUser == NULL)
                return ERROR;

            // add passed in info to user
            pUser->user_id = user_id;
            strcpy(pUser->password, password);

            // add user to hash table
            // if there are no entries in bucket yet
            if (ht->buckets[uBucket] == NULL)
            {
                ht->buckets[uBucket] = pUser;
                return SUCCESS;
            }

            // if there are entries in bucket
            else
            {
                // set previous and current user pointers
                User *previous = NULL;
                User *current = ht->buckets[uBucket];
                do
                {
                    // if the user id is already in the table
                    if (current->user_id == user_id)
                    {
                        // go to current user and modify password
                        strcpy(current->password, pUser->password);
                        return SUCCESS;
                    }

                    // increment pointers
                    previous = current;
                    current = current->next;
                }
                while (current != NULL);

                // add new user if user isn't already in table
                // set "next" of last user in bucket to the new user
                previous->next = pUser;
                return SUCCESS;
            }
        }
    }
    // if there are errors
    return ERROR;
}

// Check if a password is valid for a given user
int is_valid_password(const HashTable *ht, int user_id, char password[])
{
    // determine if user id is valid and return a pointer to the user if this is the case
    User *pUser = find_user(ht, user_id);

    // check password
    if (pUser != NULL && strcmp(pUser->password, password) == 0)
        return SUCCESS;

    // if user is invalid or password is wrong
    return ERROR;
}

// Free memory of hash table
void free_buckets(HashTable *ht)
{
    // ensure ht is valid
    if (ht != NULL)
    {
        // loop for all buckets
        for (int i = 0; i < HASH_TABLE_SIZE; i++)
        {
            // make sure current bucket isn't empty
            if (ht->buckets[i] != NULL)
            {
                // set previous and current user pointers
                User *user_to_free = NULL;
                User *current = ht->buckets[i];
                do
                {
                    // increment pointers
                    user_to_free = current;
                    current = current->next;

                    // free user
                    free(user_to_free);
                }
                while (current != NULL);
            }
        }
    }
}

// Load users from file into hash table
int load_users_from_file(HashTable *ht, char filename[])
{
    // initialise file pointer
    // open file and make sure it exists
    FILE *pFile = fopen(filename, "r");
    if (pFile != NULL)
    {
        // read file
        // initialise vars to store user id and password
        char *usr, *pass;

        // read in first 46 characters of current line
        char line[46];
        while (fgets(line, 46, pFile))
        {
            // split line into user id and password
            usr = strtok(line, " ");

            // remove trailing newline
            pass = strtok(NULL, "\n");

            // convert user id from string to int
            int usr_id = atoi(usr);

            // register user
            if (register_user(ht, usr_id, pass) != SUCCESS)
                return ERROR;
        }

        // close file and return
        fclose(pFile);
        return SUCCESS;
    }
    return ERROR;
}
