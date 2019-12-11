#ifndef CFHEAP_H__
#define CFHEAP_H__

#include <vector>


using namespace std;


template<class T ,bool(*Smaller)(T,T)>
class CFMinHeap
{
public:
	//************************************
	// Method:    CFMinHeap
	// FullName:  CFMinHeap<T, Smaller>::CFMinHeap
	// Access:    public 
	// Returns:   
	// Qualifier: ���캯����Ϊ�˼򻯼��㣬��heap[0]�м�����ռλ����
	//************************************
	CFMinHeap()
	{
		T placeholder;
		heap_entity.push_back(placeholder);
	}
	~CFMinHeap()
	{

	}


	//************************************
	// Method:    UpAdjustHeap
	// FullName:  CFMinHeap<T, Smaller>::UpAdjustHeap
	// Access:    public 
	// Returns:   void
	// Qualifier: heap�м���Ԫ��ʱ���ϵ���
	// Parameter: int index
	//************************************
	void UpAdjustHeap(int index)
	{
		int parent = GetParent(index);
		T index_ele,parent_ele;
		while(index > 1)
		{
			GetHeapElement(index,index_ele);
			GetHeapElement(parent,parent_ele);
			if(Smaller(index_ele,parent_ele) == true)
			{
				Exchange(index,parent);
				index = parent;
				parent = GetParent(index);
			}
			else
			{
				break;
			}

		}
	}

	enum POSITION
	{
		LEFT = 1,
		RIGHT = 2,
		SRC   = 3
	};


	//************************************
	// Method:    FindMinOfThree
	// FullName:  CFMinHeap<T, Smaller>::FindMinOfThree
	// Access:    public 
	// Returns:   POSITION
	// Qualifier: ѡ����Ԫ������С��һ����λ��
	// Parameter: int index
	// Parameter: int left
	// Parameter: int right
	//************************************
	POSITION FindMinOfThree(int index,int left,int right)
	{
		int val = -1;
		T index_ele,left_ele,right_ele;

		GetHeapElement(index,index_ele);	
		if(left > 0)
		{
			GetHeapElement(left,left_ele);
			if(right < 0)
			{
				if(Smaller(index_ele,left_ele) == false)
				{
					return LEFT;
				}
				else
				{
					return SRC;
				}
			}
			else
			{
				GetHeapElement(right,right_ele);
				if(Smaller(right_ele,left_ele) == false)
				{
					if(Smaller(index_ele,left_ele) == false)
					{
						return LEFT;
					}
					else
					{
						return SRC;
					}
				}
				else
				{
					if(Smaller(index_ele,right_ele) == false)
					{
						return RIGHT;
					}
					else
					{
						return SRC;
					}
				}	
			}
		}
		return SRC;
	}

	//************************************
	// Method:    DownAdjustHeap
	// FullName:  CFMinHeap<T, Smaller>::DownAdjustHeap
	// Access:    public 
	// Returns:   void
	// Qualifier: ��heap��ɾ��һ��Ԫ�أ����µ���
	// Parameter: int index
	//************************************
	void DownAdjustHeap(int index)
	{
		int left = GetLeft(index);
		int right = GetRight(index);
		while(index >= 1)
		{
			 POSITION pos =  FindMinOfThree(index,left,right);
			 if(pos == SRC)
			 {
				 break;
			 }
			 else if(pos == LEFT)
			 {
				 Exchange(index,left);
				 index = left;
				 left = GetLeft(index);
				 right = GetRight(index); 
			 }
			 else
			 {
				 Exchange(index,right);
				 index = right;
				 left = GetLeft(index);
				 right = GetRight(index); 
			 }
		}

	}



	//************************************
	// Method:    InsertElement
	// FullName:  CFMinHeap<T, Smaller>::InsertElement
	// Access:    public 
	// Returns:   void
	// Qualifier: ��heap�в���Ԫ��
	// Parameter: T element
	//************************************
	void InsertElement(T element)
	{
		heap_entity.push_back(element);
		UpAdjustHeap(GetHeapSize());
	}

	bool GetRoot(T& element)
	{
		if(GetHeapSize() >= 1)
		{
			element = heap_entity[1];
			return true;
		}
		return false;
	}

	//************************************
	// Method:    pop
	// FullName:  CFMinHeap<T, Smaller>::pop
	// Access:    public 
	// Returns:   bool
	// Qualifier: ����heap�ĸ�Ԫ��
	// Parameter: T & ele
	//************************************
	bool pop(T& ele)
	{
		if(GetHeapSize() >= 1)
		{
			ele = heap_entity[1];
			Exchange(1,GetHeapSize());
			heap_entity.pop_back();
			DownAdjustHeap(1);

			return true;
		}
		return false;
	}

	int GetParent(int index)
	{
		if(index <= 1)
		{
			return -1;
		}
		return index/2;
	}
	int GetLeft(int index)
	{
		return index*2 < heap_entity.size() ? index*2 : -1;
	}

	int GetRight(int index )
	{
		return index*2+1 < heap_entity.size() ? index*2+1 : -1;
	}

	//************************************
	// Method:    GetHeapElement
	// FullName:  CFMinHeap<T, Smaller>::GetHeapElement
	// Access:    public 
	// Returns:   bool
	// Qualifier: ͨ���±����heap�е�Ԫ��
	// Parameter: int index
	// Parameter: T & ele
	//************************************
	bool GetHeapElement(int index,T& ele)
	{
		if(index >= 1 && index < heap_entity.size())
		{
			ele =  heap_entity[index];

			return true;
		}
		return false;
	}
	//************************************
	// Method:    GetHeapSize
	// FullName:  CFMinHeap<T, Smaller>::GetHeapSize
	// Access:    public 
	// Returns:   int
	// Qualifier: ��ȡheap�Ĵ�С,������heap[0]λ�õ�ռλ����
	//************************************
	int GetHeapSize()
	{
		return heap_entity.size() - 1;
	}

	//************************************
	// Method:    CheckRange
	// FullName:  CFMinHeap<T, Smaller>::CheckRange
	// Access:    public 
	// Returns:   bool
	// Qualifier: ����±��Ƿ��ǺϷ���
	// Parameter: int index
	//************************************
	bool CheckRange(int index)
	{
		if(index >=1 && index <= GetHeapSize())
		{
			return true;
		}

		return false;
	}

	//************************************
	// Method:    Exchange
	// FullName:  CFMinHeap<T, Smaller>::Exchange
	// Access:    public 
	// Returns:   bool
	// Qualifier: ��������Ԫ�ص�λ��
	// Parameter: int index1
	// Parameter: int index2
	//************************************
	bool Exchange(int index1,int index2)
	{
		if(index1 == index2)
		{
			return true;
		}
		if((true == CheckRange(index1)) && (true == CheckRange(index2)))
		{
			T tmp = heap_entity[index1];
			heap_entity[index1] = heap_entity[index2];
			heap_entity[index2] = tmp;
			return true;
		}
		return false;
	}
private:
	vector<T> heap_entity;
	int       heap_size_;
};
#endif
