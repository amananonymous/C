#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 100

typedef struct node {
	char title[SIZE];
	double rate;
	struct node* link;
} node;

typedef struct searchList {
	node* prevMovie;
	struct searchList* link;
} searchList;

int menu();
node* add(node**, node*, node*);
node* searchPrev(node*, const char[], node*);
void delete(node*, node**);
void print(node*);
int toFile(node*);

int main(void)
{
	int cmd = 0, counter = 0;
	node* head = NULL, * tail = NULL, * superHead = NULL, * movie = NULL;
	node init = { "", -1.0, head };
	char title[SIZE] = "";

	superHead = &init;

	while (1)
	{
		cmd = menu();

		switch (cmd)
		{
		case 1:
			printf("============================================================\n");
			printf("[1. 영화 추가하기]\n");
			printf("============================================================\n");
			if (!(tail = add(&head, tail, superHead)))
				printf("영화 추가에 실패하였습니다.\n");
			else
			{
				printf("\n성공적으로 영화를 추가하였습니다.\n");
				counter++;
			}
			printf("============================================================\n");
			break;
		case 2:
			printf("============================================================\n");
			printf("[2. 영화 검색하기]\n");
			printf("============================================================\n");
			printf("검색할 영화의 제목을 입력하세요: ");
			scanf("%[^\n]", title);
			getchar();
			if (movie = searchPrev(head, title, superHead))
				print(movie->link);
			printf("============================================================\n");
			break;
		case 3:
			printf("============================================================\n");
			printf("[3. 영화 삭제하기]\n");
			printf("============================================================\n");
			printf("삭제할 영화의 제목을 입력하세요: ");
			scanf("%[^\n]", title);
			getchar();
			if (movie = searchPrev(head, title, superHead))
			{
				strcpy(title, movie->link->title);
				delete(movie, &head);
				printf("\n성공적으로 영화 '%s'을(를) 삭제하였습니다.\n", title);
				counter--;
			}
			else
				printf("\a영화 삭제에 실패하였습니다. 해당하는 영화가 존재하지 않았을 수 있습니다.\n");
			printf("============================================================\n");
			break;
		case 4:
			printf("============================================================\n");
			printf("[4. 저장된 영화 개수 확인하기]\n");
			printf("현재 저장되어 있는 영화의 개수는 %d개입니다.\n", counter);
			printf("============================================================\n");
			break;
		case 5:
			printf("============================================================\n");
			printf("[5. 현재 영화 목록을 파일에 저장하기]\n");
			if (!counter)
				printf("저장되어 있는 영화가 없습니다.\n");
			else
			{
				printf("============================================================\n");

				if (!toFile(head))
					printf("\n총 %d개의 영화를 파일에 저장하였습니다.\n", counter);
				else
					printf("파일 저장에 실패하였습니다.\n");
			}
			printf("============================================================\n");
			break;
		case 6:
			printf("============================================================\n");
			printf("[프로그램을 종료합니다]\n");
			printf("============================================================\n\n");
			return 0;
		default:
			printf("\a잘못된 작업입니다.\n");
		}

		putchar('\n');
	}

	return 0;
}

int menu()
{
	int cmd = 0;

	printf("============================================================\n");
	printf("1. 영화 추가하기\n2. 영화 검색하기\n3. 영화 삭제하기\n4. 저장된 영화 개수 확인하기\n5. 현재 영화 목록을 파일에 저장하기\n6. 종료하기\n");
	printf("============================================================\n");
	printf("수행할 작업 선택하기: ");
	scanf("%d", &cmd);
	printf("============================================================\n\n");
	getchar();

	return cmd;
}

node* add(node** head, node* tail, node* superHead)
{
	node* movie = NULL;
	char title[SIZE] = "";
	double rate = -1.0;

	printf("추가할 영화의 제목을 입력하세요: ");
	scanf("%[^\n]", title);

	while (1)
	{
		printf("추가할 영화의 평점을 입력하세요: ");
		scanf("%lf", &rate);

		if (rate < 0.0 || rate > 10.0)
			printf("\a평점은 0.0 ~ 10.0 사이의 값만 입력할 수 있습니다.\n");
		else break;
	}

	if (!(movie = (node*)malloc(sizeof(node))))
	{
		fprintf(stderr, "\a영화 추가 과정에서 동적 메모리 할당 오류 발생\n");
		return NULL;
	}

	strcpy(movie->title, title);
	movie->rate = rate;
	movie->link = NULL;

	if (!*head)
	{
		*head = movie;
		superHead->link = *head;
	}
	else
		tail->link = movie;

	return movie;
}

node* searchPrev(node* head, const char title[], node* superHead)
{
	searchList* list = NULL, * listHead = NULL, * listPrev = NULL, * tmp = NULL;
	node* prev = superHead, * value = NULL;
	int counter = 0, cmd = 0;

	while (head)
	{
		if (strstr(head->title, title))
		{
			if (!(list = (searchList*)malloc(sizeof(searchList))))
			{
				fprintf(stderr, "\a영화 검색 과정에서 동적 메모리 할당 오류 발생\n");
				return NULL;
			}

			list->prevMovie = prev;
			list->link = NULL;

			if (!listHead)
				listHead = list;
			else
				listPrev->link = list;

			listPrev = list;

			counter++;
		}

		prev = head;
		head = head->link;
	}

	printf("\n총 %d개의 영화를 찾았습니다.\n", counter);
	if (!counter) return NULL;

	list = listHead;
	for (int i = 0; i < counter; i++)
	{
		printf("%d. %s\n", i + 1, list->prevMovie->link->title);
		list = list->link;
	}

	printf("\n찾고자 한 영화에 해당하는 번호를 입력하세요: ");
	scanf("%d", &cmd);

	while (cmd < 1 || cmd > counter)
	{
		printf("잘못된 번호입니다. 다시 입력하세요: ");
		scanf("%d", &cmd);
	}

	getchar();

	for (int i = 0; i < counter; i++)
	{
		if (i + 1 == cmd)
			value = listHead->prevMovie;

		tmp = listHead->link;
		free(listHead);
		listHead = tmp;
	}

	return value;
}

void delete(node* prev, node** head)
{
	node* movie = prev->link;
	
	if (movie == *head)
		*head = (*head)->link;

	prev->link = movie->link;
	free(movie);
}

void print(node* movie)
{
	printf("------------------------------------------------------------\n");
	printf("영화 제목: %s\n", movie->title);
	printf("영화 평점: %.2lf\n", movie->rate);
	printf("------------------------------------------------------------\n");
}

int toFile(node* head)
{
	FILE* fp = NULL;
	char filename[SIZE] = "";

	printf("저장할 파일 이름을 입력하세요: ");
	scanf("%s", filename);

	if (!(fp = fopen(filename, "w")))
	{
		fprintf(stderr, "\a'%s' 파일을 열 수 없습니다.\n", filename);
		return -1;
	}

	while (head)
	{
		fputs("------------------------------------------------------------\n", fp);
		fprintf(fp, "영화 제목: %s\n", head->title);
		fprintf(fp, "영화 평점: %.2lf\n", head->rate);
		fputs("------------------------------------------------------------\n", fp);

		head = head->link;
	}

	fclose(fp);
	return 0;
}
