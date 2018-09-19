#include <iostream>
#include <assert.h>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

//구문 단위(Operator, Operand)
struct Token
{
	//operator일때 true, operand일때 false
	bool op;
	int data;

	Token(bool op, int data) :op(op), data(data) {}

	bool operator== (const Token &r)const
	{
		return op == r.op && data == r.data;
	}

	bool operator!= (const Token &r)const
	{
		return !(*this == r);
	}

	//연산자 우선순위
	//간결화를 위해 한 연산자인데 두개의 문자를 갖는 괄호쌍()은 우선순위없이 따로 재귀처리하였다
	int priority() const
	{
		if(!op)
			throw "Error";
		switch (data)
		{
		case'+':
		case'-':
			return 1;
		case'*':
		case'/':
		case'%':
			return 2;
		case'_'://unary minus
			return 3;
		case'^':
			return 4;
		}
		throw "Error";
	}

	//연관방향 0=left, 1=right
	int association()
	{
		if (!op)
			throw "Error";
		if (data == '_' || data == '^')//unary minus도 우측연관이라고 봐야함.
			return 1;
		return 0;
	}
};

bool is_digit(char c)
{
	return '0' <= c && c <= '9';
}

bool is_op(char c)
{
	vector<char> s = { '+', '-', '*', '/', '%', '_', '^', '(', ')' };
	return find(s.begin(), s.end(), c) != s.end();
}

//문자열을 토큰화 시킨다. 이 때, 단항마이너스는 _로 바꾼다.
vector<Token> parse(char *buf)
{
	vector<Token> ret;

	int i;
	for (i = 0; is_digit(buf[i]) || is_op(buf[i]) || buf[i] == ' ';)
	{
		while (buf[i] == ' ')
			i++;
		bool flag = false;
		int n = 0;
		while (is_digit(buf[i]))
		{
			n *= 10;
			n += buf[i++] - '0';
			flag = true;
		}
		if (flag)
			ret.push_back(Token(false, n));

		while (buf[i] == ' ')
			i++;
		if (is_op(buf[i]))
			ret.push_back(Token(true, buf[i] == '-' && (ret.empty() || ret.back().op) ? '_' : buf[i])), i++;
	}
	//다 읽지 않았는데 끝난 경우, 이상한 문자가 들어온 것이다(허용문자: 연산자, 숫자, 공백)
	if (buf[i])
		throw "ERROR";

	return ret;
}

//infix 토큰배열을 postfix로 바꿔준다.
vector<Token> toPostfix(const vector<Token> &tks, int idx, bool root)
{
	vector<Token> ret;
	stack<Token> st;
	for (auto i = idx; i < tks.size(); i++)
	{
		if (tks[i].op)
		{
			if (tks[i].data == '(')
			{
				//여는 괄호면 i+1의 위치에서 재귀적으로 진행
				auto tmp = toPostfix(tks, i + 1, false);
				ret.push_back(Token(true, '('));
				ret.insert(ret.end(), tmp.begin(), tmp.end());
				ret.push_back(Token(true, ')'));
				//재귀에서 진행한 토큰만큼 건너뜀
				i += tmp.size() + 2;
				i--;
				//닫는괄호가 없이 파싱이 끝나는건 오류
				if (i >= tks.size() || tks[i] != Token{ true, ')' })
					throw "ERROR";
			}
			else if (tks[i].data == ')')
				if (!root)
					break;
				else//재귀의 루트함수에서 닫는괄호가 나오는 것은 오류
					throw "ERROR";
			else
			{
				for (; st.size() &&
					(st.top().priority() > tks[i].priority() ||
						//우선순위가 같을 때, right associated operator는 중단, left associated operator는 진행해야 한다. 
						(st.top().priority() == tks[i].priority() && !st.top().association())); st.pop())
					ret.push_back(st.top());
				st.push(tks[i]);
			}
		}
		else
			ret.push_back(tks[i]);
	}

	for (; st.size(); st.pop())
		ret.push_back(st.top());
	return ret;
}

//기본 연산
int calc(int a, int b, int opd)
{

	switch (opd)
	{
	case'+':
		return a + b;
	case'-':
		return a - b;
	case'*':
		return a * b;
	case'/':
		if (!b)
			throw "ERROR";
		return a / b;
	case'%':
		return a % b;
	case'^':
		return pow(a, b);
	case'_':
		return -b;
	}
	throw "ERROR";
}

//후위연산식의 값을 평가한다.
int eval(const vector<Token> postexp)
{
	stack<int> st;
	for (auto i : postexp)
	{
		if (i.op)
		{
			if (st.empty())
				throw "ERROR";
			int b = st.top(); 
			st.pop();
			
			int a = 0;
			//단항 minus일때는 operand가 하나뿐이다.
			if (i.data != '_')
			{
				if (st.empty())
					throw "ERROR";
				a = st.top(), st.pop();
			}
			st.push(calc(a, b, i.data));
		}
		else
			st.push(i.data);
	}

	//올바른 수식은 항상 stack의 크기가 1일 수밖에 없다(크다면 operator가 부족, 작다면 operand가 부족한 것)
	if (st.size() != 1)
		throw "ERROR";

	return st.top();
}

int main()
{
	char buf[10000];

	while (true)
	{
		cin.getline(buf, sizeof buf);
		if (!strcmp(buf, "q"))
			break;

		try
		{
			auto infix = parse(buf);
			auto postfix = toPostfix(infix, 0, true);
			//postfix로 변환하면서 필요없어진 괄호를 지운다.
			infix.erase(remove(infix.begin(), infix.end(), Token(true, '(')), infix.end());
			infix.erase(remove(infix.begin(), infix.end(), Token(true, ')')), infix.end());
			postfix.erase(remove(postfix.begin(), postfix.end(), Token(true, '(')), postfix.end());
			postfix.erase(remove(postfix.begin(), postfix.end(), Token(true, ')')), postfix.end());
			if (infix.size() != postfix.size())
				throw "ERROR";
			int res = eval(postfix);

			cout << "= ";
			//unary minus를 원래 표기로 되돌린다.
			for (auto i = postfix.rbegin();i!=postfix.rend();i++)
			{
				if (i->op)
				{
					int cnt = 0;
					while (i->data == '_')
						i++, cnt++;
					if (cnt % 2)
						i->data *= -1;
				}
			}
			//postfix expression 출력
			for (auto i : postfix)
			{
				if (i.op)
				{
					if (i.data != '_')
						cout << char(i.data) << ' ';
				}
				else
					cout << i.data << ' ';
			}
			cout << endl;
			//계산값 출력
			cout << "= " << res << endl;

		}
		//어떤 예외를 잡던지 ERROR라고 출력하고 진행하면 된다.
		catch (...)
		{
			cout << "ERROR" << endl;
		}
	}

	return 0;
}