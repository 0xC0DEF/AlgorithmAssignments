#include <iostream>
#include <algorithm>
#include <ctime>
#include <chrono>

#define THREE_WAY_PART 1

using namespace std;
using namespace std::chrono;

struct Node
{
	int data;
	Node *next;
	Node *prev;

	Node() :data(0), next(nullptr), prev(nullptr) {}

	Node(Node *it, int data) :data(data)
	{
		prev = it->prev;
		next = it;
		prev->next = this;
		next->prev = this;
	}
};

struct List
{
	//moc nodes
	Node *pbegin;//prev of begin
	Node *end;

	List()
	{
		pbegin = new Node();
		end = new Node();
		pbegin->next = end;
		end->prev = pbegin;
	}

	int size()
	{
		int ret = -1;
		Node *cur = pbegin;
		while (cur != end)
		{
			ret++;
			cur = cur->next;
		}
		return ret;
	}

	void push_front(int val)
	{
		new Node(pbegin->next, val);
	}

	void push_back(int val)
	{
		new Node(end, val);
	}

	void insert(int idx, int val)
	{
		new Node(at(idx), val);
	}

	void pop_front()
	{
		del(pbegin->next);
	}

	void pop_back()
	{
		del(end->prev);
	}

	void remove(int val)
	{
		Node *cur = pbegin->next;
		while (cur != end)
		{
			Node *ncur = cur->next;
			if (cur->data == val)
				del(cur);
			cur = ncur;
		}
	}

	void erase(int idx)
	{
		del(at(idx));
	}

	Node *at(int idx)
	{
		Node *ret = pbegin->next;
		for (int i = 0; i < idx; i++)
			ret = ret->next;
		return ret;
	}

	Node *find(int val)
	{
		Node *cur = pbegin->next;
		while (cur != end)
		{
			if (cur->data == val)
				return cur;
			cur = cur->next;
		}
		return nullptr;
	}

	void clear()
	{
		while (pbegin->next != end)
			pop_front();
	}

	List *clone()
	{
		List *ret = new List();
		Node *cur = pbegin->next;
		while (cur != end)
		{
			ret->push_back(cur->data);
			cur = cur->next;
		}
		return ret;
	}

	void sort_asc(int op)
	{
		switch (op)
		{
		case 1:
			esort(false);
			break;
		case 2:
			qsort(pbegin->next, end->prev, false);
			break;
		case 3:
			msort(pbegin->next, end->prev, false);
			break;
		}
	}

	void sort_desc(int op)
	{
		switch (op)
		{
		case 1:
			esort(true);
			break;
		case 2:
			qsort(pbegin->next, end->prev, true);
			break;
		case 3:
			msort(pbegin->next, end->prev, true);
			break;
		}
	}

	void print()
	{
		//return;
		Node *cur = pbegin->next;
		while (cur != end)
		{
			cout << cur->data << ' ';
			cur = cur->next;
		}
		cout << endl;
	}
private:
	void del(Node *it)
	{
		Node *p = it->prev;
		Node *n = it->next;
		delete(it);
		p->next = n;
		n->prev = p;
	}

	//교환정렬(exchange sort)
	void esort(bool desc)
	{
		for (Node *i = pbegin->next; i != end; i = i->next)
			for (Node *j = i->next; j != end; j = j->next)
				if (desc ? i->data<j->data : i->data > j->data)
					swap(i->data, j->data);
	}

	//퀵정렬
	void qsort(Node *l, Node *r, bool desc)
	{
		const auto comp = [desc](int a, int b) { return desc ? a >= b : a <= b; };

		//0 또는 1개짜리 리스트는 정렬되있다.
		if (l == r || l->prev == r)
			return;
		Node *pivot = l;
		Node *rsave = r;
		swap(pivot->data, jump(l, rand() % dist(l, r))->data);
		l = l->next;

		while (l != r)
		{
#if THREE_WAY_PART
			//3 way partition (for all same element)
			int samecnt = 0;
			while (l != r && (comp(l->data, pivot->data) || comp(pivot->data, r->data)))
			{
				if (comp(l->data, pivot->data) && comp(pivot->data, r->data))
				{
					if(samecnt++%2)
						l = l->next;
					else
						r = r->prev;
				}
				else
				{
					if (comp(l->data, pivot->data))
						l = l->next;
					else
						r = r->prev;
				}
			}
#else
			while (l != r && comp(l->data, pivot->data))
				l = l->next;
			while (l != r && comp(pivot->data, r->data))
				r = r->prev;
#endif

			swap(l->data, r->data);
		}
		if (comp(l->data, pivot->data))
			swap(l->data, pivot->data);
		else
			swap(l->prev->data, pivot->data);

		qsort(pivot, l->prev, desc);
		qsort(l, rsave, desc);
	}

