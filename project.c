/*
Name: Patrick Carnevale
SID: 110009428
Date: 2022/08/12
Desc: This program will translate logical to physical addresses for a virtual address space of 216 = 65,536 bytes. It will read from a file containing logical addresses and, 
using a page table, will translate each logical address to its corresponding physical address and output both.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

const int TS=16; //table size
const int VM_SIZE=256; 
const int PS=256; //page size
const int MM_SIZE=256; 

int main(int argc, char* argv[])
{
    FILE *fd; //reads file
    fd=fopen("addresses.txt","r"); //accesses and opens file so it can be read by the program
    if(fd==NULL) //if error
    {
        printf("Something went wrong, make sure your file path/name is correct."); //if file can't be read, alert user
        return 0; //exits program
    }
    int f;
    int l;
    int q = 0;
    int p = 0;
    char *value = NULL;
    size_t len = 0;
    size_t read;
    long long page_n, offset, th = 0, ft = 0, pages = 0;
    int tlb[TS][2];
    int pt[PS];
    memset(tlb, -1, TS*2*sizeof(tlb[0][0]));
    memset(pt, -1, sizeof(pt));
    int mask=255, mask1=62580, i, hit;
    while((read=getline(&value, &len, fd)) != -1) //Creates a loop that prints out the converted logical to physical addresses for a virtual address space of size 216 = 65,536 bytes
    {
        pages++;
        page_n = atoi(value);
        page_n = page_n >> 8;
        page_n = page_n & mask;
        offset = atoi(value);
        offset = offset & mask;
        l = atoi(value);
        f = 0, p = 0;
        hit = 0;
        for(i=0; i<TS; i++)
        {
            if(tlb[i][0]==page_n)
            {
                hit = 1;
                th++;
                f = tlb[i][1];
                break;
            }
        }
        if(hit==1) //alerts user that a TLB has been hit
        printf("Translation lookaside buffer hit\n");
        else //if a TLB is not hit, converts logical to phisical
        {
            int f = 0;
            for(i=0; i<PS; i++)
            {
                if(pt[i]==page_n)
                {
                    f = i;
                    ft++;
                    break;
                }
                if(pt[i]==-1)
                {
                    f = 1;
                    break;
                }
            }
            if(f==1)
            {
                pt[i] = page_n;
                f = i;
            }
            tlb[q][0] = page_n;
            tlb[q][1] = i;
            q++;
            q = q%15;
        }
        if(l<10000) //display's virtual address
            printf("Virtual address = %d \t\t\t",l);
        else
            printf("Virtual address = %d \t\t",l);
        p = f*PS + offset;
        printf("Physical address = %d\n",p); //display's physical address
    } //ends loop
    double hr = (double)th/pages*100; //calculates the hitrate
    double fr = (double)ft/pages*100; //calculates the faultrate
    printf("\nTranslation lookaside buffer hit rate = %.2f %c", hr, '%'); //Display's the rate of both hit's and miss's for page table's and TLB's
    printf("\nTranslation lookaside buffer miss rate = %.2f %c", (100-hr), '%');
    printf("\nPage table hit rate = %.2f %c", fr, '%');
    printf("\nPage table miss rate = %.2f %c\n", (100-fr), '%');
}