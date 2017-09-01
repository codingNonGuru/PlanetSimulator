#include <stdlib.h>
#include "Memory.hpp"

int64_t MemoryLog::usedAmount_ = 0;

void MemoryLog::accrue(int64_t amount) {
	usedAmount_ += amount;
	//std::cout<<"Memory in use is - "<<usedAmount_<<" bytes. Just added - "<<amount<<" bytes.\n";
}

/*namespace container {
	template<class O, typename I>
	StaticFragmentedPoolDynamicBlocked<O, I>::StaticFragmentedPoolDynamicBlocked() {}
	
	template<class O, typename I>
	StaticFragmentedPoolDynamicBlocked<O, I>::StaticFragmentedPoolDynamicBlocked(I possibleObjectCount) {
		possibleObjectCount_ = possibleObjectCount;
		actualObjectCount_ = 0;
		possibleMemorySize_ = possibleObjectCount * sizeof(O);
		blockStart_ = new O[possibleObjectCount_];
		indexStart_ = new I[possibleObjectCount_];
	}

	template<class O, typename I>
	void StaticFragmentedPoolDynamicBlocked<O, I>::allocate(I blockObjectCount, DynamicBlock<O, unsigned short> &block) {
		actualObjectCount_ += blockObjectCount;
		block.objectStart_ = blockStart_;
		block.indexStart_ = indexStart_;
		blockStart_ += blockObjectCount;
		indexStart_ += blockObjectCount;
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
	void DynamicBlock<O, I>::initialize(StaticFragmentedPoolDynamicBlocked <O, unsigned int> &pool, I objectCount) {
		pool.allocate(objectCount, this);
		possibleObjectCount_ = objectCount;
		I freeIndex = 0;
		for(I* indexIterator = indexStart_; indexIterator != indexStart_ + possibleObjectCount_; ++indexIterator) {
			*indexIterator = freeIndex++;
		}
	}

	template<class O, typename I>
	O* DynamicBlock<O, I>::allocate(O object) {
		*(objectStart_ + *(indexStart_ + firstIndex_)) = object;
		return objectStart_ + *(indexStart_ + firstIndex_--);
	}

	template<class O, typename I>
	void DynamicBlock<O, I>::deallocate(O* object) {
		indexStart_ + (object - objectStart_) = object - objectStart_;
		firstIndex_++;
	}



	template<class O, typename I>
	DynamicPool<O, I>::DynamicPool() {
	}

	template<class O, typename I>
	DynamicPool<O, I>::DynamicPool(I size) : size_(size){
		memorySize_ = size_ * sizeof(O);
		objectCount_ = 0;
		objectStart_ = new O[size_];
		freeIndexStart_ = new I[size_];
		firstFreeIndex_ = 0;
		for(I N = 0; N < size_; ++N) {
			*(freeIndexStart_ + N) = N;
		}
	}

	template<class O, typename I>
	O *const DynamicPool<O, I>::allocate() {
		O *const newObject = (objectStart_ + *(freeIndexStart_ + firstFreeIndex_));
		firstFreeIndex_++;
		objectCount_++;
		return newObject;
	}

	template<class O, typename I>
	void DynamicPool<O, I>::deallocate(O *const object) {
		I index = object - objectStart_;
		*(freeIndexStart_ + firstFreeIndex_) = index;
		firstFreeIndex_--;
		objectCount_--;
	}

	template<class O, typename I>
	O* DynamicPool<O, I>::getStart() {
		return objectStart_;
	}

	template<class O, typename I>
	O* DynamicPool<O, I>::getEnd() {
		return (objectStart_ + size_);
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
	void DynamicPool<O, I>::insertAndMove(O* from, DynamicBlock<O, unsigned short> &block) {
		if(block.isEmpty() == false) {
			for(O* blockIterator = block.objectStart_; blockIterator != block.getEnd(); ++blockIterator)
				if(*blockIterator != 0) {
					*from = *blockIterator;
					from++;
				}
		}
	}

	template<class O, typename I>
	void DynamicPool<O, I>::setAllToNull() {
		for(unsigned int* iterator = (unsigned int*)objectStart_; iterator != (unsigned int*)objectStart_ + (memorySize_ / sizeof(unsigned int)); ++iterator)
			*iterator = 0;
	}
}*/
