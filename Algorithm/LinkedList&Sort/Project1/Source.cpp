#include <iostream>
#include <algorithm>
#include <string>
#include <ctime>
#include <chrono>

#define RANDOM_PIVOT 1
#define THREE_WAY_PART 1

using namespace std;
using namespace std::chrono;

struct d_node
{
	int data;
	d_node *next;
	d_node *prev;

	d_node() :data(0), next(nullptr), prev(nullptr) {}

	d_node(d_node *it, int data) :data(data)
	{
		prev = it->prev;
		next = it;
		prev->next = this;
		next->prev = this;
	}
};

struct d_list
{
	//moc nodes
	//moc node를 사용하면 이중연결리스트에서 삽입, 삭제시 null체크할 필요가 없어짐.
	d_node *pbegin;//prev of begin
	d_node *end;

	d_list()
	{
		pbegin = new d_node();
		end = new d_node();
		pbegin->next = end;
		end->prev = pbegin;
	}

	int list_cnt()
	{
		int ret = -1;
		d_node *cur = pbegin;
		while (cur != end)
		{
			ret++;
			cur = cur->next;
		}
		return ret;
	}

	void list_insert_head(int val)
	{
		new d_node(pbegin->next, val);
	}

	void list_insert_tail(int val)
	{
		new d_node(end, val);
	}

	void list_insert_index(int idx, int val)
	{
		new d_node(list_search_index(idx), val);
	}

	void list_delete_head()
	{
		del(pbegin->next);
	}

	void list_delete_tail()
	{
		del(end->prev);
	}
	//val을 data로 갖는 모든 원소를 삭제
	void list_delete_data(int val)
	{
		d_node *cur = pbegin->next;
		while (cur != end)
		{
			d_node *ncur = cur->next;
			if (cur->data == val)
				del(cur);
			cur = ncur;
		}
	}

	void list_delete_index(int idx)
	{
		del(list_search_index(idx));
	}

	d_node *list_search_index(int idx)
	{
		d_node *ret = pbegin->next;
		for (int i = 0; i < idx; i++)
			ret = ret->next;
		return ret;
	}

	d_node *list_search_data(int val)
	{
		d_node *cur = pbegin->next;
		while (cur != end)
		{
			if (cur->data == val)
				return cur;
			cur = cur->next;
		}
		return nullptr;
	}

	void list_clear()
	{
		while (pbegin->next != end)
			list_delete_head();
	}

	void list_copy(d_list &r)
	{
		r.list_clear();
		d_node *cur = pbegin->next;
		while (cur != end)
		{
			r.list_insert_tail(cur->data);
			cur = cur->next;
		}
	}

	enum SortAlgorithm
	{
		sa_exchange,
		sa_quick,
		sa_merge,
	};

	void list_sort_asc(int op)
	{
		switch (op)
		{
		case sa_exchange:
			esort(false);
			break;
		case sa_quick:
			qsort(pbegin->next, end->prev, false);
			break;
		case sa_merge:
			msort(pbegin->next, end->prev, false);
			break;
		}
	}

	void list_sort_desc(int op)
	{
		switch (op)
		{
		case sa_exchange:
			esort(true);
			break;
		case sa_quick:
			qsort(pbegin->next, end->prev, true);
			break;
		case sa_merge:
			msort(pbegin->next, end->prev, true);
			break;
		}
	}

	void print()
	{
		d_node *cur = pbegin->next;
		while (cur != end)
		{
			cout << cur->data << ' ';
			cur = cur->next;
		}
		cout << endl;
	}

	//인자로 들어온 node를 삭제한다.
	void del(d_node *it)
	{
		d_node *p = it->prev;
		d_node *n = it->next;
		delete(it);
		p->next = n;
		n->prev = p;
	}

	//교환정렬(exchange sort)
	void esort(bool desc)
	{
		for (d_node *i = pbegin->next; i != end; i = i->next)
			for (d_node *j = i->next; j != end; j = j->next)
				if (desc ? i->data < j->data : i->data > j->data)
					swap(i->data, j->data);
	}

