/*
 * memory.cpp
 *
 *  Created on: May 8, 2016
 *      Author: chris
 */

#include "../utils/utils.h"
#include <memory.h>
#include <list.h>
#include <errors.h>
#include <log.h>

typedef struct {
	void* location;
	size_t size;
} Block;

// the list of free blocks
List<Block*> freeBlocks;

/* these pointers are set by the linker; don't use the values */
extern uint8 kernelStart; // &kernelStart - start of the kernel
extern uint8 kernelEnd; // &kernelEnd - end of the kernel

void amemory_init(multiboot_info_t* mbd) {
	if (!getBit(mbd->flags, 6)) {
		crash("6th bit in mbd is not set, cannot get memory map");
	}

	uint64 kernelSize = &kernelEnd - &kernelStart;

	memory_map_t* mmap = (memory_map_t*) mbd->mmap_addr;
	while ((uint32) mmap < mbd->mmap_addr + mbd->mmap_length) {
		mmap = (memory_map_t*) ((unsigned int) mmap + mmap->size
				+ sizeof(unsigned int));

		if (mmap->type == 1) { // memory is useable
			uint64 memoryLocation = merge(mmap->base_addr_high,
					mmap->base_addr_low);
			uint64 memorySize = merge(mmap->length_high, mmap->length_low);
			if (memoryLocation == (uint64) &kernelStart) {
				memoryLocation += kernelSize;
				memorySize -= kernelSize;
			}

			free((void*) memoryLocation // + 0x1b
					, memorySize);
		}
	}
}

// ----- memory management -----

void* amalloc(size_t size) {
	for (uint64 i = 0; i < freeBlocks.size(); i++) {
		Block* block = freeBlocks.get(i);

		if (size == block->size) { // if block matches in size
			// remove this block and return it's location
			free(freeBlocks.remove(i), sizeof(Block*));

			// wipe memory
			memset((uint8*) block->location, 0, size);

			return block->location;
		} else if (block->size > size) {
			// "split" block

			// we must copy the location to a local so that the block
			// location can be updated to split it
			void* location = block->location;

			block->location += size;
			block->size -= size;

			if (block->size == 0) {
				// the block is empty, delete it
				free(freeBlocks.remove(i), sizeof(Block*));
			} else if (freeBlocks.size() > 1) {
				// update the blocks position in the list
				Element<Block*>* blockElement = freeBlocks.removeElement(i);

				for (uint64 j = i; j - 1 < j && j >= 0; j--) {
					Block* thisBlock = freeBlocks.get(j);

					if (block->size >= thisBlock->size) {
						// update position of moving block
						freeBlocks.addElement(blockElement, j + 1);
						break;
					}
				}
			}

			// wipe memory
			memset((uint8*) location, 0, size);

			return location;
		}

		// try the next block (if there is one)
	}

	// out of blocks
	crash(ALLOCATE_FAILED);

	return 0;
}

bool acanAllocate(size_t size) {
	for (uint64 i = 0; i < freeBlocks.size(); i++) {
		if (freeBlocks.get(i)->size >= size) {
			return true;
		}
	}

	// could not find a suitable block
	return false;
}

uint64 aavailableMemory() {
	uint64 availableMemory = 0;

	for (uint64 i = 0; i < freeBlocks.size(); i++) {
		availableMemory += freeBlocks.get(i)->size;
	}

	return availableMemory;
}

void afree(void* location, size_t size) {
	if (freeBlocks.isEmpty()) {
		// we must manually allocate memory since we have no available blocks

		size_t blockSize = sizeof(Block);
		size_t elementSize = sizeof(Element<Block*> );

		size_t metadataSize = blockSize + elementSize;

		Block* block = (Block*) location;
		block->location = location + metadataSize;
		block->size = size - metadataSize;

		Element<Block*>* element = (Element<Block*>*) ((uint64) location
				+ blockSize);
		element->value = block;
		element->next = 0;

		freeBlocks.addElement(element);
	} else {
		uint64 potentialSizeIndex = 0;

		Block* alreadyMergedBlock = 0;

		for (uint64 i = 0; i - 1 < i && i >= 0; i--) {
			Block* block = freeBlocks.get(i);

			debug("location", (uint64) location);
			debug("size", size);
			debug("block location", (uint64) block->location);

			if (location + size == block->location) { // memory is at bottom of this block
				// attempt to merge blocks
				if (alreadyMergedBlock->location + alreadyMergedBlock->size
						== block->location) {
					block->size += alreadyMergedBlock->size;
					block->location -= alreadyMergedBlock->size;

					free((void*) freeBlocks.remove(alreadyMergedBlock),
							sizeof(Block));

					break;
				}

				// grow this block downwards to fill space
				block->location -= size;
				block->size += size;

				alreadyMergedBlock = block;
			} else if (block->location + block->size == location) { // memory is at the top of this block
				// attempt to merge blocks
				if (block->location + block->size
						== alreadyMergedBlock->location) {
					block->size += alreadyMergedBlock->size;

					free((void*) freeBlocks.remove(alreadyMergedBlock),
							sizeof(Block));

					break;
				}

				block->size += size;

				alreadyMergedBlock = block;
			} else {
				// store potential size position

				if (size > block->size) {
					potentialSizeIndex = i + 1; // after this block
				}
			}
		}

		// could not align to another block, we need another block
		Block* newBlock = (Block*) malloc(sizeof(Block));
		newBlock->location = location;
		newBlock->size = size;

		freeBlocks.add(newBlock, potentialSizeIndex);
	}
}
