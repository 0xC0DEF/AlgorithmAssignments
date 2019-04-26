#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>

//수식의 최대 입력 길이
#define INPUT_LEN_MAX 1000
//자리 수 최대값
#define NUMBER_LEN_MAX 50

//오류 처리용 반환 값들
#define SUCCESS 1
#define ERROR 0
#define OVERFLOW -1
#define DIV_BY_ZERO -2

//수식 분석을 위한 토큰 (연결리스트)
struct Token
{
	char variable; //변수 문자값
	char op; //연산자 문자값
	char n[NUMBER_LEN_MAX]; //절댓값
	int len; //저장된 절댓값의 길이
	int sign; //부호
	struct Token *prev; //이전 토큰
	struct Token *next; //다음 토큰
};
//연결리스트 관련 함수들
struct Token* NewToken();
struct Token *NewNumber(int n);
struct Token *NewVariable(char c);
void DeleteToken(struct Token *token);
void DeleteTokensBetween(struct Token *from, struct Token *to);
void DeleteTokenAll(struct Token *token);
void Connect(struct Token *prev, struct Token *next);
void Reconnect(struct Token *prev, struct Token *next);

//토큰 관련 함수들
void RecalculateLen(struct Token *token);
void CopyData(struct Token *to, struct Token *from);
void SwapData(struct Token *a, struct Token *b);
int Compare(struct Token *a, struct Token *b);
int CompareAbs(struct Token *a, struct Token *b);

//숫자토큰 관련 함수들
_Bool IsDigit(char c);
_Bool IsDigitToken(struct Token *token);
int PushDigit(struct Token *token, char c);

//연산자토큰 관련 함수들
_Bool IsOperator(char c);
_Bool IsOperatorToken(struct Token *token);
void SetOperator(struct Token *token, char c);
int GetOperatorPriority(char op);

//변수토큰 관련 함수들
_Bool IsVariable(char c);
_Bool IsVariableToken(struct Token *token);
void SetVariable(struct Token *token, char c);
void SetGlobalVariableTokenData(char c, struct Token *token);
_Bool IsGlobalVariableInitialized(char c);
struct Token* NewVariableTokenFromGlobalVariable(char c);

//계산 함수들
int Add(struct Token *a, struct Token *b);
int Sub(struct Token *a, struct Token *b);
int Mul(struct Token *a, struct Token *b);
int Div(struct Token *a, struct Token *b);
int Mod(struct Token *a, struct Token *b);
int Assign(struct Token *a, struct Token *b);

//계산처리 함수들
struct Token* Parse(char *str);
int GetOperatorCount(struct Token *head);
void SortOperatorsDesc(struct Token **ops, int count);
_Bool HandleUnaryOperators(struct Token *token);
_Bool Calculate(struct Token *token);

//입출력 관련 함수들
_Bool GetLine(char str[INPUT_LEN_MAX]);
void RemoveSpaces(char str[INPUT_LEN_MAX], int start_idx);
void Print(struct Token *token);
void PrintVariables();

int Max(int a, int b);

int main()
{
	printf("Start....\n");
	char str[INPUT_LEN_MAX];
	while (GetLine(str) != SUCCESS)
		printf("ERROR : Too Long Input (%d characters limit)\n", INPUT_LEN_MAX - 1);
	RemoveSpaces(str, 0);
	while (strcmp(str, "end") != 0)
	{
		if (strcmp(str, "") == 0)
			;
		else if (strcmp(str, "VAR") == 0)
		{
			PrintVariables();
		}
		else if (strcmp(str, "clear") == 0)
		{
			system("clear");
		}
		else
		{
			struct Token *head = Parse(str);
			if (head == NULL)
				Print(NULL);
			else
			{
				//단항 연산자부터 처리한다 (제일 높은 우선순위)
				if (HandleUnaryOperators(head))
				{
					//연결리스트로 파싱된 수식을 계산한다
					if (Calculate(head)==SUCCESS)
						Print(head->next);
					else
						Print(NULL);
				}
				else
					Print(NULL);
			}
			//Parse()로 생성된 연결리스트 메모리를 전부 해제한다
			DeleteTokenAll(head);
		}
		while (GetLine(str) != SUCCESS)
			printf("ERROR : Too Long Input (%d characters limit)\n", INPUT_LEN_MAX - 1);
		RemoveSpaces(str, 0);
	}
	return 0;
}

