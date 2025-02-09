/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filesystem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   Improved: by ChatGPT                             +#+  +:+       +#+        */
/*   Date: 2025/02/01                                 #+#    #+#             */
/*                                                    ###   ########.fr       */
/* ************************************************************************** */

#include <lib/filesystem/filesystem.h>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------------------------------------------------------------
// Error Handling
// --------------------------------------------------------------------------
const char *fs_get_error_message(int error_code) {
	switch (error_code) {
		case FS_SUCCESS:
			return "Success";
		case FS_ERROR:
			return "General error";
		case FS_NOT_FOUND:
			return "File or directory not found";
		case FS_PERMISSION_DENIED:
			return "Permission denied";
		case FS_ALREADY_EXISTS:
			return "File or directory already exists";
		case FS_ERROR_FILE_OPEN:
			return "Error opening file";
		case FS_ERROR_FILE_READ:
			return "Error reading file";
		case FS_ERROR_FILE_WRITE:
			return "Error writing file";
		case FS_ERROR_MEMORY_ALLOCATION:
			return "Memory allocation error";
		case FS_ERROR_INVALID_PATH:
			return "Invalid path";
		default:
			return "Unknown error";
	}
}

int fs_get_last_system_error(void) {
	return errno;
}

// --------------------------------------------------------------------------
// Metadata Operations
// --------------------------------------------------------------------------
t_metadata *fs_get_metadata(const char *path) {
	struct stat st;
	if (lstat(path, &st) < 0)
		return NULL;

	t_metadata *metadata = malloc(sizeof(t_metadata));
	if (!metadata)
		return NULL;

	metadata->owner_id		   = st.st_uid;
	metadata->group_id		   = st.st_gid;
	metadata->permissions	   = st.st_mode;
	metadata->access_time	   = st.st_atime;
	metadata->modify_time	   = st.st_mtime;
	metadata->change_time	   = st.st_ctime;
	metadata->creation_time	   = st.st_ctime; // Remplacer par st_birthtime si disponible
	metadata->inode			   = st.st_ino;
	metadata->hard_links	   = st.st_nlink;
	metadata->device_id		   = st.st_dev;
	metadata->is_symbolic_link = S_ISLNK(st.st_mode);
	metadata->symlink_target   = NULL;

	if (metadata->is_symbolic_link)
		metadata->symlink_target = fs_get_symlink_target(path);

	return metadata;
}

void fs_free_metadata(t_metadata *metadata) {
	if (metadata) {
		if (metadata->symlink_target)
			free(metadata->symlink_target);
		free(metadata);
	}
}

// --------------------------------------------------------------------------
// File Operations
// --------------------------------------------------------------------------
t_file *fs_open(const char *path, int mode) {
	if (!path)
		return NULL;

	t_file *file = malloc(sizeof(t_file));
	if (!file)
		return NULL;

	file->path = strdup(path);
	if (!file->path) {
		free(file);
		return NULL;
	}

	// Si O_CREAT est utilisé, fournir un mode par défaut (0644)
	if (mode & O_CREAT)
		file->fd = open(path, mode, 0644);
	else
		file->fd = open(path, mode);
	if (file->fd < 0) {
		free(file->path);
		free(file);
		return NULL;
	}

	struct stat st;
	if (fstat(file->fd, &st) < 0) {
		close(file->fd);
		free(file->path);
		free(file);
		return NULL;
	}

	file->size		   = st.st_size;
	file->is_directory = S_ISDIR(st.st_mode);
	file->mode		   = mode;
	file->offset	   = 0;
	file->metadata	   = fs_get_metadata(path);

	return file;
}

int fs_close(t_file *file) {
	if (!file)
		return FS_ERROR;

	if (file->fd >= 0)
		close(file->fd);
	if (file->path)
		free(file->path);
	if (file->metadata)
		fs_free_metadata(file->metadata);
	free(file);
	return FS_SUCCESS;
}

int fs_read(t_file *file, void *buffer, uint32_t size) {
	if (!file || !buffer || file->is_directory)
		return FS_ERROR;

	// Vérifier les droits d'accès à l'aide de fcntl()
	int flags = fcntl(file->fd, F_GETFL);
	if (flags == -1)
		return FS_ERROR_FILE_READ;
	int acc_mode = flags & O_ACCMODE;
	if (acc_mode != O_RDONLY && acc_mode != O_RDWR)
		return FS_PERMISSION_DENIED;

	ssize_t total_read = 0;
	ssize_t bytes_read = 0;
	while (total_read < size) {
		bytes_read = read(file->fd, (char *)buffer + total_read, size - total_read);
		if (bytes_read < 0)
			return FS_ERROR_FILE_READ;
		else if (bytes_read == 0) // Fin de fichier
			break;
		total_read += bytes_read;
		file->offset += bytes_read;
	}
	return total_read;
}

