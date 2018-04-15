/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

int isinlist(int list[MAXPROCESSES][MAXPROCPAGES], int proc, int page);

void pageit(Pentry q[MAXPROCESSES]) {

    /* This file contains the stub for a predictive pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int initialized = 0;
    static int tick = 1; // artificial time

    static int t1[MAXPROCESSES][MAXPROCPAGES];
    static int t2[MAXPROCESSES][MAXPROCPAGES];
    static int b1[MAXPROCESSES][MAXPROCPAGES];
    static int b2[MAXPROCESSES][MAXPROCPAGES];
    static int st1;
    static int st2;
    static int sb1;
    static int sb2;
    static int mt1;
    static int mt2;
    static int mb1;
    static int mb2;
    static int timestamps[MAXPROCESSES][MAXPROCPAGES];

    /* Local vars */

    int proctmp;
    int pagetmp;
    int pc;
    int page;

    /* initialize static vars on first run */
    if(!initialized){
    	/* Init complex static vars here */

      // Zero out age variable
    	for(proctmp =0; proctmp < MAXPROCESSES; proctmp++) {
        for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
          t1[proctmp][pagetmp] = 0;
          t2[proctmp][pagetmp] = 0;
          b1[proctmp][pagetmp] = 0;
          b2[proctmp][pagetmp] = 0;
          timestamps[proctmp][pagetmp] = 0;
        }
      }
      st1 = 0;
      st2 = 0;
      sb1 = 0;
      sb2 = 0;
      mt1 = 10;
      mt2 = 10;
      mb1 = 10;
      mb2 = 10;
    	initialized = 1;
    }

    /* TODO: Implement Predictive Paging */

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

            // ARC case 2
            if(isinlist(b1, proctmp, page)) {
              if(st2 < mt2) {
                b1[proctmp][page] = 0;
                sb1--;
                t2[proctmp][page] = 1;
                st2++;
              }
              else {
                int lrupage;
                int longest = 0;
                for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
                  if(isinlist(t2, proctmp, pagetmp)) {
                    if(timestamps[proctmp][pagetmp] > longest) {
                      longest = timestamps[proctmp][pagetmp];
                      lrupage = pagetmp;
                    }
                  }
                }
                if(sb2 < mb2) {
                  b2[proctmp][lrupage] = 1;
                  sb2++;
                  t2[proctmp][lrupage] = 0;
                  pageout(proctmp, lrupage);
                  t2[proctmp][page] = 1;
                  timestamps[proctmp][page] = 0;
                }
                else {
                  int lrupage2;
                  for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
                    if(isinlist(b2, proctmp, pagetmp)) {
                      if(timestamps[proctmp][pagetmp] > longest) {
                        longest = timestamps[proctmp][pagetmp];
                        lrupage2 = pagetmp;
                      }
                    }
                  }
                  b2[proctmp][lrupage2] = 0;
                  b2[proctmp][lrupage] = 1;
                  pageout(proctmp, lrupage);
                  t2[proctmp][page] = 1;
                  t2[proctmp][lrupage] = 0;
                  timestamps[proctmp][page] = 0;
                }
              }
            }

            else if(isinlist(b2, proctmp, page)) {
              if(st2 < mt2) {
                b2[proctmp][page] = 0;
                sb2--;
                t2[proctmp][page] = 1;
                st2++;
              }
              else {
                int lrupage;
                int longest = 0;
                for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
                  if(isinlist(t2, proctmp, pagetmp)) {
                    if(timestamps[proctmp][pagetmp] > longest) {
                      longest = timestamps[proctmp][pagetmp];
                      lrupage = pagetmp;
                    }
                  }
                }
                if(sb2 < mb2) {
                  b2[proctmp][lrupage] = 1;
                  sb2++;
                  t2[proctmp][lrupage] = 0;
                  pageout(proctmp, lrupage);
                  t2[proctmp][page] = 1;
                  timestamps[proctmp][page] = 0;
                }
                else {
                  int lrupage2;
                  for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
                    if(isinlist(b2, proctmp, pagetmp)) {
                      if(timestamps[proctmp][pagetmp] > longest) {
                        longest = timestamps[proctmp][pagetmp];
                        lrupage2 = pagetmp;
                      }
                    }
                  }
                  b2[proctmp][lrupage2] = 0;
                  b2[proctmp][lrupage] = 1;
                  pageout(proctmp, lrupage);
                  t2[proctmp][page] = 1;
                  t2[proctmp][lrupage] = 0;
                  timestamps[proctmp][page] = 0;
                }
              }
            }

            else {
              if(st1 < mt1) {
                t1[proctmp][page] = 1;
                st1++;
              }
              else {
                int lrupage;
                int longest = 0;
                for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
                  if(isinlist(t1, proctmp, pagetmp)) {
                    if(timestamps[proctmp][pagetmp] > longest) {
                      longest = timestamps[proctmp][pagetmp];
                      lrupage = pagetmp;
                    }
                  }
                }
                if(sb1 < mb1) {
                  b1[proctmp][lrupage] = 1;
                  sb1++;
                  t1[proctmp][lrupage] = 0;
                  pageout(proctmp, lrupage);
                  t1[proctmp][page] = 1;
                  timestamps[proctmp][page] = 0;
                }
                else {
                  int lrupage2;
                  for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
                    if(isinlist(b1, proctmp, pagetmp)) {
                      if(timestamps[proctmp][pagetmp] > longest) {
                        longest = timestamps[proctmp][pagetmp];
                        lrupage2 = pagetmp;
                      }
                    }
                  }
                  b1[proctmp][lrupage2] = 0;
                  b1[proctmp][lrupage] = 1;
                  pageout(proctmp, lrupage);
                  t1[proctmp][page] = 1;
                  t1[proctmp][lrupage] = 0;
                  timestamps[proctmp][page] = 0;
                }
              }
            }


          }

          // is page already in?
          else {
            // ARC case 1
            if(isinlist(t1, proctmp, page) || isinlist(t2, proctmp, page)) {
              if(st2 < mt2) {
                t2[proctmp][page] = 1;
                st2++;
                t1[proctmp][page] = 0;
                st1--;
                timestamps[proctmp][page] = 0;
              }
              else {
                int lrupage;
                int longest = 0;
                for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
                  if(isinlist(t2, proctmp, pagetmp)) {
                    if(timestamps[proctmp][pagetmp] > longest) {
                      longest = timestamps[proctmp][pagetmp];
                      lrupage = pagetmp;
                    }
                  }
                }
                if(sb2 < mb2) {
                  b2[proctmp][lrupage] = 1;
                  sb2++;
                  t2[proctmp][lrupage] = 0;
                  pageout(proctmp, lrupage);
                  t2[proctmp][page] = 1;
                  timestamps[proctmp][page] = 0;
                }
                else {
                  int lrupage2;
                  for(pagetmp = 0; pagetmp < MAXPROCPAGES; pagetmp++) {
                    if(isinlist(b2, proctmp, pagetmp)) {
                      if(timestamps[proctmp][pagetmp] > longest) {
                        longest = timestamps[proctmp][pagetmp];
                        lrupage2 = pagetmp;
                      }
                    }
                  }
                  b2[proctmp][lrupage2] = 0;
                  b2[proctmp][lrupage] = 1;
                  pageout(proctmp, lrupage);
                  t2[proctmp][page] = 1;
                  t2[proctmp][lrupage] = 0;
                  timestamps[proctmp][page] = 0;
                }
              }
            }


            break;
          }
        }
      }
    }

    /* advance time for next pageit iteration */
    tick++;
}

int isinlist(int list[MAXPROCESSES][MAXPROCPAGES], int proc, int page) {
  if(list[proc][page]) {
    return 1;
  }
  else return 0;
}
