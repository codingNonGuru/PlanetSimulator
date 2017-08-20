#pragma once

#include <iostream>

class MemoryLog {
public:
	static int64_t usedAmount_;

	static void accrue(int64_t);
};

namespace container {
	template<class O, typename I>
	class DynamicBlock;

	template<class O, typename I>
	class StaticBlock;

	template<class O>
	class Pool {
	};

	template<typename I = int>
	class Container {
	public:
		virtual I getWidth() const {return 0;}
		virtual I getHeight() const {return 0;}
		virtual void* getVoidStart() const {return nullptr;}
	};

	template<class O>
	class LayeredGrid {
	private:
		int height_;
		int width_;
		int depth_;
		int objectCount_;
		O* objects_;
		long memorySize_;
		int tierSize_;
	public:
		O* operator() (int x, int y, int z) {
			if(x < 0)
				x += width_;
				//x = width_ - 1;
			else if(x >= width_)
				x -= width_;
				//x = 0;

			if(y < 0)
				y += height_;
				//y = height_ - 1;
			else if(y >= height_)
				y -= height_;
				//y = 0;

			if(z < 0)
				z += depth_;
			else if(z >= depth_)
				z -= depth_;

			return objects_ + (tierSize_ * z + width_ * y + x);
		}

		O* get(int x, int y, int z) {
			if(x < 0)
				x += width_;
			else if(x >= width_)
				x -= width_;

			if(y < 0)
				y += height_;
			else if(y >= height_)
				y -= height_;

			if(z < 0)
				z += depth_;
			else if(z >= depth_)
				z -= depth_;

			return objects_ + (tierSize_ * z + width_ * y + x);
		}

		O* getStart() {
			return objects_;
		}

		O* getEnd() {
			return objects_ + objectCount_;
		}

		int getWidth() const {
			return width_;
		}

		int getHeight() const {
			return height_;
		}

		int getDepth() const {
			return depth_;
		}

		void* getVoidStart() const {
			return (void*)objects_;
		}

		int getMemoryUse() const {
			return memorySize_;
		}

		int getTierMemory() const {
			return tierSize_ * sizeof(O);
		}

		LayeredGrid() {
			objects_ = nullptr;
		}

		LayeredGrid(int width, int height, int depth) {
			objectCount_ = height * width * depth;
			objects_ = new O[objectCount_];
			width_ = width;
			height_ = height;
			depth_ = depth;
			memorySize_ = objectCount_ * sizeof(O);
			tierSize_ = height_ * width_;

			MemoryLog::accrue(memorySize_);
		}

		void initialize(int columnCount, int rowCount, int depth) {
			objectCount_ = rowCount * columnCount * depth;
			objects_ = new O[objectCount_];
			width_ = columnCount;
			height_ = rowCount;
			depth_ = depth;
			memorySize_ = objectCount_ * sizeof(O);
			tierSize_ = height_ * width_;

			MemoryLog::accrue(memorySize_);
		}

		void destroy() {
			if(objects_ != nullptr)
				delete[] objects_;

			objects_ = nullptr;

			MemoryLog::accrue(-memorySize_);
		}
	};

	template<class O>
	class Grid : public Container<int> {
	private:
		int height_;
		int width_;
		int objectCount_;
		O* objects_;
		int memorySize_;
	public:
		O* operator() (int x, int y) {
			if(x < 0)
				x += width_;
				//x = width_ - 1;
			else if(x >= width_)
				x -= width_;
				//x = 0;

			if(y < 0)
				y += height_;
				//y = height_ - 1;
			else if(y >= height_)
				y -= height_;
				//y = 0;

			return objects_ + (width_ * y + x);
		}

		O* get(int x, int y) {
			if(x < 0)
				x = width_ - 1;
			else if(x >= width_)
				x = 0;

			if(y < 0)
				y = height_ - 1;
			else if(y >= height_)
				y = 0;

			return objects_ + (width_ * y + x);
		}

		O* getStart() {
			return objects_;
		}

		O* getEnd() {
			return objects_ + objectCount_;
		}

		int getWidth() const override {
			return width_;
		}

		int getHeight() const override {
			return height_;
		}

		void* getVoidStart() const {
			return (void*)objects_;
		}

		int getMemoryUse() const {
			return memorySize_;
		}

		Grid() {
			objects_ = nullptr;
		}

		Grid(int columnCount, int rowCount) {
			objectCount_ = rowCount * columnCount;
			objects_ = new O[objectCount_];
			width_ = columnCount;
			height_ = rowCount;
			memorySize_ = objectCount_ * sizeof(O);

			MemoryLog::accrue(memorySize_);
		}

