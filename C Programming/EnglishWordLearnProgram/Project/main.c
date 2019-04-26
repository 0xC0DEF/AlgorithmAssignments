#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//단어
typedef struct DictionaryItem
{
	char *eng;
	char *kor[3];
	int kor_cnt;
	struct DictionaryItem *prev;
	struct DictionaryItem *next;
} DictionaryItem;

//단어장
typedef struct
{
	int count;
	DictionaryItem *head;
} Dictionary;

//LinkedList
DictionaryItem *NewDictionaryItem();
void DeleteDictionaryItem(DictionaryItem *item);
void ClearDictionary(Dictionary *dic);
void Connect(DictionaryItem *a, DictionaryItem *b);
void Insert(DictionaryItem *target, DictionaryItem *dest);
void Swap(DictionaryItem *a, DictionaryItem *b);
DictionaryItem *GetHead(const DictionaryItem *item);
DictionaryItem *GetTail(const DictionaryItem *item);
DictionaryItem *GetNthNext(const DictionaryItem *head, int n);

//Algorithm
void Sort(Dictionary *dic);
void RandomShuffle(Dictionary *dic);
void Sleep(int t);

//Dictionary management base
int ReadIntegerFromStdin();
char ReadCharacterFromStdin();
void ReadFileNameFromStdinWithForm(char *p);
void WriteDictionaryToFile(char *fname, const Dictionary *dic);
void ReadDictionaryDataFromStdin(Dictionary *dic);
void ReadDictionaryDataFromFile(char *fname, Dictionary *dic);

//Dictionary management
void AddNewDictionary();
void AddNewWord();
void ViewDictionary();
void ViewDictionaryList();

//Program Functions
void EnglishWordJudgement();
void FlashCard();
void HangMan();
void DictionaryManagement();

int main()
{
	srand(time(NULL));
	while (1)
	{
		system("clear");
		printf(">> 영어 단어 암기 프로그램 <<\n");
		printf("%-20s %-20s\n", "1. 영어 단어 맞추기", "2. 플래쉬카드");
		printf("%-20s %-20s\n", "3. 행맨(hangman)", "4. 단어장 관리");
		printf("%-20s\n", "5. 프로그램 종료");

		printf("\n");

		printf("번호를 선택하세요 : ");
		int n = ReadIntegerFromStdin();

		switch (n)
		{
		case 1:
			EnglishWordJudgement();
			break;
		case 2:
			FlashCard();
			break;
		case 3:
			HangMan();
			break;
		case 4:
			DictionaryManagement();
			break;
		case 5:
			system("clear");
			return 0;
		default:
			printf("잘못 입력하셨습니다.\n");
			break;
		}
	}

    return 0;
}

DictionaryItem *NewDictionaryItem()
{
	DictionaryItem *ret = calloc(1, sizeof(DictionaryItem));
	return ret;
}

void DeleteDictionaryItem(DictionaryItem *item)
{
	free(item->eng);
	for (int i = 0; i < item->kor_cnt; i++)
		free(item->kor[i]);
	free(item);
}

void ClearDictionary(Dictionary *dic)
{
	DictionaryItem *cur = dic->head, *next;
	while (dic->count--)
	{
		next = cur->next;
		DeleteDictionaryItem(cur);
		cur = next;
	}
	dic->count = 0;
}

void Connect(DictionaryItem *a, DictionaryItem *b)
{
	if (a)
		a->next = b;
	if (b)
		b->prev = a;
}

void Insert(DictionaryItem *target, DictionaryItem *dest)
{
	Connect(dest->prev, target);
	Connect(target, dest);
}

void Swap(DictionaryItem *a, DictionaryItem *b)
{
	DictionaryItem *ap = a->prev;
	DictionaryItem *an = a->next;
	DictionaryItem *bp = b->prev;
	DictionaryItem *bn = b->next;
	if (a->prev == b)
	{
		Connect(bp, a);
		Connect(a, b);
		Connect(b, an);
	}
	else if (a->next == b)
	{
		Connect(ap, b);
		Connect(b, a);
		Connect(a, bn);
	}
	else
	{
		Connect(b, an);
		Connect(bp, a);
		Connect(ap, b);
		Connect(a, bn);
	}
}

DictionaryItem *GetHead(const DictionaryItem *item)
{
	while (item&&item->prev)
		item = item->prev;
	return (DictionaryItem *)item;
}

DictionaryItem *GetTail(const DictionaryItem *item)
{
	while (item&&item->next)
		item = item->next;
	return (DictionaryItem *)item;
}

DictionaryItem *GetNthNext(const DictionaryItem *head, int n)
{
	for (int i = 0; i < n; i++)
		head = head->next;
	return (DictionaryItem *)head;
}

