
/*
    File: frame_pool.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 12/09/03

    Description: Management of the Free-Frame Pool.


*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define MB * (0x1 << 20)
#define KB * (0x1 << 10)

#include "frame_pool.H"
#include "console.H"

unsigned char *FramePool::bitmap = 0;  
FramePool::FramePool(unsigned long _base_frame_no, unsigned long _nframes, unsigned long _info_frame_no){
	   
	   g_base_frame_no = _base_frame_no;
	   g_nframes = _nframes;
	   g_info_frame_no = _info_frame_no;
	   
	   bitmap = (unsigned char)0x200000;
	   
	   //bitmap[8192];
	   
	   //set all free (0)
	   
	   for(int i = g_base_frame_no; i < g_base_frame_no + g_nframes; i++)
			bitmap[i] = '0';
			
	   bitmap[500] = '1'; //for bitmap
	   bitmap[501] = '1'; //for bitmap
	   
	   //if(_info_frame_no != 0)
		//	for(;;){}
	   //testings
	   //for(;;){}
		
   }

   unsigned long FramePool::get_frame(){
	   int i = g_base_frame_no;
	   for(i; i < g_base_frame_no + g_nframes; i++)
	   {
		   if(bitmap[i] == '0'){
			   bitmap[i] = '1';
			   //for(;;){}
			   return i;
		   }
	   }
	   return 0;
   }

   void FramePool::mark_inaccessible(unsigned long _base_frame_no, unsigned long _nframes){
	   for(int i = _base_frame_no; i < _base_frame_no + _nframes; i++){
		   bitmap[i] = '1';
	   }
	   //for(;;){}
   }

   void FramePool::release_frame(unsigned long _frame_no){
	   bitmap[_frame_no] = '0';
   }
   
   unsigned long FramePool::get_frame_address(unsigned long _frame_no)
   {
	   return 0x400000 + ((_frame_no-g_base_frame_no) * 4096);
   }



