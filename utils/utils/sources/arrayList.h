#ifndef A3D_ARRAY_LIST_INC
#define A3D_ARRAY_LIST_INC

class arrayList { 
	void **items;
	int  itemsCount;
	int  capacity;
public:	
	
	arrayList() {  
		this->capacity = 0;
		this->itemsCount = 0;
		this->items = NULL;
	}
	arrayList(int capacity) { 
		this->capacity = capacity;
		this->items = new void*[this->capacity];
		this->itemsCount = 0;
	}
	~arrayList() { this->clear(); }

	void initialize(int capacity) { 
		this->clear();
		
		this->capacity = capacity;
		this->items = new void*[this->capacity];
		this->itemsCount = 0;
	}

	void clear() { 
		if(this->itemsCount != NULL) { 
			for(int i = 0; i < this->itemsCount; i++)
				delete this->items[i];
			delete[] this->items;
		}
		this->items = NULL;
		this->itemsCount = 0;
		this->capacity = 0;
	}

	int addItem(void *item) { 
		if(this->itemsCount >= this->capacity - 1) return -1;
		this->items[this->itemsCount] = item;
		this->itemsCount ++;
	}

	void insert(int index, void *item) { 
		if(this->itemsCount >= this->capacity - 1) return;
		for(int i = this->itemsCount - 1; i > index; i--) { 
			this->items[i] = this->items[i-1];
		}
		this->items[index] = item;
		this->itemsCount ++;
	}

	void removeAt(int index) { 
		for(int i = index; i < this->itemsCount - 2; i++) { 
			this->items[i] = this->items[i+1];
		}
		this->itemsCount --;
	}

	int getCount() { return this->itemsCount; } 

	void* getItem(int index) { return this->items[index]; }
	void setItem(int index, void *item) { this->items[index] = item; }
};

#endif