//삽입정렬
void Sort(Dictionary *dic)
{
	DictionaryItem *cur = dic->head;
	while (cur)
	{
		DictionaryItem *cnext = cur->next;
		DictionaryItem *i = dic->head;
		while (i != cur)
		{
			DictionaryItem *inext = i->next;
			if (strcmp(i->eng, cur->eng) > 0)
			{
				Connect(cur->prev, cur->next);
				Insert(cur, i);
				dic->head = GetHead(dic->head);
				break;
			}
			i = inext;
		}
		cur = cnext;
	}
}

//미완성(버그있을듯)
//바로 앞이나 뒤가 target으로 잡히면 연결리스트 깨지는 버그있음. do-While문으로 땜빵해뒀지만, 다시짜자
void RandomShuffle(Dictionary *dic)
{
	DictionaryItem *cur = dic->head;
	while (cur)
	{
		DictionaryItem *target = GetNthNext(dic->head, rand() % dic->count);
		Swap(cur, target);
		dic->head = GetHead(dic->head);
		cur = target->next;
	}
}

void Sleep(int t)
{
	clock_t s = clock();
	while ((clock() - s) / CLOCKS_PER_SEC < t)
		;
}

int ReadIntegerFromStdin()
{
	int ret;
	scanf("%d", &ret);
	while (getchar() != '\n')
		;
	return ret;
}

char ReadCharacterFromStdin()
{
	char ret;
	scanf("%c", &ret);
	while (getchar() != '\n')
		;
	return ret;
}

void ReadFileNameFromStdinWithForm(char *p)
{
	printf("파일명(일차) : ");
	int day = ReadIntegerFromStdin();
	sprintf(p, "%d.dic", day);
}

void WriteDictionaryToFile(char *fname, const Dictionary *dic)
{
	FILE *fp = fopen(fname, "w+");
	fprintf(fp, "%d\n", dic->count);
	int i;
	DictionaryItem *cur = dic->head;
	while(cur)
	{
		fprintf(fp, "%s ", cur->eng);
		fprintf(fp, "%d", cur->kor_cnt);
		int j;
		for (j = 0; j < cur->kor_cnt; j++)
			fprintf(fp, " %s", cur->kor[j]);
		fprintf(fp, "\n");
		cur = cur->next;
	}
	fclose(fp);
}

void ReadDictionaryDataFromStdin(Dictionary *dic)
{
	DictionaryItem *tail;
	if (dic->head == NULL)
		tail = dic->head = NewDictionaryItem();
	else
	{
		tail = GetTail(dic->head);
		Connect(tail, NewDictionaryItem());
		tail = tail->next;
	}
	while (dic->count < 20)
	{
		char buf[1000];
		int i = 0;
		while ((buf[i] = getchar()) != '\n')
			i++;
		buf[i] = '\0';

		i = 0;
		while (buf[i] != '\0'&&buf[i] == ' ')
			i++;
		tail->eng = calloc(strlen(buf) + 1 - i, sizeof(char));
		int j = 0;
		while (buf[i] != '\0'&&buf[i] != ' ')
			tail->eng[j++] = buf[i++];
		tail->eng[j] = '\0';
		if (!strcmp(tail->eng, ".add"))
			break;

		int k;
		tail->kor_cnt = 0;
		for (k = 0; k < 3; k++)
		{
			while (buf[i] != '\0'&&buf[i] == ' ')
				i++;

			if (buf[i] != '\0'&&buf[i] != ' ')
				tail->kor[k] = calloc(strlen(buf) + 1 - i, sizeof(char));
			j = 0;
			while (buf[i] != '\0'&&buf[i] != ' ')
				tail->kor[k][j++] = buf[i++];
			if (tail->kor[k])
			{
				tail->kor[k][j] = '\0';
				tail->kor_cnt++;
			}
		}
		Connect(tail, NewDictionaryItem());
		tail = tail->next;

		dic->count++;
	}

	if (tail->prev)
		tail->prev->next = NULL;
	DeleteDictionaryItem(tail);
}

void ReadDictionaryDataFromFile(char *fname, Dictionary *dic)
{
	FILE *fp = fopen(fname, "r");
	int n;
	fscanf(fp, "%d", &n);
	dic->count = n;

	DictionaryItem *tail;
	if (dic->head == NULL)
	{
		dic->head = NewDictionaryItem();
		tail = dic->head;
	}
	else
	{
		tail = GetTail(dic->head);
		Connect(tail, NewDictionaryItem());
		tail = tail->next;
	}
	char buf[30 * 3 + 1];
	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%s", buf);
		tail->eng = calloc(strlen(buf) + 1, sizeof(char));
		strcpy(tail->eng, buf);
		fscanf(fp, "%d", &tail->kor_cnt);
		for (int j = 0; j < tail->kor_cnt; j++)
		{
			fscanf(fp, "%s", buf);
			tail->kor[j] = calloc(strlen(buf) + 1, sizeof(char));
			strcpy(tail->kor[j], buf);
		}
		Connect(tail, NewDictionaryItem());
		tail = tail->next;
	}

	if (tail->prev)
		tail->prev->next = NULL;
	DeleteDictionaryItem(tail);
}

