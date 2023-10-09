#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <sys/types.h>

#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#include <errno.h>

#include "colors.h"

#define LOCATION __PRETTY_FUNCTION__, __FILE__, __LINE__

#define DOTXT(Message) #Message

#define MCP(Message)                                                                                       \
        do                                                                                                 \
        {                                                                                                  \
            fprintf (stderr, KNRM "errno [%d] Failed check " KRED Kbright DOTXT (Message) " "                              \
            KNRM "in " KYLW "%s" KNRM " in " KWHT Kunderscore "%s:%d\n" KNRM, errno,                             \
             __PRETTY_FUNCTION__, __FILE__, __LINE__ );                                                    \
        } while (0)


#define MY_COOLER_ASSERT(Condition, ReturnNum)                     \
        do                                            \
        {                                             \
            if (!(Condition))                         \
            {                                         \
                MCP (Condition);                      \
                return ReturnNum;                     \
            }                                         \
        } while (0)

#define MY_SILENT_ASSERT(Condition)                     \
        do                                            \
        {                                             \
            if (!(Condition))                         \
            {                                         \
                MCP (Condition);                      \
                fprintf (stderr, "errno(%d): %s\n", errno, strerror(errno)); \
            }                                         \
        } while (0)


int main (int argc, char** argv)
{
    const int MAX_GROUPS = 16;

    bool cmd_marker = false;

    if (argc > 1)
    {
        cmd_marker = true;
    }

    if (cmd_marker == false)
    {
        uint32_t size = getgroups(0, 0);
        MY_SILENT_ASSERT(size > 0);

        gid_t* list = (gid_t*) calloc (size, sizeof (gid_t));
        getgroups(size, list);

        uid_t uid = getuid ();
        struct passwd password =  *(getpwuid(uid));

        gid_t gid = getgid();
        struct group group =  *(getgrgid(gid));

        std::cout <<
        "UID " << uid << " " << password.pw_name    << std::endl <<
        "GID " << gid << " " << group.gr_name       <<  std::endl <<
        "========================================" <<
        std::endl << "size = " << size << std::endl;

        for (uint32_t cnt = 0; cnt != size; ++cnt)
        {
            std::cout << "[" << list[cnt] << "]" << " " << getgrgid(list[cnt])->gr_name << std::endl;
        }

        free (list);

        return 0;
    }

    char* user_name = argv[1];

    MY_SILENT_ASSERT(getpwnam(user_name) != NULL);
    if (getpwnam(user_name) == NULL)
    {
        perror("");
        std::cout << "my_id: \"" << argv[1] << "\": user not found" << std::endl;

        return 0;
    }

    int size = MAX_GROUPS;
    gid_t* list = (gid_t*) calloc (size, sizeof (gid_t));

    struct passwd password =  *(getpwnam(user_name));

    struct group group =  *(getgrnam(user_name));

    uid_t uid = password.pw_uid;
    gid_t gid = group.gr_gid;

    getgrouplist(user_name, gid, list, &size);

    std::cout <<
    "UID " << uid << " " << password.pw_name    << std::endl <<
    "GID " << gid << " " << group.gr_name       <<  std::endl <<
    "========================================" <<
    std::endl << "size = " << size << std::endl;

    for (int cnt = 0; cnt != size; ++cnt)
    {
        std::cout << "[" << list[cnt] << "]" << " " << getgrgid(list[cnt])->gr_name << std::endl;
    }

    free (list);

    return 0;
}