		~Grid() {
			destroy();
		}

		void initialize(int columnCount, int rowCount) {
			objectCount_ = rowCount * columnCount;
			destroy();
			objects_ = new O[objectCount_];
			width_ = columnCount;
			height_ = rowCount;
			memorySize_ = objectCount_ * sizeof(O);

			MemoryLog::accrue(memorySize_);
		}

		void destroy() {
			if(objects_ != nullptr)
				delete[] objects_;

			objects_ = nullptr;

			MemoryLog::accrue(-memorySize_);
		}
	};

	template<class O, typename I = int>
	class Array : public Pool<O> {
		O* objectStart_;
		I size_;
		unsigned long memorySize_;

	public:
		I objectCount_;

		Array() {
			objectStart_ = nullptr;
		}

		Array(I size) : size_(size){
			memorySize_ = size_ * sizeof(O);
			objectCount_ = 0;
			objectStart_ = new O[size_];

			MemoryLog::accrue(memorySize_);
		}

		void initialize(I size) {
			size_ = size;
			memorySize_ = size_ * sizeof(O);
			objectCount_ = 0;
			destroy();
			objectStart_ = new O[size_];

			MemoryLog::accrue(memorySize_);
		}

		void reset() {
			objectCount_ = 0;
		}

		bool isFull() {
			return objectCount_ == size_ - 1;
		}

		O *const allocate(I count) {
			O *const newObject = (objectStart_ + objectCount_);
			objectCount_ += count;
			return newObject;
		}

		O *const allocate() {
			O *const newObject = (objectStart_ + objectCount_);
			objectCount_++;
			if(objectCount_ == size_ + 1) {
				std::cout<<"Abort "<<objectCount_<<"\n";
				abort();
			}
			return newObject;
		}

		template<class DerivedType>
		O *const allocate() {
			O *const newObject = (objectStart_ + objectCount_);
			*(long*)newObject = *(long*)&DerivedType();
			objectCount_++;
			if(objectCount_ == size_ + 1) {
				std::cout<<"Abort "<<objectCount_<<"\n";
				abort();
			}
			return newObject;
		}

		O* get(I index) {
			if(index < 0)
				index += size_;
			if(index >= size_)
				index -= size_;

			return objectStart_ + index;
		}

		O* getStart() {
			return objectStart_;
		}

		O* getEnd() {
			return (objectStart_ + objectCount_);
		}

		I getSize() const {
			return objectCount_;
		}

		I getCapacity() const {
			return size_;
		}

		unsigned int getMemorySize() {
			return objectCount_ * sizeof(O);
		}

		void insert(O* from, O first, O last) {
			O iterator_2 = first;
			for(O* iterator = from; iterator_2 != last + 1; ++iterator, ++iterator_2) {
				*iterator = iterator_2;
			}
		}

		void insertAndMove(O* from, O* first, O* last) {
			O* iterator_2 = first;
			for(O* iterator = from; iterator_2 != last + 1; ++iterator, ++iterator_2) {
				*iterator = *iterator_2;
			}
			from += (last - first);
		}

		I getMemoryUse() const {
			return memorySize_;
		}

		void setAllToNull() {
			for(unsigned int* iterator = (unsigned int*)objectStart_; iterator != (unsigned int*)objectStart_ + (memorySize_ / sizeof(unsigned int)); ++iterator)
				*iterator = 0;
		}

		void destroy() {
			if(objectStart_ != nullptr)
				delete[] objectStart_;

			objectStart_ = nullptr;

			MemoryLog::accrue(-memorySize_);
		}
	};

	template<class O, typename Iobject, typename Iindex>
	class StaticFragmentedPoolDynamicBlocked {
		O* blockStart_;
		Iindex* indexStart_;
		Iobject actualObjectCount_;
		Iobject possibleObjectCount_;
		Iobject possibleMemorySize_;

	public:
		StaticFragmentedPoolDynamicBlocked();

		StaticFragmentedPoolDynamicBlocked(Iobject possibleObjectCount);

		void initialize(Iobject possibleObjectCount);

		void allocate(Iindex blockObjectCount, DynamicBlock<O, Iindex> &block);
	};

	template<class O, typename Iobject>
	class StaticFragmentedPoolStaticBlocked {
		O* blockStart_;
		Iobject actualObjectCount_;
		Iobject possibleObjectCount_;
		Iobject possibleMemorySize_;

	public:
		StaticFragmentedPoolStaticBlocked();