void AddNewDictionary()
{
	system("clear");
	printf(">> 영어 단어 암기 프로그램 : 단어장 관리 : 새 파일 추가 <<\n");

	Dictionary dic = { 0, };
	ReadDictionaryDataFromStdin(&dic);

	FILE *fp = fopen("dic.list", "r+");
	int dic_cnt;
	fscanf(fp, "%d", &dic_cnt);
	char next_name[10] = "";
	sprintf(next_name, "%d.dic", dic_cnt + 1);
	fseek(fp, 0, SEEK_END);
	fprintf(fp, "%d.dic\n", dic_cnt + 1);
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "%d", dic_cnt + 1);
	fclose(fp);

	WriteDictionaryToFile(next_name, &dic);
	ClearDictionary(&dic);
}

void AddNewWord()
{
	system("clear");
	printf(">> 영어 단어 암기 프로그램 : 단어장 관리 : 새 단어 추가 <<\n");

	char fname[10];
	ReadFileNameFromStdinWithForm(fname);

	Dictionary dic = { 0, };
	ReadDictionaryDataFromFile(fname, &dic);
	ReadDictionaryDataFromStdin(&dic);
	WriteDictionaryToFile(fname, &dic);
	ClearDictionary(&dic);
}

void ViewDictionary()
{
	system("clear");
	printf(">> 영어 단어 암기 프로그램 : 단어장 관리 : 단어장 보기 <<\n");

	char fname[10];
	ReadFileNameFromStdinWithForm(fname);

	Dictionary dic = { 0, };
	ReadDictionaryDataFromFile(fname, &dic);
	printf("\n---------- 단어장 ----------\n");
	DictionaryItem *cur = dic.head;
	for (int i = 0; i < dic.count; i++)
	{
		printf("%s", cur->eng);
		for (int j = 0; j < cur->kor_cnt; j++)
			printf(" %s", cur->kor[j]);
		printf("\n");
		
		cur = cur->next;
	}
	getchar();

	ClearDictionary(&dic);
}

void ViewDictionaryList()
{
	system("clear");
	printf(">> 영어 단어 암기 프로그램 : 단어장 관리 : 단어장 목록보기 <<\n");

	FILE *fp = fopen("dic.list", "r");
	int dic_cnt;
	fscanf(fp, "%d", &dic_cnt);

	char buf[10];
	printf("\n---------- 단어장 목록 ----------\n");
	for (int i = 1; i <= dic_cnt; i++)
	{
		fscanf(fp, "%s", buf);
		printf("%10s", buf);
		if (i % 5 == 0)
			printf("\n");
	}
	getchar();

	fclose(fp);
}

void EnglishWordJudgement()
{
	char fname[10];
	ReadFileNameFromStdinWithForm(fname);
	printf("출력 방식(알파벳 순서대로 : 1, 무작위 : 2) : ");
	int type = ReadIntegerFromStdin();
	Dictionary dic = { 0, };
	ReadDictionaryDataFromFile(fname, &dic);

	switch (type)
	{
	case 1:
		Sort(&dic);
		break;
	case 2:
		RandomShuffle(&dic);
		break;
	default:
		return;
	}

	int try_cnt = 0;
	int correct_cnt = 0;
	DictionaryItem *cur = dic.head;
	while (1)
	{
		printf("%s", cur->kor[0]);
		for (int i = 1; i<cur->kor_cnt; i++)
			printf(" %s", cur->kor[i]);
		printf(" -> ");
		
		char input[15 + 1];
		scanf("%s", input);
		while (getchar() != '\n')
			;

		if (!strcmp(input, ".quit"))
			break;

		try_cnt++;
		if (!strcmp(input, cur->eng))
		{
			printf("correct!\n");
			correct_cnt++;
		}
		else
		{
			printf("incorrect!\n");
		}

		cur = cur->next ? cur->next : dic.head;
	}
	printf("당신의 점수는 %.2f점 입니다.", 100 * (try_cnt ? correct_cnt / (float)try_cnt : 0));
	getchar();

	ClearDictionary(&dic);
}

