
#include "sys_funcs.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include <cstring>
#include <iostream>
#include <new>
#include <cstdint>
#include <string>
#include <pwd.h>
#include <vector>


const int error_ret = -1;

//===================================================================================================================

#define STDIN_FD 0
#define STDOUT_FD 1
#define STDERR_FD 2

//===================================================================================================================

ssize_t my_write (int fd, char* buffer, size_t count)
{
    ssize_t writed_symbols_cnt = 0;
    while (count != 0)
    {
        count -= writed_symbols_cnt;
        buffer += writed_symbols_cnt;

        writed_symbols_cnt = write(fd, buffer, count);
        MY_COOLER_ASSERT(writed_symbols_cnt >= 0, error_ret);
    }

    return 0;
}

void clean_buffer (char* buffer, size_t count)
{
    for (size_t cnt = 0; cnt != count; ++cnt)
    {
        buffer[cnt] = '\0';
    }
}

int output_file (const char* source_path, int destination_fd)
{
    int return_check = 0;

    int file_descriptor = open(source_path, O_RDONLY);
    MY_COOLER_ASSERT(file_descriptor >= 0, error_ret);

    return_check = read_plus_write(file_descriptor, destination_fd);
    MY_COOLER_ASSERT(return_check >= 0, error_ret);

    close(file_descriptor);

    return 0;
}

int read_plus_write (int source_fd, int destination_fd)
{
    const ssize_t buffers_size = 1024;
    char buffer[buffers_size]{};


    ssize_t writed_symbols = 0;
    ssize_t readed_symbols = 0;
    do
    {
        readed_symbols = read(source_fd, buffer, buffers_size);
        MY_COOLER_ASSERT(readed_symbols >= 0, error_ret);

        writed_symbols = buffers_size > readed_symbols ? readed_symbols : buffers_size;
        MY_COOLER_ASSERT(my_write(destination_fd, buffer, writed_symbols) >= 0, error_ret);

        clean_buffer(buffer, buffers_size);
    } while (readed_symbols > 0);

    return 0;
}

int copy_file (const char* source_path, const char* destination_path, Flags* flags)
{
    int rwx_mode = 0777;

    int oflags = O_WRONLY;

    if (flags->force_flag)
    {
        oflags = oflags | O_CREAT | O_TRUNC;
    }
    else if (flags->interactive_flag)
    {
        oflags = oflags | O_CREAT | O_EXCL;
    }

    if (flags->many_files)
    {
        oflags = oflags | O_APPEND;
    }

//     if (flags->interactive_flag)
//     {
//
//     }

    int destination_fd = open(destination_path, oflags, rwx_mode);
    if (destination_fd < 0 && flags->interactive_flag)
    {
        destination_fd = 0;

        printf ("my_cp: переписать '%s'? ", destination_path);

        char* answer_line = NULL;
        scanf ("%ms", &answer_line);

        if ((strncmp(answer_line, "yes", 3) == 0) ||  (strcmp(answer_line, "y") == 0))
        {
            destination_fd = open(destination_path, O_WRONLY | O_CREAT | O_TRUNC, rwx_mode);
            MY_COOLER_ASSERT(destination_fd >= 0, error_ret);
        }
        else
        {
            free (answer_line);

            return 0;
        }

        free (answer_line);
    }
    MY_COOLER_ASSERT(destination_fd >= 0, error_ret);

    MY_COOLER_ASSERT(output_file(source_path, destination_fd) >= 0, error_ret);

    close(destination_fd);

    if (flags->verbose_flag)
    {
        printf ("'%s' -> '%s'\n", source_path, destination_path);
    }

    return 0;
}