//두 정수 중 큰 값을 반환한다
int Max(int a, int b)
{
	return a > b ? a : b;
}

//새로운 토큰 생성
struct Token* NewToken()
{
	struct Token *token = malloc(sizeof(struct Token));
	memset(token, 0, sizeof(struct Token));
	int i;
	for (i = 0; i < NUMBER_LEN_MAX; i++)
		token->n[i] = 0;
	return token;
}

//n값을 갖는 새로운 토큰 생성
struct Token *NewNumber(int n)
{
	struct Token *token = NewToken();
	token->sign = (n >= 0) * 2 - 1;

	int arr[10] = { 0, };
	int i = 0;
	while (n)
	{
		arr[i++] = n % 10;
		n /= 10;
	}
	for (i = 9; i >= 0; i--)
		PushDigit(token, arr[i] + '0');
	return token;
}

//새로운 일회용 변수토큰 생성
struct Token* NewVariable(char c)
{
	struct Token *token = NewNumber(0);
	SetVariable(token, c);
	return token;
}

//토큰을 삭제한다
void DeleteToken(struct Token *token)
{
	free(token);
}

//from 토큰과 to 토큰 사이에 있는 토큰들을 삭제한다
void DeleteTokensBetween(struct Token *from, struct Token *to)
{
	from = from->next;
	while (from != to)
	{
		struct Token *next = from->next;
		DeleteToken(from);
		from = next;
	}
}

//token과 연결되있는 모든 토큰을 지운다.
void DeleteTokenAll(struct Token *token)
{
	if (token == NULL)
		return;
	struct Token *p, *n;
	p = token->prev;
	n = token->next;
	if (p)
		p->next = NULL;
	if (n)
		n->prev = NULL;
	DeleteToken(token);
	DeleteTokenAll(p);
	DeleteTokenAll(n);
}

//prev 토큰과 next 토큰을 연결해준다
void Connect(struct Token *prev, struct Token *next)
{
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
}

//prev토큰과 next토큰을 다시 연결한다
void Reconnect(struct Token *prev, struct Token *next)
{
	if (prev == NULL)
		return;

	//next토큰이 prev토큰의 오른쪽에 존재하지 않으면 오류이므로 함수를 종료한다.
	struct Token *cur = prev;
	while (cur != next)
	{
		if (cur == NULL)
			return;
		cur = cur->next;
	}

	//prev토큰과 next토큰 사이의 토큰들을 지운다
	DeleteTokensBetween(prev, next);
	//prev토큰과 next토큰을 연결한다
	Connect(prev, next);
}

//token에 저장된 숫자의 길이을 다시 계산한다
void RecalculateLen(struct Token * token)
{
	token->len = NUMBER_LEN_MAX;
	while (token->len > 0 && token->n[token->len - 1] == 0)
		token->len--;
}

//to토큰의 데이터들을 from토큰으로 복사해준다
void CopyData(struct Token *to, struct Token *from)
{
	to->len = from->len;
	memcpy(to->n, from->n, sizeof(char)*NUMBER_LEN_MAX);
	to->op = from->op;
	to->sign = from->sign;
	to->variable = from->variable;
}

//a와 b의 데이터를 바꿔준다
void SwapData(struct Token *a, struct Token *b)
{
	struct Token *token = NewToken();
	CopyData(token, a);
	CopyData(a, b);
	CopyData(b, token);
	DeleteToken(token);
}

//두 토큰의 값을 비교해서 a가 크면 -1, b가 크면 1, 같으면 0을 반환한다
int Compare(struct Token *a, struct Token *b)
{
	if (a->sign > b->sign)
		return -1;
	if (a->sign < b->sign)
		return 1;
	if (a->len > b->len)
		return -1;
	if (a->len < b->len)
		return 1;

	//Now, a->len==b->len && a->sign==b->sign
	int sign = a->sign;
	int len = a->len;

	int i;
	for (i = len - 1; i >= 0; i--)
	{
		if (a->n[i] > b->n[i])
			return -1 * sign;
		if (a->n[i] < b->n[i])
			return 1 * sign;
	}
	return 0;
}