void FlashCard()
{
	printf("속도(초) : ");
	int t = ReadIntegerFromStdin();
	char fname[10];
	ReadFileNameFromStdinWithForm(fname);
	printf("출력 방식(알파벳 순서대로 : 1, 무작위 : 2) : ");
	int type = ReadIntegerFromStdin();
	Dictionary dic = {0,};
	ReadDictionaryDataFromFile(fname, &dic);

	switch (type)
	{
	case 1:
		Sort(&dic);
		break;
	case 2:
		RandomShuffle(&dic);
		break;
	default:
		return;
	}

	DictionaryItem *cur = dic.head;
	while (cur)
	{
		system("clear");
		printf("%s\n", cur->eng);
		Sleep(t);
		system("clear");
		printf("%s", cur->kor[0]);
		for(int i=1;i<cur->kor_cnt;i++)
			printf(" %s", cur->kor[i]);
		printf("\n");
		Sleep(t);
		cur = cur->next;
	}

	ClearDictionary(&dic);
}

void HangMan()
{
	char filename[10] = { 0 };//파일명
	ReadFileNameFromStdinWithForm(filename);
	Dictionary dic = { 0, };
	ReadDictionaryDataFromFile(filename, &dic);
	DictionaryItem *word = GetNthNext(dic.head, rand() % dic.count);

	char solution[16] = { 0 };//파일에서 영어단어 1개를 받는다
	strcpy(solution, word->eng);//wordname에 파일의 단어를 1개복사한다
	int length;//i 반복문 n 랜드함수
	length = strlen(solution);//단어의 길이

	char answer[16] = { 0 };
	for (int i = 0; i<length; i++)
		answer[i] = '_';//단어의 길이만큼 _을 출력

	char c;
	int try_cnt = 1;
	int life = 6;
	while (1)
	{
		system("clear");
		printf(">>영어 단어 암기 프로그램 : 행맨 <<\n");

		printf("(힌트)");
		for (int i = 0; i<word->kor_cnt; i++)
			printf(" %s", word->kor[i]);//한글 뜻 출력
		printf("\n");
		printf("\n");		
		switch (life)//life의 크기에따라 행맨 그림 출력
		{
		case 6:
			printf("-----------+");
			printf("\n\n\n\n\n");
			break;
		case 5:
			printf("-----------+\n");
			printf("           O");
			printf("\n\n\n\n");
			break;
		case 4:
			printf("-----------+\n");
			printf("           O\n");
			printf("           |\n");
			printf("\n\n");
			break;
		case 3:
			printf("-----------+\n");
			printf("           O\n");
			printf("          /|\n");
			printf("\n\n");
			break;
		case 2:
			printf("-----------+\n");
			printf("           O\n");
			printf("          /|\\ \n");
			printf("\n\n");
			break;
		case 1:
			printf("-----------+\n");
			printf("           O\n");
			printf("          /|\\ \n");
			printf("          /\n\n");
			break;
		case 0:
			printf("-----------+\n");
			printf("           O\n");
			printf("          /|\\ \n");
			printf("          / \\\n\n");
			break;
		default:
			break;
		}
		if (life == 0)
		{
			printf("%s\n", answer);
			printf("%d번쨰 시도 : %c\n", try_cnt - 1, c);
			printf("########################\n");
			printf("###      Fail...     ###\n");
			printf("########################\n");
			getchar();
			break;
		}
		printf("%s\n", answer);
		printf("%d번쨰 시도 : ", try_cnt);

		c = ReadCharacterFromStdin();
		int found = 0;//입력한 문자가 틀렸는지 맞았는지 체크 맞으면 1 틀리면 0
		for (int i = 0; i<length; i++)//맞았는지 검사 맞으면 check+1
		{
			if (c == solution[i])
			{
				answer[i] = c;
				found++;
			}
		}
		if (!found)
		{
			life--;
		}
		if (strcmp(solution, answer) == 0)
		{
			printf("##########################\n");
			printf("### Congratulations!!! ###\n");
			printf("##########################\n");
			getchar();
			break;
		}
		try_cnt++;
	}

	ClearDictionary(&dic);
}

void DictionaryManagement()
{
	while (1)
	{
		system("clear");
		printf(">> 영어 단어 암기 프로그램 : 단어장 관리 <<\n");
		printf("%-20s %-20s\n", "1. 새 파일 추가하기", "2. 새 단어 추가하기");
		printf("%-20s %-20s\n", "3. 단어장 보기", "4. 단어장 목록보기");
		printf("%-20s\n", "5. 단어장 관리 종료");

		printf("\n");

		printf("번호를 입력하세요 : ");
		int n = ReadIntegerFromStdin();

		switch (n)
		{
		case 1:
			AddNewDictionary();
			break;
		case 2:
			AddNewWord();
			break;
		case 3:
			ViewDictionary();
			break;
		case 4:
			ViewDictionaryList();
			break;
		case 5:
			return;
		default:
			printf("잘못 입력하셨습니다.\n");
			break;
		}
	}
}