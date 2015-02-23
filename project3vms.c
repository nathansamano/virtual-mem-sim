/* Nathan Samano
 * December 5, 2013
 * Project 3: Virtual Memory Simulation
 * Simulate a virtual memory system, read in virtual memory address requests,
 * and write out physical memory addresses
 * using a 16-bit virtual address space, 16KB main memory, and 2KB page sizes
 */

#include <stdio.h>
#include <string.h>

#define SIZE 9                        // number of addresses in 3pages/9pages
#define DEBUG 0                       // used for debugging
#define MASK 0x07FF                   // mask used to get offset
#define VTABLESIZE 32                 // size of virtual page table
#define PTABLESIZE 8                  // size of physical page frame table
#define NIM 99                        // value given for a page not in memory

unsigned int size=0;                      // number of addresses in file
int pageFault=0;                          // count number of page faults that occurr
unsigned int vaddr=0x6842;                // virtual address
unsigned int vaddrArray[SIZE]={0};        // array of virtual address from file
unsigned int paddrArray[SIZE]={0};        // array of physical address from file
unsigned int vpage=0;                     // virtual page
unsigned int paddr=0;                     // physical address
unsigned int pframe=0;                    // page frame
unsigned int offset=0;                    // offset
int present[VTABLESIZE]={0};              // if pframe is present=0 if pframe not present=1
unsigned int pframeArray[VTABLESIZE]={0}; // array of physical page frame in virtual memory
unsigned int pframeTableArray[VTABLESIZE]={0}; // array of page frames used in table
unsigned int vpageArray[PTABLESIZE]={0};  // array of virtual pages in physical memory

char buffer[SIZE] = "";
char infilename[SIZE] = "";               // in file name string
FILE * infileptr = NULL;                  // infile pointer
char outfilename[SIZE] = "outfilep3";              // out file name string
FILE * outfileptr = NULL;                 // outfile pointer


int main(int argc, char *argv[]) {

    int i=0;  // counting variable
    int j=0;  // counting variable

//    printf("Enter file name: ");
//    scanf("%[^\n]s", infilename);         // stackoverflow help

    /* open a file with command line*/
    if (argc != 3) {  // had this at != 2 but w/ Alex's help got 3
       printf("usage: %s filename", argv[0]);
    } else {
        infileptr = fopen(argv[1], "r");
        outfileptr = fopen(argv[2], "w");
        if (infileptr == NULL || outfileptr == NULL || infileptr == 0 || outfileptr == 0) {
           printf("Error opening file\n");
           return 1;
        } // if

        /* scan every line of file */
        while(fscanf(infileptr, "%x", &vaddr) > 0) {  // Alex stackoverflow
           vaddrArray[i] = vaddr;
           i++;
           size++;
        } // while

        if (DEBUG == 0) {
           for(i=0; i<size; i++) {
              printf("vaddrArray[%d]=%X\n", i, vaddrArray[i]);
           } // for
        } // if

        /* intialize pframe array to non-zero number */
        for (i=0; i<VTABLESIZE; i++) {
           pframeTableArray[i] = NIM;
        } // for

        /* do conversion with vaddrs */
        for (i=0; i<size; i++) {
           vaddr = vaddrArray[i];

           /* get virtual page # and offset */
           vpage = vaddr>>11;
           offset = vaddr&MASK;
           printf("vpage=%X\n", vpage);
           printf("offset=%X\n", offset);

           /* check to see if page is in memory */
           if (present[vpage] == 1) {

              /* get physical page # and physical address */
              pframe = pframeTableArray[vpage];
              printf("pframe=%X\n", pframe);
              paddr = pframe<<11;
              printf("paddr w/o offset=%X\n", paddr);
              paddr = paddr | offset;
              printf("paddr=0x%X\n", paddr);
              paddrArray[i] = paddr;
              printf("paddrArray[%d]=%04X\n", i, paddrArray[i]);

           } else { /* not in memory */
               pageFault++;
               pframeTableArray[vpage] = pframe;
               vpageArray[pframe] = vpage;
               present[vpage] = 1;
               pframe = pframeTableArray[vpage];
               paddr = pframe<<11;
               paddr = paddr | offset;
               paddrArray[i] = paddr; 
           } // if else
    
           /* change pframe to be next in slot and loop around when end is reached */
           if (pframe > PTABLESIZE-1) {
              pframe = 0;
           } else {
               pframe++;
           } // if else
        } // for

        /* set up virtual page table */
        printf("\nVirtual Page Table\n\n");
        printf("\tVirtual Page Number\tPresent\t\tPage Frame\n");
        for (i=VTABLESIZE-1; i>-1; i--) {
           printf("\t\t%d\t\t%d\t\t%d\n", i, present[i], pframeTableArray[i]/*pframeArray[i]*/);   
        } // for

        /* set up physical page frame table */
        printf("\nPhysical Page Frame Table\n\n");
        printf("\tPage Frame\tVirtual Page Number\n");
        for (i=PTABLESIZE-1; i>-1; i--) {
           printf("\t%d\t\t%d\n", i, vpageArray[i]);
        } // for

        /* Report the number of addresses requested and the number of page faults that occurred */
        printf("Addresses Requested: %d\n", size);
        printf("Page Faults: %d\n", pageFault);

        /* write to file */
        for (i=0; i<size; i++) {
           sprintf(buffer, "0x%04X\n", paddrArray[i]);
           fputs(buffer, outfileptr);
        } // for

        /* print contents */
        if (DEBUG == 0) {
           for(i=0; i<size; i++) {
              printf("paddrArray[%d]=0x%04X\n", i, paddrArray[i]);
           } // for
        } // if

        /* close a file */
        fclose(infileptr);
        fclose(outfileptr);
    } // if else
} // main