		StaticFragmentedPoolStaticBlocked(Iobject possibleObjectCount);

		void allocate(unsigned char blockObjectCount, StaticBlock<O, unsigned char> &block);
	};

	template<class O, typename I>
	class StaticBlock {
		O* objectStart_;
		I objectCount_;

	public:
		void initialize(O* objectStart, I objectCount) {
			objectCount_ = objectCount;
			objectStart_ = objectStart;
		}

		bool isEmpty() const {
			return objectCount_ == 0 ? true : false;
		}

		O* getStart() const {
			return objectStart_;
		}

		void setStart(O* objectStart) {
			objectStart_ = objectStart;
		}

		O* getEnd() const {
			return objectStart_ + objectCount_;
		}

		I getSize() const {
			return objectCount_;
		}

		void setSize(I objectCount) {
			objectCount_ = objectCount;
		}

		O* get(I index) {
			return objectStart_ + index;
		}
	};

	template<class O, typename I>
	class DynamicBlock {
		O* objectStart_;
		I* indexStart_;
		I actualObjectCount_;
		I possibleObjectCount_;
		I firstIndex_;

	public:
		DynamicBlock();

		DynamicBlock(I possibleObjectCount, O* objectStart);

		void initialize(StaticFragmentedPoolDynamicBlocked <O, unsigned int, unsigned short> &pool, I objectCount);

		O* allocate(O object);

		void deallocate(O* object);

		bool isEmpty() const {
			return actualObjectCount_ == 0 ? true : false;
		}

		O* getEnd() {
			return objectStart_ + possibleObjectCount_;
		}

		O* getStart() {
			return objectStart_;
		}

		I getActualSize() const {
			return actualObjectCount_;
		}

		I getPossibleSize() const {
			return possibleObjectCount_;
		}

		void setStart(O* objectStart) {
			objectStart_ = objectStart;
		}

		void setIndexStart(I* indexStart) {
			indexStart_ = indexStart;
		}
	};

	template<class O, typename I>
	class DynamicPool {
		O* objectStart_;
		I size_;
		unsigned long memorySize_;
		I* freeIndexStart_;
		I firstFreeIndex_;

	public:
		I objectCount_;

		DynamicPool();

		DynamicPool(I size);

		void initialize(I size) {
			size_ = size;
			memorySize_ = (size_ * sizeof(O)) + (size_ * sizeof(I));
			objectCount_ = 0;
			objectStart_ = new O[size_];
			freeIndexStart_ = new I[size_];
			firstFreeIndex_ = size_ - 1;
			for(I N = 0; N < size_; ++N) {
				*(freeIndexStart_ + N) = N;
			}
#ifdef TRACE_ENABLED
			std::cout<<"Dynamic Pool was created, totalling "<<size<<" objects.\n";
			std::cout<<"Total container used is "<< (float)memorySize_ / 1048576 <<" MBytes.\n";
			std::cout<<"\n";
#endif
		}

		O *const allocate();

		template<class DerivedType>
		O *const allocate() {
			O *const newObject = (objectStart_ + *(freeIndexStart_ + firstFreeIndex_));
			*(long*)newObject = *(long*)&DerivedType();
			firstFreeIndex_--;
			objectCount_++;
			if(objectCount_ == size_ + 1) {
				std::cout<<"Abort "<<objectCount_<<"\n";
				abort();
			}
			return newObject;
		}

		void deallocate(O *const object);

		O* get(I index) {
			return objectStart_ + index;
		}

		O* getStart();

		O* getLastObject();

		O* getEnd();

		I getSize();

		I getCapacity() {return size_;}

		float getFillPercentage();

		void insert(O* from, O first, O last);

		void insertAndMove(O* from, O* first, O* last);

		void insertAndMove(DynamicBlock<O, unsigned short> &block);

		void setAllToNull();
	};

	template<class O>
	class Block {
	public:
		virtual O* getStart() {return nullptr;}
		virtual O* getEnd() {return nullptr;}
		virtual bool isEmpty() {return true;}
		virtual bool isFull() {return true;}
		virtual O* allocate() {return nullptr;}
		virtual void deallocate(O*) {}
		virtual unsigned short getSize() {return 0;}
	};

	template<class O, typename I, int C>
	class DynamicBlockStaticSize : public Block<O> {
		O objects_[C];
		I freeIndices_[C];
		//I takenIndices_[C];
		I firstIndex_;
		I objectCount_;

	public:
		DynamicBlockStaticSize() {
			objectCount_ = 0;
			firstIndex_ = C - 1;
			for(I i = 0; i < C; ++i)
				freeIndices_[i] = i;
		}

