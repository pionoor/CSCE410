

  #define MB * (0x1 << 20)
  #define KB * (0x1 << 10)

  #include "page_table.H"


    PageTable     * PageTable::current_page_table; 
    unsigned int    PageTable::paging_enabled;     
    FramePool     * PageTable::kernel_mem_pool;    
    FramePool     * PageTable::process_mem_pool;   
    unsigned long   PageTable::shared_size;       
    
    //unsigned int PageTable :: vm_pool = 0; 

    void PageTable::init_paging(FramePool * _kernel_mem_pool, FramePool * _process_mem_pool, const unsigned long _shared_size){

  	  kernel_mem_pool = _kernel_mem_pool;
  	  process_mem_pool = _process_mem_pool;
  	  shared_size = _shared_size;
  	  

    }
    /* Set the global parameters for the paging subsystem. */

    PageTable::PageTable(){
  	  pageDirectory = (unsigned long *) (kernel_mem_pool->get_frame() * 4096);
      unsigned long* pageTable = (unsigned long *) (kernel_mem_pool->get_frame() * 4096);
   
   
      // map the first 4MB of memory
      for(int i = 0; i<1024; i++)
      {

  		pageTable[i] = (i * Machine::PAGE_SIZE) | 3; 

      }
   
      pageDirectory[0] = (unsigned long)pageTable; 
      
      pageDirectory[0] = pageDirectory[0] | 3;
      
      
      for(int i = 1; i<1024; i++){   
       pageDirectory[i] = 0 | 2; 
      }

  		 
   
    }

    void PageTable::load(){
  	  current_page_table = this;
  	  write_cr3((unsigned long) pageDirectory);
    }

    void PageTable::enable_paging(){
  	  write_cr0(read_cr0() | 0x80000000);
    }

    // get the page number and release the corresponding frame
    void PageTable::free_page(unsigned long _page_no)
    {

  	  int PageDirIndex = _page_no >> 22;
  	  int PageTableIndex = (_page_no >> 12) & 0x03FF;
  	  unsigned long* VPageTable = (unsigned long*)(0xFFC00000 | (PageDirIndex << 12));
  	  process_mem_pool->release_frame(VPageTable[PageTableIndex]);
    }
    
  	void PageTable::register_vmpool(VMPool* _pool)
  	{
      unsigned int vmPool = 0;
  		unsigned char * vmBitmap = (unsigned char *)kernel_mem_pool->get_frame();
  		VMPool** registered_pools = (VMPool**)vmBitmap;
  		registered_pools[vmPool] = _pool;
  		vmPool++;
  	}



  		

    void PageTable::handle_fault(REGS * _r) 
    {
         unsigned long address = read_cr2();
         
         if ((_r->err_code & 1) == 0) 
         {

             unsigned long index;
             unsigned long table;
             unsigned long* page_table;
  		   unsigned long* addr;
             unsigned long* page_dir;
             
             index = address  >> 22;
             table = (address & (0x03FF << 12)) >> 12;
             page_dir = current_page_table->pageDirectory;
             page_table = (unsigned long *)page_dir[index];
              
             if ((page_dir[index] & 1) == 0) 
             {
  			   page_table = (unsigned long *)(kernel_mem_pool->get_frame() * 4096);
  			   
                 for(int i=0; i<1024; i++) 
                 {
  				   addr = (unsigned long *)(process_mem_pool->get_frame() * 4096);
                     page_table[i] = (unsigned long)(addr) | 3;
                 }
       
                                     
                 page_dir[index] = (unsigned long)(page_table);
                 page_dir[index] |= 3;
       
             }
              
             addr = (unsigned long *)(process_mem_pool->get_frame() * 4096);
              
             page_table[table] = (unsigned long)(addr);
                 
             page_table[table] |= 7;
       
                     
       
       }     
       
   }

    


