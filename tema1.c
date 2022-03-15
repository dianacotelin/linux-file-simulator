//321CD Cotelin Maria-Diana
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

File* create_f(char* name) {
	File* file;
	file = malloc(sizeof(File));
	file->name = malloc(MAX_INPUT_LINE_SIZE);
	strcpy(file->name, name);
	file->next = NULL;
	file->parent = NULL;

	return file;
}
void touch (Dir* parent, char* name) {
	File* tmp;
	tmp = parent->head_children_files;
	Dir* directory_tmp;
	directory_tmp = parent->head_children_dirs;

	while (directory_tmp != NULL) {
		if (strcmp(name, directory_tmp->name) == 0) {
			printf("Directory already exists\n");
			return;
		}
		directory_tmp = directory_tmp->next;
	}
	
	File* tmp2;
	while (tmp != NULL) {
		if (strcmp(tmp->name, name) == 0) {
		printf("File already exists\n");
		return;
		}
		tmp2 = tmp;
		tmp = tmp->next;
	}

	File* file;
	file = create_f(name);
	file->parent = parent;
	if (parent->head_children_files == NULL) {
		parent->head_children_files = file;

	}
	else
		tmp2->next = file;
	
}

Dir* create_d(char* name) {
	Dir* directory;
	directory = malloc(sizeof(Dir));
	directory->parent = NULL;
	directory->name = malloc(MAX_INPUT_LINE_SIZE);
	strcpy(directory->name, name);
	directory->head_children_files = NULL;
	directory->head_children_dirs = NULL;
	directory->next = NULL; 
	return directory;
}
void mkdir (Dir* parent, char* name) {
	Dir* directory_tmp;
	directory_tmp = parent->head_children_dirs;

	File* file_tmp;
	file_tmp = parent->head_children_files;
	while (file_tmp != NULL) {
			if (strcmp(name, file_tmp->name) == 0) {
				printf("File already exists\n");
				return;
			}
			file_tmp = file_tmp->next;
	}
		

	Dir* tmp2;
	while(directory_tmp != NULL) {
		if (strcmp(directory_tmp->name, name) == 0) {
			printf("Directory already exists\n");
			return;
		}
		tmp2 = directory_tmp;
		directory_tmp = directory_tmp->next;
	}

	Dir* directory;
	directory = create_d(name);
	directory->parent = parent;
	if(parent->head_children_dirs == NULL) {
		parent->head_children_dirs = directory;
	}
	else
		tmp2->next = directory;

}



void ls (Dir* parent) {
	Dir* tmp_dir;
	tmp_dir = parent->head_children_dirs;
	File* tmp_f;
	tmp_f = parent->head_children_files;

	while (tmp_dir != NULL) {
		printf("%s\n", tmp_dir->name);
		tmp_dir = tmp_dir->next;
	}
	
	while (tmp_f != NULL) {
		printf("%s\n", tmp_f->name);
		tmp_f = tmp_f->next;
	}

}

void rm (Dir* parent, char* name) {
	File* tmp;
	File* tmp2;
	tmp = parent->head_children_files;
	
	if (tmp == NULL) {
		printf("Could not find the file\n");
		return;
	}
	if ((tmp->next == NULL) && (strcmp(tmp->name, name) == 0)) {
		parent->head_children_files = NULL;
		free(tmp->name);
		free(tmp);
		return;
	}
	if (strcmp(tmp->name, name) == 0) {
		parent->head_children_files = parent->head_children_files->next;
		free(tmp->name);
		free(tmp);
		return;
	}
	while (tmp->next != NULL) {
		if(strcmp(tmp->name, name) == 0) {
			tmp2->next = tmp->next;
			File* tmp3;
			tmp3 = tmp;
			tmp = tmp->next;
			free(tmp3->name);
			free(tmp3);
			return;
		}
		else {
			tmp2 = tmp;
			tmp = tmp->next;
		}
	}
	if (strcmp(tmp->name, name) == 0) {
		tmp2->next = NULL;
		free(tmp->name);
		free(tmp);
		return;
	}
	printf("Could not find the file\n");
}