		O* getStart() {
			return &objects_[0];
		}

		O* getEnd() {
			return &objects_[C];
		}

		unsigned short getSize() {
			return objectCount_;
		}

		bool isEmpty() {
			return objectCount_ == 0 ? true : false;
		}

		bool isFull() {
			return objectCount_ == C ? true : false;
		}

		O* allocate() {
			//takenIndices_[objectCount_] = freeIndices_[firstIndex_];
			objectCount_++;
			if(objectCount_ == C + 1) {
				std::cout<<"Abort "<<objectCount_<<"\n";
				abort();
			}
			firstIndex_--;
			return &objects_[freeIndices_[firstIndex_ + 1]];
		}

		void deallocate(O* object) {
			objectCount_--;
			firstIndex_++;
			freeIndices_[firstIndex_] = object - &objects_[0];
		}
	};
}
namespace container {
	template<class O, typename Iobject, typename Iindex>
	StaticFragmentedPoolDynamicBlocked<O, Iobject, Iindex>::StaticFragmentedPoolDynamicBlocked() {}

	template<class O, typename Iobject, typename Iindex>
	StaticFragmentedPoolDynamicBlocked<O, Iobject, Iindex>::StaticFragmentedPoolDynamicBlocked(Iobject possibleObjectCount) {
		possibleObjectCount_ = possibleObjectCount;
		actualObjectCount_ = 0;
		possibleMemorySize_ = possibleObjectCount * sizeof(O);
		blockStart_ = new O[possibleObjectCount_];
		for(O* clearIterator = blockStart_; clearIterator != blockStart_ + possibleObjectCount_; ++clearIterator)
			*clearIterator = nullptr;
		indexStart_ = new Iindex[possibleObjectCount_];
	}

	template<class O, typename Iobject, typename Iindex>
	void StaticFragmentedPoolDynamicBlocked<O, Iobject, Iindex>::initialize(Iobject possibleObjectCount) {
		possibleObjectCount_ = possibleObjectCount;
		actualObjectCount_ = 0;
		possibleMemorySize_ = possibleObjectCount * sizeof(O);
		blockStart_ = new O[possibleObjectCount_];
		for(O* clearIterator = blockStart_; clearIterator != blockStart_ + possibleObjectCount_; ++clearIterator)
			*clearIterator = nullptr;
		indexStart_ = new Iindex[possibleObjectCount_];
	}

	template<class O, typename Iobject, typename Iindex>
	void StaticFragmentedPoolDynamicBlocked<O, Iobject, Iindex>::allocate(Iindex blockObjectCount, DynamicBlock<O, Iindex> &block) {
		actualObjectCount_ += blockObjectCount;
		block.setStart(blockStart_);
		block.setIndexStart(indexStart_);
		blockStart_ += blockObjectCount;
		indexStart_ += blockObjectCount;
	}



	template<class O, typename Iobject>
	StaticFragmentedPoolStaticBlocked<O, Iobject>::StaticFragmentedPoolStaticBlocked() {}

	template<class O, typename Iobject>
	StaticFragmentedPoolStaticBlocked<O, Iobject>::StaticFragmentedPoolStaticBlocked(Iobject possibleObjectCount) {
		possibleObjectCount_ = possibleObjectCount;
		actualObjectCount_ = 0;
		possibleMemorySize_ = possibleObjectCount * sizeof(O);
		blockStart_ = new O[possibleObjectCount_];
	}

	template<class O, typename Iobject>
	void StaticFragmentedPoolStaticBlocked<O, Iobject>::allocate(unsigned char blockObjectCount, StaticBlock<O, unsigned char> &block) {
		actualObjectCount_ += blockObjectCount;
		block.setStart(blockStart_);
		block.setSize(blockObjectCount);
		blockStart_ += blockObjectCount;
	}



	template<class O, typename I>
	DynamicBlock<O, I>::DynamicBlock() {
		actualObjectCount_ = 0;
	}

	template<class O, typename I>
	DynamicBlock<O, I>::DynamicBlock(I possibleObjectCount, O* objectStart) {
		possibleObjectCount_ = possibleObjectCount;
		actualObjectCount_ = 0;
		objectStart_ = objectStart;
		firstIndex_ = possibleObjectCount_ - 1;
	}

