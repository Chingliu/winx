//@encoding utf8
//@ns NS_STDEXT

// -------------------------------------------------------------------------
// class HashSet

/** ��ϣ���ϡ�����һ�ִ洢Key���������������洢��Key��Ψһ�ġ���ϣ�������ڿ��ٲ���һ��Key��
@arg ValT
	��ϣ�����д�ŵ�Key�������͡�
@arg HashCompT
	��ϣ����Key��Ҫʵ�ֵĲ����ࡣ����Ϊ��
		[[code type="c++"]]
			concept HashCompare
			{
				typename hasher; // ��Key��hashֵ
				typename key_equal; // �Ƚ�����Key�Ƿ����
				typename key_pred; // �ж�һ��Key�Ƿ�С����һ��Key
			};
		[[/code]]
	Ĭ�� HashCompT ȡ NS_STDEXT::HashCompare<ValT>��
@arg AllocT
	�ù�ϣ���ϵ��ڴ��������ࡣĬ�� AllocT ȡ NS_STDEXT::DefaultAlloc��
@*/
template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = DefaultAlloc
	>
class HashSet : public PHashSet<ValT, HashCompT, AllocT>
{

/** ��ϣ���ϵĳ�ʼ����
@arg alloc
	��ϣ����ʹ�õ��ڴ��������
@arg n
	���������nֵ������洢Ͱ������
@*/
	explicit HashSet(AllocT& alloc, size_type n = 100)
		: Base(alloc, n) {}

/** ��ϣ���ϵĳ�ʼ��
@arg alloc
	��ϣ����ʹ�õ��ڴ��������
@arg first
	�ӵ��������ϣ��������ĵ�һ��Ԫ�ء�
@arg last
	�ӵ��������ϣ������������һ��Ԫ�ء�
@arg n
	���������nֵ������洢Ͱ������
@*/
	template <class Iterator>
	HashSet(AllocT& alloc, Iterator first, Iterator last, size_type n = 100)
		: Base(alloc, first, last, n) {}
};

// -------------------------------------------------------------------------
// class HashMultiSet

/** ��ֵ��ϣ���ϡ�����һ�ִ洢Key������������ͨ��ϣ���ϲ�ͬ���ǣ������ж����ͬ��Key��
@arg ValT
	��ϣ�����д�ŵ�Key�������͡�
@arg HashCompT
	��ϣ����Key��Ҫʵ�ֵĲ����ࡣ����Ϊ��
		[[code type="c++"]]
			concept HashCompare
			{
				typename hasher; // ��Key��hashֵ
				typename key_equal; // �Ƚ�����Key�Ƿ����
				typename key_pred; // �ж�һ��Key�Ƿ�С����һ��Key
			};
		[[/code]]
	Ĭ�� HashCompT ȡ NS_STDEXT::HashCompare<ValT>��
@arg AllocT
	�ù�ϣ���ϵ��ڴ��������ࡣĬ�� AllocT ȡ NS_STDEXT::DefaultAlloc��
@*/
template <
	class ValT,
	class HashCompT = HashCompare<ValT>,
	class AllocT = DefaultAlloc
	>
class HashMultiSet : public PHashMultiSet<ValT, HashCompT, AllocT>
{

/** ��ֵ��ϣ���ϵĳ�ʼ��
@arg alloc
	��ֵ��ϣ����ʹ�õ��ڴ��������
@arg n
	���������nֵ������洢Ͱ������
@*/
	explicit HashMultiSet(AllocT& alloc, size_type n = 100)
		: Base(alloc, n) {}

/** ��ֵ��ϣ���ϵĳ�ʼ��
@arg alloc
	��ֵ��ϣ����ʹ�õ��ڴ��������
@arg first
	�ӵ��������ֵ��ϣ��������ĵ�һ��Ԫ�ء�
@arg last
	�ӵ��������ֵ��ϣ������������һ��Ԫ�ء�
@arg n
	���������nֵ������洢Ͱ������
@*/
	template <class Iterator>
	HashMultiSet(AllocT& alloc, Iterator first, Iterator last, size_type n = 100)
		: Base(alloc, first, last, n) {}
};