//두 토큰의 절대값을 비교해서 반환한다
int CompareAbs(struct Token *a, struct Token *b)
{
	int tmp_a = a->sign;
	int tmp_b = b->sign;
	a->sign = b->sign = 1;
	int c = Compare(a, b);
	a->sign = tmp_a;
	b->sign = tmp_b;
	return c;
}

//이 문자가 숫자를 나타내는지 여부를 반환한다
_Bool IsDigit(char c)
{
	return '0' <= c&&c <= '9';
}

//이 토큰이 숫자토큰인지를 반환한다
_Bool IsDigitToken(struct Token *token)
{
	return token && token->op == '\0' && token->sign != 0;
}

//token에 숫자한자리(c-'0')를 추가해준다
int PushDigit(struct Token *token, char c)
{
	if (token->len == NUMBER_LEN_MAX)
		return OVERFLOW;
	int i;
	for (i = token->len; i > 0; i--)
		token->n[i] = token->n[i - 1];
	token->n[0] = c - '0';
	RecalculateLen(token);
	return SUCCESS;
}

//이 문자가 연산자를 나타내는지 여부를 반환한다
_Bool IsOperator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '%';
}

//이 토큰이 연산자를 나타내는지 여부를 반환한다
_Bool IsOperatorToken(struct Token *token)
{
	return token && token->op != '\0'&&token->sign == 0 && token->variable == '\0';
}

//연산자 우선순위를 반환한다 (이항연산자 전용)
int GetOperatorPriority(char op)
{
	switch (op)
	{
	case '=':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
	case '%':
		return 2;
	}
	return -1;
}

//토큰에 연산자데이터를 저장해준다
void SetOperator(struct Token *token, char c)
{
	token->op = c;
}

//이 문자가 변수를 나타내는지 여부를 반환한다
_Bool IsVariable(char c)
{
	return 'A' <= c&&c <= 'Z';
}

//이 토큰이 변수토큰인지 여부를 반환한다
_Bool IsVariableToken(struct Token *token)
{
	return IsDigitToken(token) && token->variable != '\0';
}

//토큰에 변수문자값을 설정해준다 
void SetVariable(struct Token * token, char c)
{
	token->variable = c;
}

/*
변수토큰을 저장하는 전역변수
변수토큰은 일회용(Parse용)변수토큰과 저장용(전역변수)변수토큰으로 구분된다.
X=X+1과 같은 식을 처리하기 위해
Parse()에서 일회용 변수토큰을 생성하고,
Assign()에서 변수값을 전역변수 변수토큰에 저장하는 방식이다.
*/
struct Token *variables[26];

//전역변수 변수토큰에 데이터를 설정한다
void SetGlobalVariableTokenData(char c, struct Token * token)
{
	if (IsGlobalVariableInitialized(c))
		DeleteToken(variables[c - 'A']);
	variables[c - 'A'] = NewToken();
	CopyData(variables[c - 'A'], token);
	SetVariable(variables[c - 'A'], c);
}

//전역변수 변수토큰에 값이 할당되어있는지 여부를 반환한다
_Bool IsGlobalVariableInitialized(char c)
{
	return variables[c - 'A'] != NULL;
}

//전역변수 변수토큰과 같은 값의 변수토큰(일회용)을 생성해서 반환한다
struct Token* NewVariableTokenFromGlobalVariable(char c)
{
	if (!IsGlobalVariableInitialized(c))
		return NewToken();
	struct Token *result = NewToken();
	CopyData(result, variables[c - 'A']);
	return result;
}

//a토큰과 b토큰을 더한값을 a에 저장한다
int Add(struct Token *a, struct Token *b)
{
	//부호가 다른경우 Sub 기능으로 대체한다
	if (a->sign != b->sign)
	{
		b->sign *= -1;
		return Sub(a, b);
	}

	int i;
	for (i = 0; i < Max(a->len, b->len); i++)
	{
		int n = a->n[i] + b->n[i];
		//n[i]에 그 자리수 값을 저장한다
		a->n[i] = (n % 10);
		if (n / 10 && i + 1 == NUMBER_LEN_MAX)
			return OVERFLOW;
		//n[i+1]에 올림된 값을 더한다
		a->n[i + 1] += n / 10;
	}
	//a가 변수토큰이었다면 이제 변수토큰이 아니므로 변수토큰설정값을 지워준다
	a->variable = '\0';
	//a의 길이가 달라졌으므로 길이를 다시 계산해준다
	RecalculateLen(a);
	return SUCCESS;
}

