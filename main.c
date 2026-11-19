#include "hash_table.h"
#include <stdio.h>

int main() {
    HashTable ht;
    initialise_hash_table(&ht);

    HashTable bad_ht;

    // register users up to 1000 with the password "passwd"
    for (int i = 1; i < 1001; i++)
        register_user(&ht, i, "passwd");

    // bucket for user test
    int valid_user = 1;
    int invalid_user = 0;
    printf("bucket %d: %d\n", valid_user, get_bucket_for_user(valid_user));
    printf("bucket %d: %d\n", invalid_user, get_bucket_for_user(invalid_user));

    printf("\nSEPERATE TESTS\n\n");

    // find existing users
    int inexistent = 2000;
    printf("user %d: %p\n", valid_user, find_user(&ht, valid_user));
    printf("user %d: %p\n", inexistent, find_user(&ht, inexistent));

    // I DONT KNOW HOW TO MAKE SURE HT IS VALID
    //printf("invalid ht %d: %p\n", valid_user, find_user(&bad_ht, valid_user));


    printf("\nSEPERATE TESTS\n\n");


    // register user with password "different
    register_user(&ht, 1000, "different");

    // change 200's password to "hello"
    register_user(&ht, 200, "hello");

    // is valid password
    printf("user %d: %d\n", 1000, is_valid_password(&ht, 1000, "different"));
    printf("user %d: %d\n", 200, is_valid_password(&ht, 200, "different"));
    printf("user %d: %d\n", 200, is_valid_password(&ht, 200, "passwd"));
    printf("user %d: %d\n", 200, is_valid_password(&ht, 200, "hello"));

    printf("\nSEPERATE TESTS\n\n");

    // load from file
    printf("Before %d: %s\n", 123, find_user(&ht, 123)->password);

    load_users_from_file(&ht, "users.txt");

    printf("After %d: %s\n", 123, find_user(&ht, 123)->password);

    printf("New %d: %s\n", 1213, find_user(&ht, 1213)->password);


    printf("\nSEPERATE TESTS\n\n");

    printf("%d\n", register_user(&ht, 2100, "12345678901234567890123456789012345678900"));
    printf("%d\n", register_user(&ht, 2101, "1234567890123456789012345678901234567890"));

    printf("\nSEPERATE TESTS\n\n");

    printf("%d\n", is_valid_password(&ht, 2, "passwd"));
    printf("%d\n", is_valid_password(&ht, 2, "jirgd"));
    printf("%d\n", is_valid_password(&ht, 0, "passwd"));


    free_buckets(&ht);
    return 0;
}