void print_help_cp ()
{
    printf (R"(
    Использование: cp [ПАРАМЕТР]… [-T] ИСТОЧНИК НАЗНАЧЕНИЕ
       или:    cp [ПАРАМЕТР]… ИСТОЧНИК… КАТАЛОГ
       или:    cp [ПАРАМЕТР]… -t КАТАЛОГ ИСТОЧНИК…
Копирует ИСТОЧНИК в НАЗНАЧЕНИЕ, или несколько ИСТОЧНИКОВ в КАТАЛОГ.

Аргументы, обязательные для длинных параметров, обязательны и для коротких.
  -a, --archive                то же, что и -dPR --preserve=all
      --attributes-only        не копировать данные файла, только атрибуты
      --backup[=КОГДА]         создавать резервную копию каждого целевого
                               файла
  -b                           то же, что и --backup, но без аргумента
      --copy-contents          копировать содержимое специальных файлов при
                               рекурсивной обработке
  -d                           то же, что и --no-dereference --preserve=link
  -f, --force                  если невозможно открыть существующий файл, то
                               удалить его и попробовать ещё раз (этот параметр
                               игнорируется, если также указан параметр -n)
  -i, --interactive            спрашивать перед тем как переписывать
                               (отменяет ранее указанный параметр -n)
  -H                           следовать по символьным ссылкам в ИСТОЧНИКЕ
  -l, --link                   создавать жёсткие ссылки вместо копирования
  -L, --dereference            всегда следовать символьным ссылкам в ИСТОЧНИКЕ
  -n, --no-clobber             не перезаписывать существующий файл (отменяет
                               ранее указанный ключ -i)
  -P, --no-dereference         не следовать по символьным ссылкам в ИСТОЧНИКЕ
  -p                           то же, что и
                               --preserve=mode,ownership,timestamps
      --preserve[=СПИС_АТТР]   сохранять указанные атрибуты (по умолчанию:
                               mode,ownership,timestamps) и, если возможно,
                               дополнительные атрибуты: context, links, xattr,
                               all
      --no-preserve=СПИС_АТТР  не сохранять указанные атрибуты
      --parents                добавить исходный путь к КАТАЛОГУ
  -R, -r, --recursive          рекурсивно копировать каталоги
      --reflink[=КОГДА]        контролировать копии clone/CoW. См. ниже.
      --remove-destination     удалять каждый файл назначения перед
                               попыткой его открыть (обратно к --force)
      --sparse=КОГДА           управлять созданием разреженных файлов.
                               См. ниже.
      --strip-trailing-slashes удалять все конечные косые черты из каждого
                               аргумента ИСТОЧНИК
  -s, --symbolic-link          создавать символьные ссылки вместо копирования
  -S, --suffix=СУФФИКС         использовать для запасных копий заданный
                               СУФФИКС
      --target-directory=КАТ   скопировать все ИСТОЧНИКИ в КАТАЛОГ
  -T, --no-target-directory    считать НАЗНАЧЕНИЕ обычным файлом
  -u, --update                 копировать только тогда когда исходный файл
                               новее чем файл назначения, или когда файл
                               назначения отсутствует
  -v, --verbose                пояснять что будет сделано
  -x, --one-file-system        оставаться в пределах одной файловой системы
  -Z                           установить контекст безопасности SELinux
                               файла назначения равным типу по умолчанию
      --context[=CTX]          подобно -Z, или если указан CTX, то установить
                               контекст безопасности SELinux
                               или SMACK равным CTX
      --help     показать эту справку и выйти
      --version  показать информацию о версии и выйти

По умолчанию, разреженные файлы ИСТОЧНИКА распознаются при помощи грубой
эвристической процедуры и соответственно создаются разреженные НАЗНАЧЕНИЯ.
Такое поведение задаётся при помощи параметр --sparse=auto.
С параметром --sparse=always всегда создаётся разреженный файл НАЗНАЧЕНИЯ, вне
зависимости от того, содержит ли ИСТОЧНИК достаточно длинные
последовательности нулевых байтов. Используйте параметр --sparse=never для
запрещения создания разреженных файлов.

Если указано --reflink[=always], выполняется облегчённое копирование, при
котором блоки данных копируются только при изменении. Если это невозможно,
или если указано --reflink=auto, то используется обычное копирование.
При указании --reflink=never всегда выполняется обычное копирование.

По умолчанию суффикс для запасных копий «~», если только не установлена
переменная окружения SIMPLE_BACKUP_SUFFIX или не задан параметр --suffix.
Способ контроля версий может быть установлен при помощи ключа --backup
или переменной окружения VERSION_CONTROL. Допустимые значения:

  none, off       никогда не создавать резервных копий (даже если
                  указан параметр --backup)
  numbered, t     создавать нумерованные копии
  existing, nil   пронумеровать, если уже существуют пронумерованные
                  резервные копии, иначе создавать простые
  simple. never   всегда создавать простые копии

Как исключение, cp создает резервную копию ИСТОЧНИКА, если заданы
параметры -f и -b, а если ИСТОЧНИК совпадает с НАЗНАЧЕНИЕМ, то cp создает
резервную копию НАЗНАЧЕНИЯ.

Страница справки по GNU coreutils: <https://www.gnu.org/software/coreutils/>
Об ошибках в переводе сообщений сообщайте по адресу <https://translationproject.org/team/ru.html>
Полная документация: <https://www.gnu.org/software/coreutils/cp>
или доступная локально: info '(coreutils) cp invocation'
        )");
}