void rmtraversal (Dir* d) {
	if( d == NULL)
		return;
	
	Dir* dir_tmp;
	while (d->head_children_dirs != NULL) {
		dir_tmp = d->head_children_dirs->next;
		rmtraversal(d->head_children_dirs);
		d->head_children_dirs = dir_tmp;
	}

	File* file;
	File* file_tmp;
	file = d->head_children_files;
	while (file != NULL) {
		file_tmp = file->next;
		rm(d, file->name);
		file = file_tmp;
	}
	free(d->name);
	free(d);

}
void rmdir (Dir* parent, char* name) {
	Dir* tmp_dir;
	Dir* tmp2;
	tmp_dir = parent->head_children_dirs;

	if (tmp_dir == NULL) {
		printf("Could not find the dir\n");
		return;
	}
	if ((tmp_dir->next == NULL) && (strcmp(tmp_dir->name, name) == 0)) {
		tmp2 = parent->head_children_dirs;
		parent->head_children_dirs = NULL;
		rmtraversal(tmp2);
		return;
	}
	if (strcmp(tmp_dir->name, name) == 0) {
		parent->head_children_dirs = parent->head_children_dirs->next;
		rmtraversal(tmp_dir);
		return;
	}

	while (tmp_dir->next != NULL) {
		if (strcmp(tmp_dir->name, name) == 0) {
			tmp2->next = tmp_dir->next;
			Dir* tmp3;
			tmp3 = tmp_dir;
			tmp_dir = tmp_dir->next;
			rmtraversal(tmp3);
			return;
		}
		else {
			tmp2 = tmp_dir;
			tmp_dir = tmp_dir->next;
		}	
	}
	
	if (strcmp(tmp_dir->name, name) == 0) {
		tmp2->next = NULL;
		rmtraversal(tmp_dir);
		return;
	}
	printf("Could not find the dir\n");
}

void cd(Dir** target, char *name) {
	Dir* tmp;
	Dir* tmp2;
	tmp = *target;
	tmp2 = *target;
	tmp = tmp->head_children_dirs;

	if (strcmp(name, "..") == 0) {
		if(tmp2->parent != NULL)
			*(target) = tmp2->parent;
		return;
	}
	while (tmp != NULL) {
		if (strcmp(tmp->name, name) == 0) {
			*(target) = tmp;
			return;
		}
		tmp = tmp->next;
	}
	printf("No directories found!\n");
	
}

char *pwd (Dir* target) {
	char* ret;
	char* ret2;
	ret2 = malloc(MAX_INPUT_LINE_SIZE);
	ret = malloc(MAX_INPUT_LINE_SIZE);
	strcpy(ret2, "/");
	strcat(ret2, target->name);
	strcpy(ret, ret2);
	target = target->parent;
	while (target != NULL) {
		strcpy(ret2, "/" );
		strcat(ret2, target->name);
		strcat(ret2, ret);
		strcpy(ret, ret2);
		target = target->parent;
	}
	free(ret2);
	return ret;
	
}


void tree (Dir* target, int level) {
	if ((target->head_children_files == NULL) && (target->head_children_dirs == NULL)) {
		return;
	}
	Dir* tmp;
	int level_2;
	level_2 = level + 1;
	tmp = target->head_children_dirs;
	while (tmp != NULL) {
		
		for (int i = 0; i < level_2; i++) {
				printf("    ");
		}
		printf("%s\n", tmp->name);
		
		tree(tmp, level_2);
		tmp = tmp->next;
	}
	
	
	File* tmp_f;
	tmp_f = target->head_children_files;
	while (tmp_f != NULL) {
		for(int i = 0; i < level_2; i++)
			printf("    ");
		printf("%s\n", tmp_f->name);
		tmp_f = tmp_f->next;
	}
	return;
}