int fs_write(t_file *file, const void *buffer, uint32_t size) {
	if (!file || !buffer || file->is_directory)
		return FS_ERROR;

	// Vérifier les droits d'accès à l'aide de fcntl()
	int flags = fcntl(file->fd, F_GETFL);
	if (flags == -1)
		return FS_ERROR_FILE_WRITE;
	int acc_mode = flags & O_ACCMODE;
	if (acc_mode != O_WRONLY && acc_mode != O_RDWR)
		return FS_PERMISSION_DENIED;

	ssize_t total_written = 0;
	ssize_t bytes_written = 0;
	while (total_written < size) {
		bytes_written = write(file->fd, (const char *)buffer + total_written, size - total_written);
		if (bytes_written < 0)
			return FS_ERROR_FILE_WRITE;
		total_written += bytes_written;
		file->offset += bytes_written;
	}

	// Mettre à jour la taille du fichier
	if (total_written > 0) {
		struct stat st;
		if (fstat(file->fd, &st) == 0)
			file->size = st.st_size;
	}
	return total_written;
}

bool fs_exists(const char *path) {
	return access(path, F_OK) == 0;
}

int fs_create(const char *path) {
	if (fs_exists(path))
		return FS_ALREADY_EXISTS;

	int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return FS_ERROR;
	close(fd);
	return FS_SUCCESS;
}

int fs_delete(const char *path) {
	if (!fs_exists(path))
		return FS_NOT_FOUND;
	return unlink(path) == 0 ? FS_SUCCESS : FS_ERROR;
}

// --------------------------------------------------------------------------
// Directory Operations
// --------------------------------------------------------------------------
t_dir_entry *fs_list_directory(const char *path) {
	DIR *dir = opendir(path);
	if (!dir)
		return NULL;

	t_dir_entry *head	 = NULL;
	t_dir_entry *current = NULL;
	struct dirent *entry;

	while ((entry = readdir(dir))) {
		t_dir_entry *new_entry = malloc(sizeof(t_dir_entry));
		if (!new_entry) {
			fs_free_dir_entries(head);
			closedir(dir);
			return NULL;
		}

		new_entry->name = strdup(entry->d_name);
		if (!new_entry->name) {
			free(new_entry);
			fs_free_dir_entries(head);
			closedir(dir);
			return NULL;
		}

		char full_path[PATH_MAX];
		snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name);
		struct stat st;
		if (lstat(full_path, &st) == 0)
			new_entry->is_directory = S_ISDIR(st.st_mode);
		else
			new_entry->is_directory = false;
		new_entry->next = NULL;

		if (!head) {
			head	= new_entry;
			current = head;
		} else {
			current->next = new_entry;
			current		  = new_entry;
		}
	}
	// Vérifier si une erreur est survenue durant readdir()
	if (errno != 0) {
		fs_free_dir_entries(head);
		closedir(dir);
		return NULL;
	}

	closedir(dir);
	return head;
}

void fs_free_dir_entries(t_dir_entry *entries) {
	while (entries) {
		t_dir_entry *next = entries->next;
		free(entries->name);
		free(entries);
		entries = next;
	}
}

uint32_t fs_get_size(const char *path) {
	struct stat st;
	if (stat(path, &st) < 0)
		return 0;
	return st.st_size;
}

bool fs_is_directory(const char *path) {
	struct stat st;
	if (stat(path, &st) < 0)
		return false;
	return S_ISDIR(st.st_mode);
}

int fs_create_directory(const char *path) {
	if (fs_exists(path))
		return FS_ALREADY_EXISTS;
	return mkdir(path, 0755) == 0 ? FS_SUCCESS : FS_ERROR;
}

int fs_remove_directory(const char *path) {
	if (!fs_exists(path))
		return FS_NOT_FOUND;
	return rmdir(path) == 0 ? FS_SUCCESS : FS_ERROR;
}