	template<class O, typename I>
	void DynamicBlock<O, I>::initialize(StaticFragmentedPoolDynamicBlocked <O, unsigned int, unsigned short> &pool, I objectCount) {
		pool.allocate(objectCount, *this);
		possibleObjectCount_ = objectCount;
		actualObjectCount_ = 0;
		I freeIndex = 0;
		for(I* indexIterator = indexStart_; indexIterator != indexStart_ + possibleObjectCount_; ++indexIterator) {
			*indexIterator = freeIndex++;
		}
		firstIndex_ = objectCount - 1;
	}

	template<class O, typename I>
	O* DynamicBlock<O, I>::allocate(O object) {
		if(actualObjectCount_ == possibleObjectCount_ + 1) {
			std::cout<<"Abort "<<actualObjectCount_<<"\n";
			abort();
		}
		actualObjectCount_++;
		*(objectStart_ + *(indexStart_ + firstIndex_)) = object;
		firstIndex_--;
		return objectStart_ + *(indexStart_ + firstIndex_ + 1);
	}

	template<class O, typename I>
	void DynamicBlock<O, I>::deallocate(O* object) {
		*object = nullptr;
		actualObjectCount_--;
		*(indexStart_ + ++firstIndex_) = object - objectStart_;
	}



	template<class O, typename I>
	DynamicPool<O, I>::DynamicPool() {
	}

	template<class O, typename I>
	DynamicPool<O, I>::DynamicPool(I size) : size_(size){
		memorySize_ = (size_ * sizeof(O)) + (size_ * sizeof(I));
		objectCount_ = 0;
		objectStart_ = new O[size_];
		freeIndexStart_ = new I[size_];
		firstFreeIndex_ = size_ - 1;
		for(I N = 0; N < size_; ++N) {
			*(freeIndexStart_ + N) = N;
		}
#ifdef TRACE_ENABLED
		std::cout<<"Dynamic Pool was created, totalling "<<size<<" objects.\n";
		std::cout<<"Total container used is "<< (float)memorySize_ / 1048576 <<" MBytes.\n";
		std::cout<<"\n";
#endif
	}

	template<class O, typename I>
	O *const DynamicPool<O, I>::allocate() {
		O *const newObject = (objectStart_ + *(freeIndexStart_ + firstFreeIndex_));
		firstFreeIndex_--;
		objectCount_++;
		if(objectCount_ == size_ + 1) {
			std::cout<<"Abort "<<objectCount_<<"\n";
			abort();
		}
		return newObject;
	}

	template<class O, typename I>
	void DynamicPool<O, I>::deallocate(O *const object) {
		I index = object - objectStart_;
		firstFreeIndex_++;
		*(freeIndexStart_ + firstFreeIndex_) = index;
		objectCount_--;
	}

	template<class O, typename I>
	O* DynamicPool<O, I>::getStart() {
		return objectStart_;
	}

	template<class O, typename I>
	O* DynamicPool<O, I>::getLastObject() {
		return objectStart_ + objectCount_;
	}

	template<class O, typename I>
	O* DynamicPool<O, I>::getEnd() {
		return (objectStart_ + size_);
	}

	template<class O, typename I>
	I DynamicPool<O, I>::getSize() {
		return objectCount_;
	}

	template<class O, typename I>
	float DynamicPool<O, I>::getFillPercentage() {
		return ((float)objectCount_ / (float)size_) * 100.0f;
	}

	template<class O, typename I>
	void DynamicPool<O, I>::insert(O* from, O first, O last) {
		O iterator_2 = first;
		for(O* iterator = from; iterator_2 != last + 1; ++iterator, ++iterator_2) {
			*iterator = iterator_2;
		}
	}

	template<class O, typename I>
	void DynamicPool<O, I>::insertAndMove(O* from, O* first, O* last) {
		O* iterator_2 = first;
		for(O* iterator = from; iterator_2 != last + 1; ++iterator, ++iterator_2) {
			*iterator = *iterator_2;
		}
		from += (last - first);
	}

	template<class O, typename I>
	void DynamicPool<O, I>::insertAndMove(DynamicBlock<O, unsigned short> &block) {
		O* from = objectStart_ + objectCount_;
		if(block.isEmpty() == false) {
			for(O* blockIterator = block.getStart(); blockIterator != block.getEnd(); ++blockIterator)
				if(*blockIterator != 0) {
					*from = *blockIterator;
					from++;
				}
			objectCount_ += block.getActualSize();
		}
	}

	template<class O, typename I>
	void DynamicPool<O, I>::setAllToNull() {
		for(unsigned int* iterator = (unsigned int*)objectStart_; iterator != (unsigned int*)objectStart_ + (memorySize_ / sizeof(unsigned int)); ++iterator)
			*iterator = 0;
	}
}