//a에서 b토큰을 뺀 값을 a에 저장한다
int Sub(struct Token *a, struct Token *b)
{
	//부호가 다른경우 Add 기능으로 대체한다
	if (a->sign != b->sign)
	{
		b->sign *= -1;
		return Add(a, b);
	}

	/*
	a와 b의 절댓값을 비교해서 절댓값이 더 큰 수를 a로 만든다.
	이렇게 함으로써 항상 위에서 내림(10을 가져오기)할 수 있다.
	example) a-b에서 a가 b보다 작으면, 위에서 내림할 수 없는 경우가 발생한다.
	*/
	int c_abs = CompareAbs(a, b);
	if (c_abs > 0)
	{
		SwapData(a, b);
		a->sign *= -1;
	}

	int i;
	for (i = 0; i < Max(a->len, b->len); i++)
	{
		//n[i]에 그 자리수 값을 저장한다
		a->n[i] = a->n[i] - b->n[i];
		//n[i]가 음수인 경우에는 n[i+1]에서 1을 빼고 n[i]에 10을 더한다
		if (a->n[i] < 0)
		{
			a->n[i + 1]--;
			a->n[i] += 10;
		}
	}
	//a가 변수토큰이었다면 이제 변수토큰이 아니므로 변수토큰설정값을 지워준다
	a->variable = '\0';
	//a의 길이가 달라졌으므로 길이를 다시 계산해준다
	RecalculateLen(a);
	return SUCCESS;
}

//a와 b토큰을 곱한 값을 a에 저장한다
int Mul(struct Token *a, struct Token *b)
{
	//곱하는 도중에 a값이 변하면 안되므로 중간값들을 저장할 result변수 생성
	struct Token *result = NewNumber(0);
	int i, j;
	for (i = 0; i < a->len; i++)
	{
		for (j = 0; j < b->len; j++)
		{
			//두 수의 곱이 50자리를 넘어가면 오버플로우 처리
			if (i + j == NUMBER_LEN_MAX)
				return OVERFLOW;
			int n = result->n[i + j] + a->n[i] * b->n[j];
			result->n[i + j] = n % 10;
			//정확히 50자리수 계산에서 올림이 발생한 경우 오버플로우 처리
			if (n / 10 && i + j + 1 == NUMBER_LEN_MAX)
				return OVERFLOW;
			result->n[i + j + 1] += n / 10;
		}
	}
	//곱한 값의 부호는, 부호끼리 곱하면 된다
	result->sign = a->sign*b->sign;
	//a가 변수토큰이었다면 이제 변수토큰이 아니므로 변수토큰설정값을 지워준다
	result->variable = '\0';
	//a의 길이가 달라졌으므로 길이를 다시 계산해준다
	RecalculateLen(result);
	//계산결과를 a로 옮겨담는다
	CopyData(a, result);
	//임시로 생성했던 토큰삭제
	DeleteToken(result);
	return SUCCESS;
}

