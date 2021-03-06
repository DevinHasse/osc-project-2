# osc-project-2
CSCI 474 Operating Systems Concepts | Group Project 2

SUMMARY:
Our program found that, when passing through 1000 randomly generated page requests 
(meaning temporal locality was not accounted for in page requests), the Clock paging 
algorithm performed comparable to the LRU and FIFO paging algorithm when there were 
very few frames, but was quickly outperformed by the two as the number of frames 
increased. LRU and FIFO were nearly indistinguishable in performance, regardless of 
the number of frames or pages. This likely would not be the case if temporal locality 
was more prevalent in the input files. Finally, unsurprisingly, the optimal paging 
algorithm significantly outperformed all other algorithms in every case. The disparity 
may have been less apparent with temporal locality.

Pass 1: 15 Pages
+--------+-------+-------+-------+-------+
| Frames |   OPT |   LRU |  FIFO | CLOCK |
+--------+-------+-------+-------+-------+
|      4 |   509 |   743 |   748 |   745 |
+--------+-------+-------+-------+-------+
|      5 |   433 |   692 |   701 |   664 |
+--------+-------+-------+-------+-------+
|      6 |   362 |   621 |   622 |   615 |
+--------+-------+-------+-------+-------+
|      7 |   299 |   554 |   538 |   540 |
+--------+-------+-------+-------+-------+
|      8 |   247 |   473 |   468 |   477 |
+--------+-------+-------+-------+-------+
|      9 |   202 |   400 |   411 |   439 |
+--------+-------+-------+-------+-------+
|     10 |   160 |   343 |   355 |   336 |
+--------+-------+-------+-------+-------+
|     11 |   125 |   277 |   260 |   311 |
+--------+-------+-------+-------+-------+
|     12 |    94 |   218 |   210 |   251 |
+--------+-------+-------+-------+-------+
|     13 |    63 |   135 |   141 |   205 |
+--------+-------+-------+-------+-------+
|     14 |    36 |    74 |    79 |   152 |
+--------+-------+-------+-------+-------+

Pass 2: 30 Pages
+--------+-------+-------+-------+-------+
| Frames |   OPT |   LRU |  FIFO | CLOCK |
+--------+-------+-------+-------+-------+
|      4 |   657 |   861 |   861 |   856 |
+--------+-------+-------+-------+-------+
|      5 |   594 |   821 |   819 |   825 |
+--------+-------+-------+-------+-------+
|      6 |   543 |   785 |   779 |   798 |
+--------+-------+-------+-------+-------+
|      7 |   493 |   746 |   745 |   770 |
+--------+-------+-------+-------+-------+
|      8 |   454 |   715 |   719 |   761 |
+--------+-------+-------+-------+-------+
|      9 |   418 |   675 |   668 |   718 |
+--------+-------+-------+-------+-------+
|     10 |   379 |   632 |   633 |   685 |
+--------+-------+-------+-------+-------+
|     11 |   350 |   597 |   601 |   678 |
+--------+-------+-------+-------+-------+
|     12 |   325 |   574 |   575 |   658 |
+--------+-------+-------+-------+-------+
|     13 |   301 |   542 |   540 |   623 |
+--------+-------+-------+-------+-------+
|     14 |   278 |   509 |   521 |   606 |
+--------+-------+-------+-------+-------+

Pass 3: 60 Pages
+--------+-------+-------+-------+-------+
| Frames |   OPT |   LRU |  FIFO | CLOCK |
+--------+-------+-------+-------+-------+
|      4 |   768 |   928 |   929 |   938 |
+--------+-------+-------+-------+-------+
|      5 |   727 |   910 |   912 |   923 |
+--------+-------+-------+-------+-------+
|      6 |   692 |   896 |   894 |   895 |
+--------+-------+-------+-------+-------+
|      7 |   661 |   880 |   876 |   891 |
+--------+-------+-------+-------+-------+
|      8 |   635 |   862 |   865 |   883 |
+--------+-------+-------+-------+-------+
|      9 |   607 |   853 |   851 |   871 |
+--------+-------+-------+-------+-------+
|     10 |   581 |   836 |   841 |   855 |
+--------+-------+-------+-------+-------+
|     11 |   554 |   826 |   827 |   852 |
+--------+-------+-------+-------+-------+
|     12 |   531 |   814 |   802 |   854 |
+--------+-------+-------+-------+-------+
|     13 |   506 |   793 |   779 |   848 |
+--------+-------+-------+-------+-------+
|     14 |   481 |   768 |   763 |   848 |
+--------+-------+-------+-------+-------+


