/*
 * memory.c
 *
 *  Created on: May 8, 2016
 *      Author: chris
 */

#include "../utils/utils.h"
#include "log.h"
#include <memory.h>
#include <list.h>
#include <errors.h>

typedef struct {
	void* location;
	size_t size;
} Block;

List<Block*> freeBlocks;

static void clear(Block* block) {
	for (uint64 i = 0; i < block->size; i++) {
		((uint8*) block->location)[i] = 0;
	}
}

void* allocateMemory(size_t size);
void free(void* location, size_t size);

void* allocateMemory(size_t size) {
	for (uint64 i = 0; i < freeBlocks.size(); i++) {
		// NOTE: after mutating memory, the index can no longer be used, use freeBlocks.indexOf(block) instead

		Block* block = freeBlocks.get(i);

		if (size == block->size) { // correct block size
			free((uint8*) freeBlocks.remove(i), sizeof(Block*));

			clear(block);
			return block->location;
		} else if (size < block->size) {
			// "split" block
			void* location = block->location;

			block->location += size;
			block->size -= size;

			if (block->size == 0) {
				// the block is empty, delete it
				free((uint8*) freeBlocks.remove(i), sizeof(Block*));
			}

			clear(block);
			return location;
		}

		// try the next block
	}

	// out of blocks
	crash(ALLOCATE_FAILED);

	return 0;
}

void free(void* location, size_t size) {
	debug("location", (uint64) location);
	debug("size", size);

	//return;

	bool didFree = false;

	if (freeBlocks.isEmpty()) {
		// we must manually allocate memory since we have no available blocks

		Block* block = (Block*) location;
		block->location = location + sizeof(Block) + sizeof(Element<Block*> );
		block->size = size - sizeof(Block) - sizeof(Element<Block*> );

		Element<Block*>* element = (Element<Block*>*) location + sizeof(Block);
		debug("element location", (uint64) element);
		debug("element value (uncleared block location value)",
				(uint64) element->value);

		element->value = block;

		debug("element value (block location)", (uint64) element->value);

		freeBlocks.add(element);

		didFree = true;
	} else {
		for (uint64 i = 0; i < freeBlocks.size() && !didFree; i++) {
			// NOTE: after mutating memory, the index can no longer be used, use freeBlocks.indexOf(block) instead

			Block* block = freeBlocks.get(i);

			if (location + size < block->location) { // create new block below
				// we need a new block
				Block* newBlock = (Block*) allocateMemory(sizeof(Block));

				newBlock->location = location;
				newBlock->size = size;
				freeBlocks.add(newBlock, freeBlocks.indexOf(block));
				didFree = true;
			} else if (location + size == block->location) { // memory is at bottom of this block
				// grow this block downwards to fill space
				block->location -= size;
				block->size += size;
				didFree = true;

				// attempt to merge blocks
				if (i > 0) {
					Block* previousBlock = freeBlocks.get(
							freeBlocks.indexOf(block) - 1);

					if (previousBlock->location + previousBlock->size
							== block->location) { // if ajacient to block on left
						// grow this block downwards
						block->size += previousBlock->size;
						block->location -= previousBlock->size;

						free((uint8*) previousBlock, sizeof(Block));
					}
				}
			} else if (block->location + block->size == location) { // memory is at the top of this block
				block->size += size;
				didFree = true;

				// attempt to merge blocks
				if (i < freeBlocks.size()) {
					Block* futureBlock = freeBlocks.get(
							freeBlocks.indexOf(block) + 1);

					if (block->location + block->size
							== futureBlock->location) { // if ajacient to block on left
						// grow this block downwards
						block->size += futureBlock->size;

						free((uint8*) futureBlock, sizeof(Block));
					}
				}
			} else if (block->location + block->size < location + size) {
				// we need a new block
				Block* newBlock = (Block*) allocateMemory(sizeof(Block));

				newBlock->location = location;
				newBlock->size = size;
				freeBlocks.add(newBlock, freeBlocks.indexOf(block) + 1);
				didFree = true;
			} else {
				// we are freeing memory that was already freed
				crash(FREE_FAILED_FREED_ALREADY);
			}
		}
	}

	if (!didFree) {
		// could not free memory
		crash(FREE_FAILED);
	}
}

void memory_init(multiboot_info_t* mbd) {
	if (!getBit(mbd->flags, 6)) {
		crash("6th bit in mbd is not set, cannot get memory map");
	}

	memory_map_t* mmap = (memory_map_t*) mbd->mmap_addr;
	debug("map location:", (uint64) mmap);
	while ((uint32) mmap < mbd->mmap_addr + mbd->mmap_length) {
		mmap = (memory_map_t*) ((unsigned int) mmap + mmap->size
				+ sizeof(unsigned int));

		if (mmap->type == 1) { // memory is useable
			free((void*) merge(mmap->base_addr_high, mmap->base_addr_low) // + 0x1b
					, merge(mmap->length_high, mmap->length_low));
		}
	}

	MemoryManager memoryManager = { &allocateMemory };
	setMemoryManager(memoryManager);
}