	//퀵정렬
	void qsort(d_node *l, d_node *r, bool desc)
	{
		const auto comp = [desc](int a, int b) { return desc ? a >= b : a <= b; };

		//0 또는 1개짜리 리스트는 정렬되있다.
		if (l == r || l->prev == r)
			return;
		d_node *pivot = l;
		d_node *rsave = r;
#if RANDOM_PIVOT
		swap(pivot->data, jump(l, rand() % dist(l, r))->data);
#endif
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
	void msort(d_node *l, d_node *r, bool desc)
	{
		//0 또는 1개짜리 리스트는 정렬되있다.
		if (l == r || l->prev == r)
			return;

		d_node *m = jump(l, dist(l, r) / 2);
		msort(l, m, desc);
		msort(m->next, r, desc);
		merge(l, m, r, desc);
	}
	//정렬된 두 리스트([l, m], [m+1, r])를 T(n)에 합친다.
	void merge(d_node *l, d_node *m, d_node *r, bool desc)
	{
		auto comp = [desc](int a, int b) { return desc ? a >= b : a <= b; };

		int sz = dist(l, r) + 1;
		int *buf = new int[sz];
		int bufi = 0;

		d_node *l1 = l;
		d_node *l2 = m->next;
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

		d_node *cur = l;
		for (int i = 0; i < sz; i++)
		{
			cur->data = buf[i];
			cur = cur->next;
		}

		delete[] buf;
	}
	//l과 r 사이의 거리를 반환한다.
	int dist(d_node *l, d_node *r)
	{
		int ret = 0;
		while (l != r)
			ret++, l = l->next;
		return ret;
	}
	//(노드n의 idx)+d를 인덱스로 갖는 노드를 반환한다.
	d_node *jump(d_node *n, int d)
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
	srand((unsigned)time(0));

	d_list a;
	for (int i = 0; i < 20; i++)
		a.list_insert_tail(rand() % 100);
	d_list b;
	for (int i = 0; i < 20; i++)
		b.list_insert_tail(rand() % 100);

	cout << "list a: "; a.print();
	cout << "list b: "; b.print();
	cout << "a.list_cnt(): " << a.list_cnt() << endl;
	a.list_insert_head(-3);
	cout << "a.list_insert_head(-3): "; a.print();
	a.list_insert_tail(10000);
	cout << "a.list_insert_tail(10000): "; a.print();
	a.list_insert_index(3, -999);
	cout << "a.list_insert_index(3, -999): "; a.print();
	a.list_insert_index(15, 999);
	cout << "a.list_insert_index(3, 999): "; a.print();
	a.list_delete_head();
	cout << "a.list_delete_head(): "; a.print();
	a.list_delete_tail();
	cout << "a.list_delete_tail(): "; a.print();
	a.list_delete_data(-999);
	cout << "a.list_delete_data(-999): "; a.print();
	a.list_delete_index(2);
	cout << "a.list_delete_index(2): "; a.print();
	cout << "a.list_search_index(5)->data: " << a.list_search_index(5)->data << endl;
	cout << "a.li.list_search_data(999)->data: " << a.list_search_data(999)->data << endl;
	a.list_clear();
	cout << "a.list_clear(): "; a.print();
	b.list_copy(a);
	cout << "b.list_copy(a): " << endl;
	cout << "\tlist a: "; a.print();
	cout << "\tlist b: "; a.print();
	a.list_sort_asc(d_list::sa_exchange);
	cout << "a.list_sort_asc(d_list::sa_exchange): "; a.print();
	b.list_sort_desc(d_list::sa_exchange);
	cout << "b.list_sort_desc(d_list::sa_exchange): "; b.print();
	cout << endl;

	const int n = 33333;
	string i2name[3] = { "exchange sort" ,"quick sort" , "merge sort" };

	cout << "case: random element, n=" << n << endl;
	for (int i = 0; i < 3; i++)
	{
		d_list li;
		for (int i = 0; i < n; i++)
			li.list_insert_tail(rand());

		auto start = system_clock::now();
		li.list_sort_asc(i);
		auto end = system_clock::now();
		duration<double> elapsed = end - start;

		cout << i2name[i] << ":" << endl;
		cout << elapsed.count() << endl;
	}
	cout << endl;

	cout << "case: all same element, n=" << n << endl;
	for (int i = 0; i < 3; i++)
	{
		d_list li;
		for (int i = 0; i < n; i++)
			li.list_insert_tail(1);

		auto start = system_clock::now();
		li.list_sort_asc(i);
		auto end = system_clock::now();
		duration<double> elapsed = end - start;

		cout << i2name[i] << ":" << endl;
		cout << elapsed.count() << endl;
	}
	cout << endl;

	cout << "case: sorted input, n=" << n << endl;
	for (int i = 0; i < 3; i++)
	{
		d_list li;
		for (int i = 0; i < n; i++)
			li.list_insert_tail(i);

		auto start = system_clock::now();
		li.list_sort_asc(i);
		auto end = system_clock::now();
		duration<double> elapsed = end - start;

		cout << i2name[i] << ":" << endl;
		cout << elapsed.count() << endl;
	}
	cout << endl;

	cout << "case: desc sorted input, n=" << n << endl;
	for (int i = 0; i < 3; i++)
	{
		d_list li;
		for (int i = 0; i < n; i++)
			li.list_insert_tail(n - i);

		auto start = system_clock::now();
		li.list_sort_asc(i);
		auto end = system_clock::now();
		duration<double> elapsed = end - start;

		cout << i2name[i] << ":" << endl;
		cout << elapsed.count() << endl;
	}
	cout << endl;
	
	return 0;
}