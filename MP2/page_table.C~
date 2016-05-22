

#define MB * (0x1 << 20)
#define KB * (0x1 << 10)

#include "page_table.H"


  PageTable     * PageTable::current_page_table; 
  unsigned int    PageTable::paging_enabled;     
  FramePool     * PageTable::kernel_mem_pool;    
  FramePool     * PageTable::process_mem_pool;   
  unsigned long   PageTable::shared_size;        

  void PageTable::init_paging(FramePool * _kernel_mem_pool, FramePool * _process_mem_pool, const unsigned long _shared_size){

	  shared_size = _shared_size;
	  kernel_mem_pool = _kernel_mem_pool;
	  process_mem_pool = _process_mem_pool;	 
  }


  PageTable::PageTable(){
	page_directory = (unsigned long *) (kernel_mem_pool->get_frame() * Machine::PAGE_SIZE);
    unsigned long* Page_Table = (unsigned long *) (kernel_mem_pool->get_frame() * Machine::PAGE_SIZE);
 

 
    // map the first 4MB of memory
    for(int i=0; i<1024; i++)
    {
	Page_Table[i] = (i* Machine::PAGE_SIZE) | 3; 
    }
 
    page_directory[0] = (unsigned long)Page_Table;   
    page_directory[0] = page_directory[0] | 3;
    
    
    for(int i = 1; i<1024; i++){
	page_directory[i] = 0 | 2; 
   }
  }

  void PageTable::load(){
 	current_page_table = this;
	write_cr3((unsigned long) page_directory);
  }

  void PageTable::enable_paging(){
	  write_cr0(read_cr0() | 0x80000000);
  }

  	

		

    void PageTable::handle_fault(REGS * _r) {
       
        unsigned long address = read_cr2();//Get the address that cuased the page fault
       
            /*check if present attribute is 0, that means a page is missing */
            if ((_r->err_code & 1) == 0) {
                    // PAGE NOT PRESENT
                    unsigned long * page_table;
                    unsigned long * page_dir = current_page_table->page_directory;
           
            /* The Higest 10 bit of the address determine the page table address in the page directory*/
                    unsigned long page_dir_index = address  >> 22;
            /* The middle 10 bit determine which entry should we map in the page table*/
                    unsigned long page_table_index = (address & (0x03FF << 12)) >> 12;
                    unsigned long * physical_address;
     
            /* Find the address of page_table in memory, which should map the missing page */
                    page_table = (unsigned long *) page_dir[page_dir_index];
           
            /* If currently there is no page table, we need to build one first*/
                    if ((page_dir[page_dir_index] & 1) == 0) {
                /* Still find some place in kernel memory to put this page_table*/
                            page_table = (unsigned long *) (kernel_mem_pool->get_frame() * PAGE_SIZE);
               
                /*Map 1024 entries in the page table to 1024 frames(totally size: 4MB) in the process memory pool*/
                    for(int i=0; i<1024; i++) {
                   
                    /* Find a free frame in process memory pool and map physical_address to this frame */
                            physical_address = (unsigned long *) (process_mem_pool->get_frame() * PAGE_SIZE);
                   
                    // attribute set to: user level, read/write, present(011 in binary)
     
                            page_table[i] = (unsigned long) (physical_address) | 3;
                    }
     
                                   
                            page_dir[page_dir_index] = (unsigned long) (page_table); // attribute set to: supervisor level, read/write, present(011 in binary)
                            page_dir[page_dir_index] |= 3;
     
                    }
            /* If there is already existing a page_table, we just map it*/
     
            /* get the physical address we need maping to*/
                            physical_address = (unsigned long *) (process_mem_pool->get_frame() * PAGE_SIZE);
            /* Map current entry in the page table to this physical_address */
               
            // attribute set to: user level, read/write, present(111 in binary)
                            page_table[page_table_index] = (unsigned long) (physical_address);
               
                           page_table[page_table_index] |= 7;
     
                   
     
     
            }      
     
    }