int fs_mkdir_p(const char *path, mode_t mode) {
	char tmp[PATH_MAX];
	if (strlen(path) >= PATH_MAX)
		return FS_ERROR_INVALID_PATH;
	strcpy(tmp, path);
	char *p;
	int status = FS_SUCCESS;

	for (p = tmp + 1; *p; p++) {
		if (*p == '/') {
			*p = '\0';
			if (mkdir(tmp, mode) != 0 && errno != EEXIST)
				return FS_ERROR;
			*p = '/';
		}
	}

	if (mkdir(tmp, mode) != 0 && errno != EEXIST)
		status = FS_ERROR;
	return status;
}

// --------------------------------------------------------------------------
// Utility Operations
// --------------------------------------------------------------------------
int fs_copy(const char *src, const char *dst) {
	if (!fs_exists(src))
		return FS_NOT_FOUND;
	if (fs_exists(dst))
		return FS_ALREADY_EXISTS;

	t_file *src_file = fs_open(src, O_RDONLY);
	if (!src_file)
		return FS_ERROR;

	t_file *dst_file = fs_open(dst, O_WRONLY | O_CREAT | O_TRUNC);
	if (!dst_file) {
		fs_close(src_file);
		return FS_ERROR;
	}

	// Copier les permissions si les métadonnées sont disponibles
	if (src_file->metadata)
		fs_set_permissions(dst, src_file->metadata->permissions);

	char buffer[4096];
	int bytes_read = 0;
	int ret		   = FS_SUCCESS;
	while ((bytes_read = fs_read(src_file, buffer, sizeof(buffer))) > 0) {
		if (fs_write(dst_file, buffer, bytes_read) != bytes_read) {
			ret = FS_ERROR;
			break;
		}
	}
	// Si la lecture s'est terminée sur une erreur, la propager
	if (bytes_read < 0)
		ret = FS_ERROR_FILE_READ;

	fs_close(src_file);
	fs_close(dst_file);
	return ret;
}

int fs_move(const char *src, const char *dst) {
	if (rename(src, dst) == 0)
		return FS_SUCCESS;
	else if (errno == EXDEV) {
		// Différents devices : copier puis supprimer
		if (fs_copy(src, dst) == FS_SUCCESS)
			return fs_delete(src);
		else
			return FS_ERROR;
	} else
		return FS_ERROR;
}

bool fs_is_file(const char *path) {
	struct stat st;
	if (stat(path, &st) < 0)
		return false;
	return S_ISREG(st.st_mode);
}

int fs_get_permissions(const char *path) {
	struct stat st;
	if (stat(path, &st) < 0)
		return FS_ERROR;
	return st.st_mode & 0777;
}

int fs_set_permissions(const char *path, int mode) {
	return chmod(path, mode) == 0 ? FS_SUCCESS : FS_ERROR;
}

int fs_update_metadata(t_file *file) {
	if (!file)
		return FS_ERROR;
	if (file->metadata)
		fs_free_metadata(file->metadata);
	file->metadata = fs_get_metadata(file->path);
	return file->metadata ? FS_SUCCESS : FS_ERROR;
}

char *fs_get_symlink_target(const char *path) {
	char *buffer = malloc(PATH_MAX);
	if (!buffer)
		return NULL;
	ssize_t len = readlink(path, buffer, PATH_MAX - 1);
	if (len == -1) {
		free(buffer);
		return NULL;
	}
	buffer[len] = '\0';
	return buffer;
}

int fs_create_symlink(const char *target, const char *linkpath) {
	if (!target || !linkpath)
		return FS_ERROR;
	return symlink(target, linkpath) == 0 ? FS_SUCCESS : FS_ERROR;
}

time_t fs_get_creation_time(const char *path) {
	t_metadata *metadata = fs_get_metadata(path);
	if (!metadata)
		return 0;
	time_t creation_time = metadata->creation_time;
	fs_free_metadata(metadata);
	return creation_time;
}

time_t fs_get_access_time(const char *path) {
	t_metadata *metadata = fs_get_metadata(path);
	if (!metadata)
		return 0;
	time_t access_time = metadata->access_time;
	fs_free_metadata(metadata);
	return access_time;
}

time_t fs_get_modify_time(const char *path) {
	t_metadata *metadata = fs_get_metadata(path);
	if (!metadata)
		return 0;
	time_t modify_time = metadata->modify_time;
	fs_free_metadata(metadata);
	return modify_time;
}

int fs_set_owner(const char *path, uid_t owner, gid_t group) {
	if (!path)
		return FS_ERROR;
	return chown(path, owner, group) == 0 ? FS_SUCCESS : FS_ERROR;
}
