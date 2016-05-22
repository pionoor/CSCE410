/** Name: vm_pool.C
    Author: Pranami Bhattacharya
    Virtual memory pool management
  

   allocate:
   scan through the vm pool
   find region with valid bit unset
   find the number of pages to be assigned
   allocate for the required request
   set the valid bit

   deallocate:
   find the start addr
   remove frame from start_addr+size
   unset the valid bit

**/

#include "vm_pool.H"
#include "page_table.H"
#include "console.H"
#include "utils.H"


VMPool::VMPool(unsigned long _base_address, unsigned long _size, FramePool* _frame_pool, PageTable* _page_table)
{
	count = 0;
	baseAddr = _base_address;
	size = _size;
	framePool = _frame_pool;
	pageTable = _page_table;

	_page_table->register_vmpool(this);
	poolCapacity =  Machine::PAGE_SIZE / (sizeof(vmpool));
	pool = (vmpool *)(framePool->get_frame_address(framePool->get_frame()));	
	

}


unsigned long VMPool::allocate(unsigned long _size)
{
	
	if(_size == 0)
	{
		Console::puts("\n 0 size req");
		return 0;
	}
	unsigned long start_address = 0;

	// compute start addr for the requested block
	if(count == 0)
		start_address = baseAddr;
	else
		// start address is the end address of the prev block
		start_address = pool[count-1].start_address + pool[count-1].size;


	pool[count].start_address = start_address;
	pool[count].size = _size;
	// mark as valid
	pool[count].valid = 1;
	// inc the region count
	count++;

	return start_address;
}


void VMPool::release(unsigned long _start_address)
{
	//find the region to be released from the start addr
	int i = 0;
	for(; i < count; i++)
	{
		if(pool[i].start_address == _start_address)
			break;
	}

	// release the frame
	#if 1
	// i wil be the index to the block where a match has been found
	for (int j = 0; j < pool[i].size/ Machine::PAGE_SIZE; j++)
	{
		pageTable->free_page(_start_address);
		_start_address = _start_address +  Machine::PAGE_SIZE;
	}
	pool[i].valid = 0;
	#endif
	pageTable->load();
}


BOOLEAN VMPool::is_legitimate(unsigned long _address)
{
	for(int i = 0; i < poolCapacity; i++)
	{
		if((pool[i].start_address <= _address) &&
				(_address < (pool[i].start_address + pool[i].size)))
		  
			return true;
	}

	return false;
}
