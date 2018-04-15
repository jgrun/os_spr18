/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */

#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) {

    /* This file contains the stub for an LRU pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    /* Local vars */
    int proctmp;
    int pagetmp;

    /* initialize static vars on first run */
    if(!initialized){
    	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
    	    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
    		timestamps[proctmp][pagetmp] = 0;
    	    }
    	}
	    initialized = 1;
    }

    /* TODO: Implement LRU Paging */

    int pc;
    int page;

    for(proctmp = 0; proctmp < MAXPROCESSES; proctmp++) {
      // update time stamp of all swapped in pages
      for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
        if(q[proctmp].pages[pagetmp]) {
          timestamps[proctmp][pagetmp] += 1;
        }
      }
      // Is process active?
      if(q[proctmp].active) {
        pc = q[proctmp].pc;
        page = pc/PAGESIZE;

        if(!q[proctmp].pages[page]) {
          if(!pagein(proctmp, page)) {
            // if swapping fails, find new page to swap in
            int lruPage;
            int longestTime = 0;

            // find least recently used page
            for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
              // only check time stamps of pages that are swapped in
              if(q[proctmp].pages[pagetmp]) {
                // make sure the page isn't the one i want
                if(q[proctmp].pages[pagetmp] != page) {
                  if(timestamps[proctmp][pagetmp] > longestTime) {
                    // update longest time so far
                    longestTime = timestamps[proctmp][pagetmp];
                    // update lru page
                    lruPage = pagetmp;
                  }
                }
              }
            }
            pageout(proctmp, lruPage);

            //pagein(proctmp, page);
            //timestamps[proctmp][page] = 0;


          }
          // If page is in, reset the timestamp for it
          else {
            timestamps[proctmp][page] = 0;
            break;
          }
        }
      }
    }

    /* advance time for next pageit iteration */
    tick++;
}