//a를 b토큰으로 나눈 값을 a에 저장한다
int Div(struct Token *a, struct Token *b)
{
	//0으로 나누는 경우 오류처리
	if (b->len == 0)
		return DIV_BY_ZERO;
	/*
	a와 b에 절대값을 취하고, 각각의 부호를 잠시 저장해둔다
	절대값을 취하는 이유는 설계한 알고리즘이 양수에 대해서만 동작하기 때문에
	절대값으로 계산한 뒤 부호를 결정해주는 것이다
	*/
	int sign_a = a->sign;
	int sign_b = b->sign;
	a->sign = b->sign = 1;

	//계산도중 a값이 바뀌면 안되므로 result변수를 따로 생성한다
	struct Token *result = NewNumber(0);
	//2값을 갖는 숫자토큰 생성(2를 곱해주는 용도)
	struct Token *_2 = NewNumber(2);
	//뺄셈할 크기
	struct Token *sub_size = NewToken();
	do
	{
		//뺄셈할 크기는 b값(=(2^0)*b)으로 시작한다
		CopyData(sub_size, b);
		//'2^n'을 의미한다
		struct Token *multiplier = NewNumber(1);
		//a-(2^n)*b가 가능할동안
		while (Compare(a, sub_size) <= 0)
		{
			//a -= (2^n)*b
			Sub(a, sub_size);
			//result += 2^n
			Add(result, multiplier);
			//아래 두줄의 의미 : n += 1
			Mul(multiplier, _2);
			Mul(sub_size, _2);
		}
		//multiplier 사용이 끝났으므로 삭제
		DeleteToken(multiplier);
		//a에서 b조차 뺄 수 없을 때 종료한다
	} while (0 < Compare(b, sub_size));
	//몫의 부호는, 부호끼리 곱하면 된다
	result->sign = sign_a*sign_b;
	//a가 변수토큰이었다면 이제 변수토큰이 아니므로 변수토큰설정값을 지워준다
	result->variable = '\0';
	//a의 길이가 달라졌으므로 길이를 다시 계산해준다
	RecalculateLen(result);
	//계산결과를 a로 옮겨담는다
	CopyData(a, result);
	//임시로 생성했던 토큰들 삭제
	DeleteToken(result);
	DeleteToken(_2);
	DeleteToken(sub_size);
	return SUCCESS;
}

/*
a를 b토큰으로 나눈 나머지 값을 a에 저장한다
Div()함수에서 result와 mul_count코드를 빼고, result대신 a를 사용한다.
부호는 a의 부호를 그대로 사용한다(NCloud Cygwin 기준)
그 외에는 Div()와 코드가 완벽하게 동일하다.
∵알고리즘이 유사하기 때문
*/
int Mod(struct Token * a, struct Token * b)
{
	if (b->len == 0)
		return DIV_BY_ZERO;
	int sign_a = a->sign;
	int sign_b = b->sign;
	a->sign = b->sign = 1;

	struct Token *_2 = NewNumber(2);
	struct Token *sub_size = NewToken();
	do
	{
		CopyData(sub_size, b);
		while (Compare(a, sub_size) <= 0)
		{
			Sub(a, sub_size);
			Mul(sub_size, _2);
		}
	} while (0 < Compare(b, sub_size));
	a->sign = sign_a;
	a->variable = '\0';
	RecalculateLen(a);
	DeleteToken(_2);
	DeleteToken(sub_size);
	return SUCCESS;
}

//a에 해당하는 전역변수 변수토큰에 b값을 저장하고, b값을 a에 저장한다
int Assign(struct Token *a, struct Token *b)
{
	SetGlobalVariableTokenData(a->variable, b);
	CopyData(a, b);
	return SUCCESS;
}

//한 줄을 입력받는다
_Bool GetLine(char str[INPUT_LEN_MAX])
{
	int i = 0;
	int c;
	printf("(input) ");
	//엔터가 나오기 전까지 입력을 받는다
	while ((c = getchar()) != '\n')
	{
		//INPUT_LEN_MAX-1이 입력버퍼의 마지막칸인데, 그곳에는 널문자가 들어가야 하므로 오버플로이다
		if (i == INPUT_LEN_MAX - 1)
		{
			//에러 발생시 입력버퍼 비우기
			while (getchar() != '\n')
				;
			return ERROR;
		}
		str[i++] = c;
	}
	str[i] = '\0';
	return SUCCESS;
}

//start_idx부터 공백이 아닐때까지 공백을 지운다.
void RemoveSpaces(char str[INPUT_LEN_MAX], int start_idx)
{
	int count = 0;
	//공백 개수 계산
	while (str[start_idx + count] == ' ')
		count++;
	int i;
	//공백 개수만큼 당겨오기
	for (i = start_idx; i < INPUT_LEN_MAX - count; i++)
		str[i] = str[count + i];
}

//수식파싱 및 연결리스트 생성
struct Token* Parse(char *str)
{
	//시작 노드
	struct Token *head = NewToken();