	//병합정렬
	void msort(Node *l, Node *r, bool desc)
	{
		//0 또는 1개짜리 리스트는 정렬되있다.
		if (l == r || l->prev == r)
			return;

		Node *m = jump(l, dist(l, r) / 2);
		msort(l, m, desc);
		msort(m->next, r, desc);
		merge(l, m, r, desc);
	}
	//정렬된 두 리스트([l, m], [m+1, r])를 T(n)에 합친다.
	void merge(Node *l, Node *m, Node *r, bool desc)
	{
		auto comp = [desc](int a, int b) { return desc ? a >= b : a <= b; };

		int sz = dist(l, r) + 1;
		int *buf = new int[sz];
		int bufi = 0;

		Node *l1 = l;
		Node *l2 = m->next;
		while (l1 != m->next && l2 != r->next)
		{
			if (comp(l1->data, l2->data))
			{
				buf[bufi++] = l1->data;
				l1 = l1->next;
			}
			else
			{
				buf[bufi++] = l2->data;
				l2 = l2->next;
			}
		}
		while (l1 != m->next)
		{
			buf[bufi++] = l1->data;
			l1 = l1->next;
		}
		while (l2 != r->next)
		{
			buf[bufi++] = l2->data;
			l2 = l2->next;
		}

		Node *cur = l;
		for (int i = 0; i < sz; i++)
		{
			cur->data = buf[i];
			cur = cur->next;
		}

		delete[] buf;
	}
	//l과 r 사이의 거리를 반환한다.
	int dist(Node *l, Node *r)
	{
		int ret = 0;
		while (l != r)
			ret++, l = l->next;
		return ret;
	}
	//(노드n의 idx)+d를 인덱스로 갖는 노드를 반환한다.
	Node *jump(Node *n, int d)
	{
		while (d)
		{
			n = n->next;
			d -= d / abs(d);
		}
		return n;
	}
};

int main()
{	
	srand(time(0));

	const int n = 30000;
	cout << "case: random element, n=" << n << endl;
	for (int i = 1; i <= 3; i++)
	{
		List li;
		for (int i = 0; i < n; i++)
			li.push_back(rand());

		//li.print();

		auto start = system_clock::now();
		li.sort_asc(i);
		auto end = system_clock::now();
		duration<double> elapsed = end - start;

		//li.print();

		switch (i)
		{
		case 1:
			cout << "exchange sort";
			break;
		case 2:
			cout << "quick sort";
			break;
		case 3:
			cout << "merge sort";
			break;
		}
		cout << ": " << elapsed.count() << endl;
	}
	cout << endl;

	cout << "case: all same element, n=" << n << endl;
	for (int i = 1; i <= 3; i++)
	{
		List li;
		for (int i = 0; i < n; i++)
			li.push_back(1);

		auto start = system_clock::now();
		li.sort_asc(i);
		auto end = system_clock::now();
		duration<double> elapsed = end - start;

		switch (i)
		{
		case 1:
			cout << "exchange sort";
			break;
		case 2:
			cout << "quick sort";
			break;
		case 3:
			cout << "merge sort";
			break;
		}
		cout << ": " << elapsed.count() << endl;
	}
	cout << endl;

	cout << "case: sorted input, n=" << n << endl;
	for (int i = 1; i <= 3; i++)
	{
		List li;
		for (int i = 0; i < n; i++)
			li.push_back(i);

		auto start = system_clock::now();
		li.sort_asc(i);
		auto end = system_clock::now();
		duration<double> elapsed = end - start;

		switch (i)
		{
		case 1:
			cout << "exchange sort";
			break;
		case 2:
			cout << "quick sort";
			break;
		case 3:
			cout << "merge sort";
			break;
		}
		cout << ": " << elapsed.count() << endl;
	}
	cout << endl;

	cout << "case: desc sorted input, n=" << n << endl;
	for (int i = 1; i <= 3; i++)
	{
		List li;
		for (int i = 0; i < n; i++)
			li.push_back(n - i);

		auto start = system_clock::now();
		li.sort_asc(i);
		auto end = system_clock::now();
		duration<double> elapsed = end - start;

		switch (i)
		{
		case 1:
			cout << "exchange sort";
			break;
		case 2:
			cout << "quick sort";
			break;
		case 3:
			cout << "merge sort";
			break;
		}
		cout << ": " << elapsed.count() << endl;
	}
	cout << endl;
	
	return 0;
}