void stop (Dir* target) {
	Dir* tmp;
	Dir* tmp2;
	tmp = target->head_children_dirs;
	while (tmp != NULL) {
		tmp2 = tmp;
		tmp = tmp->next;
		rmdir(target, tmp2->name);
	}
	
	File* tmp_file;
	File* tmp_file2;
	tmp_file = target->head_children_files;
	while (tmp_file != NULL) {
		tmp_file2 = tmp_file;
		tmp_file = tmp_file->next;
		rm(target, tmp_file2->name);
	}
	
}

void mv(Dir* parent, char *oldname, char *newname) {
	Dir* tmp_d;
	int ok_old_f = 0;
	int ok_new_f = 0;
	int ok_old_d = 0;
	int ok_new_d = 0;
	tmp_d = parent->head_children_dirs;
	while (tmp_d != NULL) {
		if (strcmp(tmp_d->name, oldname) == 0)
			ok_old_d = 1;
		if (strcmp(tmp_d->name, newname) == 0)
			ok_new_d = 1;
		tmp_d = tmp_d->next;
	}
	File* tmp_f;
	tmp_f = parent->head_children_files;
	while (tmp_f != NULL) {
		if (strcmp(tmp_f->name, oldname) == 0)
			ok_old_f = 1;
		if (strcmp(tmp_f->name, newname) == 0)
			ok_new_f = 1;
		tmp_f = tmp_f->next;
	}
	if ((ok_old_f == 0)&&(ok_old_d == 0)) {
		printf("File/Director not found\n");
		return;
	}
	if ((ok_new_d != 0)||(ok_new_f != 0)) {
		printf("File/Director already exists\n");
		return;
	}
	if (ok_old_d != 0) {
		rmdir(parent, oldname);
		mkdir(parent, newname);
		return;
	}
	if (ok_old_f != 0) {
		rm(parent, oldname);
		touch(parent, newname);
		return;
	}
	
}

int main () {
	
	char* line =(char*) malloc(MAX_INPUT_LINE_SIZE);

	Dir* home;
	home = (Dir*)malloc(sizeof(Dir));
	home->parent = NULL;
	home->next = NULL;
	home->head_children_dirs = NULL;
	home->head_children_files = NULL;
	home->name = malloc(MAX_INPUT_LINE_SIZE);
	strcpy(home->name, "home");
	char* command;
	char* create;
	char* create_mv;
	Dir* home2 = home;
	do
	{
		/*
		Summary:
			Reads from stdin a string and breaks it down into command and in
			case it needs into a name.
		*/
		
		fgets(line, MAX_INPUT_LINE_SIZE, stdin);
		line[strcspn(line, "\n")] = 0;
		
		command = strtok(line, " ");
		if (strcmp(command, "mv") == 0) {
			create = strtok(NULL, " ");
			create_mv = strtok(NULL, "0");
			mv(home, create, create_mv);
		}
		else
			create = strtok(NULL, "0");
		if (strcmp(command, "ls") == 0) {
			ls(home2);
		}
		if (strcmp(command, "touch") == 0) {
			touch(home2, create);
		}
		if (strcmp(command, "mkdir") == 0) {
			mkdir(home2, create);
		}
		if (strcmp(command, "rm") == 0) {
			rm(home2, create);
		}
		if (strcmp(command, "rmdir") == 0) {
			rmdir(home2, create);
		}
		if (strcmp(command, "cd") == 0) {
			cd((&home2), create);
		}
		if (strcmp(command, "tree") == 0) {
			tree(home2, -1);
		}
		char* ret;
		if (strcmp(command, "pwd") == 0) {
			ret = pwd(home2);
			printf("%s\n", ret);
			free(ret);
		}
		if (strcmp(command, "stop") == 0) {
			stop(home);
			free(home->name);
			free(home);
		}
		
	} while (strcmp(line, "stop"));
	free(line);
	
	return 0;
}