	int i = 0;
	struct Token *cur = head;
	while (1)
	{
		RemoveSpaces(str, i);
		//변수는 대소문자 구분하지 않으므로 소문자를 모두 대문자로 변환한다
		if ('a' <= str[i] && str[i] <= 'z')
			str[i] = str[i] - 'a' + 'A';
		//변수인 경우
		if (IsVariable(str[i]))
		{
			struct Token *token;
			//전역변수 변수토큰이 존재한다면
			if (IsGlobalVariableInitialized(str[i]))
				//전역변수 변수토큰으로부터 새로운 일회용 변수토큰을 생성한다
				token = NewVariableTokenFromGlobalVariable(str[i]);
			//아니라면
			else
				//그냥 일회용 변수토큰을 생성한다
				token = NewVariable(str[i]);
			Connect(cur, token);
			//변수 앞에는 연산자만 올 수 있다
			if (IsDigitToken(cur))
				return NULL;
			cur = cur->next;
			i++;
		}
		//숫자인 경우
		else if (IsDigit(str[i]))
		{
			//새로운 숫자토큰을 생성하고 cur에 연결한다
			Connect(cur, NewNumber(0));
			while (IsDigit(str[i]))
				if (PushDigit(cur->next, str[i++]) == OVERFLOW)
					return NULL;
			//숫자 앞에는 연산자만 올 수 있다
			if (IsDigitToken(cur))
				return NULL;
			cur = cur->next;
			i;
		}
		//연산자인 경우
		else if (IsOperator(str[i]))
		{
			//새로운 토큰을 생성하고 cur에 연결한다
			Connect(cur, NewToken());
			SetOperator(cur->next, str[i]);
			if (cur->next->op == '=')
				//대입연산자의 왼쪽에는 변수만 올 수 있다
				if (!IsVariableToken(cur))
					return NULL;
			cur = cur->next;
			i++;
		}
		//수식의 끝
		else if (str[i] == '\0')
			break;
		//이 외의 경우들은 오류
		else
			return NULL;
	}
	return head;
}

//연결리스트 내의 연산자토큰 개수를 반환한다
int GetOperatorCount(struct Token *head)
{
	int op_cnt = 0;
	struct Token *cur = head;
	while (cur)
	{
		if (IsOperatorToken(cur))
			op_cnt++;
		cur = cur->next;
	}
	return op_cnt;
}

//연산자토큰 배열을 연산자우선순위를 기준으로 내림차순 정렬한다
void SortOperatorsDesc(struct Token **ops, int count)
{
	int i, j;
	for (i = 0; i < count; i++)
	{
		for (j = 0; j < count - i - 1; j++)
		{
			if (GetOperatorPriority(ops[j]->op) < GetOperatorPriority(ops[j + 1]->op))
			{
				struct Token *tmp = ops[j];
				ops[j] = ops[j + 1];
				ops[j + 1] = tmp;
			}
		}
	}

	//대입연산자는 연산순서가 ←이므로 역순으로 배치한다
	for (i = 0; i < count; i++)
	{
		if (ops[i]->op == '=')
		{
			for (j = i; j < (count + i) / 2; j++)
			{
				struct Token *tmp = ops[j];
				ops[j] = ops[count - j + i - 1];
				ops[count - j + i - 1] = tmp;
			}
			break;
		}
	}
}

//단항연산자 처리
_Bool HandleUnaryOperators(struct Token *token)
{
	while (token)
	{
		while (IsDigitToken(token))
			token = token->next;
		if (token)
			//현재 연산자가 -이면서 앞의 토큰이 숫자토큰이 아니고 뒤의 토큰이 숫자토큰이면 단항-연산자이다
			if (token->op == '-' && token->prev && !IsDigitToken(token->prev) && IsDigitToken(token->next))
			{
				struct Token *p = token->prev;
				struct Token *n = token->next;
				Reconnect(p, n);
				n->sign *= -1;
				token = n;
			}
			//수식이 연산자로 끝날 수 없다
			else if (token->next == NULL)
				return ERROR;
			else
				token = token->next;
	}
	return SUCCESS;
}

