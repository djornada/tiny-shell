/*INCLUDE*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*DEFINE*/
#define SECTOR_SIZE	512
#define CLUSTER_SIZE	2*SECTOR_SIZE
#define ENTRY_BY_CLUSTER CLUSTER_SIZE /sizeof(dir_entry_t)
#define NUM_CLUSTER	4096
#define fat_name	"fat.part"

struct _dir_entry_t
{
	unsigned char filename[18];
	unsigned char attributes;
	unsigned char reserved[7];
	unsigned short first_block;
	unsigned int size;
};

typedef struct _dir_entry_t  dir_entry_t;

union _data_cluster
{
	dir_entry_t dir[CLUSTER_SIZE / sizeof(dir_entry_t)];
};

typedef union _data_cluster data_cluster;

/*DATA DECLARATION*/
unsigned short fat[NUM_CLUSTER];
unsigned char boot_block[CLUSTER_SIZE];
dir_entry_t root_dir[32];
data_cluster clusters[4086];

/*Function declaration*/
void append(char* path, char* content);
void create(char* path);
void mkdir (char* path);
void read  (char* path);
void unlink(char* path);
int  find_free_space(dir_entry_t* dir);

void init(void)
{
	FILE* ptr_file;
	int i;
	ptr_file = fopen(fat_name,"wb");
	for (i = 0; i < 2; ++i)
		boot_block[i] = 0xbb;

	fwrite(&boot_block, sizeof(boot_block), 1,ptr_file);

	fat[0] = 0xfffd;
	for (i = 1; i < 9; ++i)
		fat[i] = 0xfffe;

	fat[9] = 0xffff;
	for (i = 10; i < NUM_CLUSTER; ++i)
		fat[i] = 0x0000;

	fwrite(&fat, sizeof(fat), 1, ptr_file);
	fwrite(&root_dir, sizeof(root_dir), 1,ptr_file);

	for (i = 0; i < 4086; ++i)
		fwrite(&clusters, sizeof(data_cluster), 1, ptr_file);

	fclose(ptr_file);
}

void load()
{
	FILE* ptr_file;
	int i;
	ptr_file = fopen(fat_name, "rb");
	fseek(ptr_file, sizeof(boot_block), SEEK_SET);
	fread(fat, sizeof(fat), 1, ptr_file);
	fread(root_dir, sizeof(root_dir), 1, ptr_file);
	fclose(ptr_file);
}

data_cluster* load_cluster(data_cluster cluster)
{
	return 0;
}

data_cluster* find_parent(data_cluster* current_cluster, char* path)
{
	char path_aux[strlen(path)];
	strcpy(path_aux, path);
	char* dir_name = strtok(path_aux, "/");
	char* rest     = strtok(NULL, "\0");
	dir_entry_t* current_dir = current_cluster->dir;

	int len = sizeof(current_dir)/sizeof(dir_entry_t);

	int i;
	for (i = 0; i < len; ++i) {
		dir_entry_t child = current_dir[i];
		if (child.filename == dir_name && rest)
		{
			data_cluster child_cluster = load_cluster()
			/*return find_parent(child, strtok(NULL, "\0"));*/
		}
		/*else if (child->filename == dir_name && !rest){*/
			/*printf("PATH ALREDY EXISTS\n");*/
			/*return NULL;*/
		/*}*/

	}
	/*if (!rest)*/
		/*return current_dir;*/

	/*return NULL;*/
	/*dir = strtok(NULL, "\0");*/
}

char* get_name(char* path)
{

	char name_aux[strlen(path)];
	strcpy(name_aux, path);

	char* name = strtok(name_aux, "/");
	char* rest = strtok(NULL, "\0");
	if (rest != NULL)
		return get_name(rest);

	return (char*) name;
}

int find_free_space(dir_entry_t* dir)
{
	int i;
	for (i = 0; i < 32; ++i){
		if (!dir->attributes)
			return i;
		dir++;
	}
	return -1;
}

void copy_name(char* target, char* src)
{
	int len = strlen(src);
	int i;
	for (i = 0; i < len; ++i) {
		target[i] = src[i];
	}
}

int search_fat_free_block(void)
{
	load();
	int i;
	for (i = 10; i < 4096; ++i)
		if(!fat[i]){
			fat[i] = -1;
			return i;
		}
	return 0;
}

/*void mkdir(char* path)*/
/*{*/
	/*if(path == "/")*/
		/*return;*/

	/*dir_entry_t* dir_parent = find_parent(root_dir, path);*/
	/*if (dir_parent){*/
		/*int free_position = find_free_space(dir_parent);*/
		/*int fat_block = search_fat_free_block();*/
		/*if (fat_block && free_position != -1) {*/
			/*char* dir_name = get_name(path);*/
			/*copy_name(dir_parent[free_position].filename, dir_name);*/
			/*dir_parent[free_position].attributes = 1;*/
			/*dir_parent[free_position].first_block= fat_block;*/
		/*}*/
	/*}*/
	/*else {*/
		/*printf("PATH NOT FOUND\n");*/
	/*}*/
/*}*/

int main(void)
{
	init();
	/*load();*/
	/*char path[] = "/usr";*/
	/*char* dir = strtok(path, "/");*/
	/*printf("%s\n", dir);*/
	/*dir = strtok(NULL, "\0");*/
	/*printf("%d\n", NULL == dir);*/
	/*printf("%s\n", dir);*/
	/*dir = strtok(dir, "/");*/
	/*printf("%s\n", dir);*/
	/*find(root_dir, "/usr");*/
	/*find(root_dir, "/usr/home");*/
	/*printf("get_name(\"%s\") -> %s\n", path ,(char*)get_name(path));*/
	/*printf("get_name(\"%s\") -> %s\n", path2 ,(char*)get_name(path2));*/

	char* path  = "/usr";
	mkdir(path);

	char* path2 = "/usr/bin/7z";
	mkdir(path2);
	mkdir("/");
	return 0;
}