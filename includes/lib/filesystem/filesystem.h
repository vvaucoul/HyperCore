/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filesystem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Improved: by ChatGPT                             +#+  +:+       +#+        */
/*   Date: 2025/02/01                                 #+#    #+#             */
/*                                                    ###   ########.fr       */
/* ************************************************************************** */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

// Déclarations des fonctions système si non définies
extern ssize_t readlink(const char *pathname, char *buf, size_t bufsiz);
extern int symlink(const char *target, const char *linkpath);
extern int lstat(const char *pathname, struct stat *statbuf);
extern int mkdir(const char *pathname, mode_t mode);

#ifndef DT_DIR
#define DT_DIR 4
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// Codes d'erreur
#define FS_SUCCESS 0
#define FS_ERROR -1
#define FS_NOT_FOUND -2
#define FS_PERMISSION_DENIED -3
#define FS_ALREADY_EXISTS -4
#define FS_ERROR_FILE_OPEN -5
#define FS_ERROR_FILE_READ -6
#define FS_ERROR_FILE_WRITE -7
#define FS_ERROR_MEMORY_ALLOCATION -8
#define FS_ERROR_INVALID_PATH -9

typedef struct s_metadata {
	uid_t owner_id;
	gid_t group_id;
	mode_t permissions;
	time_t access_time;
	time_t modify_time;
	time_t change_time;
	time_t creation_time;
	uint64_t inode;
	uint32_t hard_links;
	uint64_t device_id;
	bool is_symbolic_link;
	char *symlink_target;
} t_metadata;

typedef struct s_file {
	char *path;
	int fd;
	int mode;
	off_t offset;
	uint32_t size;
	bool is_directory;
	t_metadata *metadata;
} t_file;

typedef struct s_dir_entry {
	char *name;
	bool is_directory;
	struct s_dir_entry *next;
} t_dir_entry;

// --------------------------------------------------------------------------
// Déclarations de fonctions
// --------------------------------------------------------------------------

// Error Handling
const char *fs_get_error_message(int error_code);
int fs_get_last_system_error(void);

// File Operations
t_file *fs_open(const char *path, int mode);
int fs_close(t_file *file);
int fs_read(t_file *file, void *buffer, uint32_t size);
int fs_write(t_file *file, const void *buffer, uint32_t size);
bool fs_exists(const char *path);
int fs_create(const char *path);
int fs_delete(const char *path);
uint32_t fs_get_size(const char *path);
bool fs_is_file(const char *path);

// Directory Operations
t_dir_entry *fs_list_directory(const char *path);
void fs_free_dir_entries(t_dir_entry *entries);
bool fs_is_directory(const char *path);
int fs_create_directory(const char *path);
int fs_remove_directory(const char *path);
int fs_mkdir_p(const char *path, mode_t mode);

// Metadata Operations
int fs_get_permissions(const char *path);
int fs_set_permissions(const char *path, int mode);
t_metadata *fs_get_metadata(const char *path);
void fs_free_metadata(t_metadata *metadata);
int fs_update_metadata(t_file *file);
char *fs_get_symlink_target(const char *path);
int fs_create_symlink(const char *target, const char *linkpath);
time_t fs_get_creation_time(const char *path);
time_t fs_get_access_time(const char *path);
time_t fs_get_modify_time(const char *path);
int fs_set_owner(const char *path, uid_t owner, gid_t group);

// Utility Operations
int fs_copy(const char *src, const char *dst);
int fs_move(const char *src, const char *dst);

#endif // FILESYSTEM_H