//연결리스트로 구성된 수식을 계산한다
//최종적으로는 head노드 하나와 계산된 값 하나, 이렇게 2개의 노드만 남게 된다
_Bool Calculate(struct Token *token)
{
	int op_cnt = GetOperatorCount(token);
	struct Token **ops = malloc(sizeof(struct Token *)*op_cnt);

	//연산자토큰 배열을 생성한다
	struct Token *cur = token;
	int idx = 0;
	while (cur)
	{
		if (IsOperatorToken(cur))
			ops[idx++] = cur;
		cur = cur->next;
	}

	SortOperatorsDesc(ops, op_cnt);

	int i;
	for (i = 0; i<op_cnt; i++)
	{
		token = ops[i];
		//연산자의 양 옆이 숫자가 아닐 수 없다
		//∵단항연산자는 Calculate()전에 제거된다
		if (!IsDigitToken(token->prev) || !IsDigitToken(token->next))
			return ERROR;
		//대입연산자의 왼쪽이외에는 정의되지 않은 변수를 사용할 수 없다
		if (token->op != '='&&IsVariableToken(token->prev) && !IsGlobalVariableInitialized(token->prev->variable))
			return ERROR;
		//대입연산자도 포함해서 오른쪽에는 정의되지 않은 변수를 사용할 수 없다
		if (IsVariableToken(token->next) && !IsGlobalVariableInitialized(token->next->variable))
			return ERROR;
		//대입연산자의 왼쪽에는 변수만 와야 한다
		if (token->op == '=' && !IsVariableToken(token->prev))
			return ERROR;

		switch (token->op)
		{
		case '+':
			if (Add(token->prev, token->next) != SUCCESS)
				return ERROR;
			Reconnect(token->prev, token->next->next);
			break;
		case '-':
			if (Sub(token->prev, token->next) != SUCCESS)
				return ERROR;
			Reconnect(token->prev, token->next->next);
			break;
		case '*':
			if (Mul(token->prev, token->next) != SUCCESS)
				return ERROR;
			Reconnect(token->prev, token->next->next);
			break;
		case '/':
			if (Div(token->prev, token->next) != SUCCESS)
				return ERROR;
			Reconnect(token->prev, token->next->next);
			break;
		case '%':
			if (Mod(token->prev, token->next) != SUCCESS)
				return ERROR;
			Reconnect(token->prev, token->next->next);
			break;
		case '=':
			if (Assign(token->prev, token->next) != SUCCESS)
				return ERROR;
			Reconnect(token->prev, token->next->next);
			break;
		}
	}
	return SUCCESS;
}

//토큰의 숫자값을 출력한다
//토큰이 NULL인 경우 error을 출력한다
//토큰이 정의되지 않은 변수인 경우 undefined.를 출력한다
void Print(struct Token *token)
{
	printf("= ");
	//정의되지 않은 변수인 경우
	if (IsVariableToken(token) && !IsGlobalVariableInitialized(token->variable))
	{
		printf("undefined.\n");
		return;
	}
	//NULL인 경우
	if (token == NULL)
	{
		printf("error\n");
		return;
	}
	//음수인 경우 -를 붙여준다
	//이 때, 0은 부호가 -1이더라도 -를 붙이지 않아야 한다
	if (token->len > 0 && token->sign == -1)
		printf("-");
	//숫자가 역순으로 저장되있으므로 루프를 거꾸로 돌려서 출력한다
	int i = token->len - 1;
	while (i >= 0)
	{
		//세 자리수마다 콤마를 붙여준다
		if (i != token->len - 1 && i % 3 == 2)
			printf(",");
		printf("%d", token->n[i--]);
	}
	//0인경우 루프가 하나도 돌지 않으므로, 따로 처리해준다
	if (token->len == 0)
		printf("0");
	printf("\n");
}

//정의되어있는 모든 변수와 그 값을 출력한다
void PrintVariables()
{
	int i;
	for (i = 'A'; i <= 'Z'; i++)
	{
		if (IsGlobalVariableInitialized(i))
		{
			printf("%c ", i);
			struct Token *token = NewVariableTokenFromGlobalVariable(i);
			Print(token);
			DeleteToken(token);
		}
	}
}