//===================================================================================================================

void user_loop (char* input_line)
{
    struct passwd* password = getpwuid (getuid());

    while (true)
    {
        printf (KGRN "msh> " KBLU "%s" KNRM":" KRED "" KNRM "$ ", password->pw_name);

        scanf (" %[^\n]", input_line);

        std::vector<char*> words;
        parse_buffer_to_words (words, input_line);

        if ((strncmp(input_line, "quit", 4) == 0) || (strncmp(input_line, "q", 1) == 0))
        {
            return;
        }

        run_commands (words);
    }
}

void parse_buffer_to_words (std::vector<char*>& words, char* buffer)
{
    char* pointer_to_word = NULL;

    bool ready_to_set_ptr = true;

    uint64_t length = strlen (buffer);
    for (uint64_t cnt = 0; cnt != length; ++cnt)
    {
        if (buffer[cnt] == ' ')
        {
            ready_to_set_ptr = true;
        }

        if (ready_to_set_ptr && (buffer[cnt] != ' '))
        {
            pointer_to_word = &(buffer[cnt]);
            words.push_back(pointer_to_word);

            ready_to_set_ptr = false;
        }

        if ((buffer[cnt] == ' ') || buffer[cnt] == '\n')
        {
            buffer[cnt] = '\0';
        }
    }
}

void run_commands (std::vector<char*>& words)
{
    const int MAX_CMD_ARGS = 10;
    const int MAX_CMDS = 10;

    const char*** argv_arr = new const char** [MAX_CMDS];

    uint64_t process_cnt    = 0;
    uint64_t inner_cnt      = 0;
    for (uint64_t cnt = 0; cnt != words.size(); ++cnt)
    {

        if (strncmp (words[cnt], "|", 1) != 0)
        {
            if (inner_cnt == 0)
            {
                argv_arr[process_cnt] = new const char* [MAX_CMD_ARGS];
            }

            argv_arr[process_cnt][inner_cnt] = words[cnt];
            ++inner_cnt;
        }
        else
        {
            argv_arr[process_cnt][inner_cnt] = NULL;
            ++process_cnt;

            inner_cnt = 0;
        }
    }
    argv_arr[process_cnt][inner_cnt] = NULL;
    ++process_cnt;

    if (fork() == 0)
    {
        process_branching ((char* const**) argv_arr, 0, process_cnt);

        wait (NULL);

        for (uint64_t cnt = 0; cnt != process_cnt; ++cnt)
        {
            // print_argv(argv_arr[cnt]);

            delete[] argv_arr[cnt];
        }
        delete[] argv_arr;

        exit (0);
    }
    wait (NULL);

    for (uint64_t cnt = 0; cnt != process_cnt; ++cnt)
    {
        printf ("===\n");
        print_argv(argv_arr[cnt]);
        printf ("===\n");

        delete[] argv_arr[cnt];
    }
    delete[] argv_arr;
}

void process_branching (char* const** arr_argv, uint64_t cnt_process, uint64_t amnt_process)
{
    int pipe_ends[2];

    printf ("[%d] %ld/%ld\n", getpid(), cnt_process, amnt_process);
    // printf ("%ld -> %ld\n", cnt_process, cnt_process + 1);

    pipe(pipe_ends);
    if (fork () == 0)
    {
        if (cnt_process != amnt_process - 1)
        {
            close(pipe_ends[1]);
        }
        dup2(pipe_ends[1], STDOUT_FILENO);

        process_branching(arr_argv, cnt_process + 1, amnt_process);

        return;
    }
    close (pipe_ends[1]);

    wait (NULL);

    print_argv((const char**) arr_argv[cnt_process]);
    execvp(arr_argv[cnt_process][0], &(arr_argv[cnt_process][0]));
    perror("Exec failed");
}

void print_argv (const char* my_argv[])
{
    for (uint64_t cnt = 0; my_argv[cnt] != NULL; ++cnt)
    {
        printf ("{%s}\n", my_argv[cnt]);
    }
}

//===================================================================================================================
