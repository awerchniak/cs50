# Testing for crawler.c

## test invalid number of arguments
```
[awerchniak@flume ]$ crawler dartmouth.edu ./test 1
Please enter a valid, internal seedURL.
[awerchniak@flume ]$ crawler
Please enter three arguments: seedURL, pageDirectory, and maxDepth.
[awerchniak@flume ]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html
Please enter three arguments: seedURL, pageDirectory, and maxDepth.
[awerchniak@flume ]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./test
Please enter three arguments: seedURL, pageDirectory, and maxDepth.
[awerchniak@flume ]$ crawler 1
Please enter three arguments: seedURL, pageDirectory, and maxDepth.
[awerchniak@flume ]$ crawler ./test 1
Please enter three arguments: seedURL, pageDirectory, and maxDepth.
[awerchniak@flume ]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html 1
Please enter three arguments: seedURL, pageDirectory, and maxDepth.
```

## test on non-internal seed URL
```
[awerchniak@flume ]$ crawler dartmouth.edu ./test 1
Please enter a valid, internal seedURL.
```

## test on non-writable directory
```
[awerchniak@flume ]$ mkdir noWrite
[awerchniak@flume ]$ chmod -w noWrite
[awerchniak@flume ]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./noWrite 1
Please enter a valid, writable pageDirectory.
```

## test on a non-existant directory
```
[awerchniak@flume ]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./test2 1
Please enter a valid, writable pageDirectory.
```

## test negative max depth
```
[awerchniak@flume ]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html ./test -1
Please enter a valid, numeric maxDepth.
```

## test non numeric max depth
```
[awerchniak@flume ]$ crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./test notANumber
Please enter a valid, numeric maxDepth.
```

## test for memory leaks
```
[awerchniak@flume ]$ leakVal ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html ./test 1
==19307== Memcheck, a memory error detector
==19307== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==19307== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==19307== Command: ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html ./test 1
==19307== 
 0   Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html
 0     Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html
 0  Scanning: http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computer_science.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computer_science.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/C_(programming_language).html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/C_(programming_language).html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Unix.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Unix.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Dartmouth_College.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Dartmouth_College.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Hash_table.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Hash_table.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
 1      Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Hash_table.html
 1      Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Hash_table.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Dartmouth_College.html
 1      Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Dartmouth_College.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Unix.html
 1      Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Unix.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/C_(programming_language).html
 1      Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/C_(programming_language).html
 1    Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computer_science.html
 1      Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computer_science.html
==19307== 
==19307== HEAP SUMMARY:
==19307==     in use at exit: 64 bytes in 2 blocks
==19307==   total heap usage: 31,717 allocs, 31,715 frees, 17,994,534 bytes allocated
==19307== 
==19307== 32 bytes in 1 blocks are still reachable in loss record 1 of 2
==19307==    at 0x4C28BF6: malloc (vg_replace_malloc.c:299)
==19307==    by 0x5BA5097: CRYPTO_malloc (in /usr/lib64/libcrypto.so.1.0.2h)
==19307==    by 0x5C5A4CE: sk_new (in /usr/lib64/libcrypto.so.1.0.2h)
==19307==    by 0x590EDA9: ??? (in /usr/lib64/libssl.so.1.0.2h)
==19307==    by 0x5911238: SSL_COMP_get_compression_methods (in /usr/lib64/libssl.so.1.0.2h)
==19307==    by 0x5917448: SSL_library_init (in /usr/lib64/libssl.so.1.0.2h)
==19307==    by 0x4E87CF5: Curl_ossl_init (in /usr/lib64/libcurl.so.4.4.0)
==19307==    by 0x4E62CA4: global_init (in /usr/lib64/libcurl.so.4.4.0)
==19307==    by 0x4E62EA7: curl_easy_init (in /usr/lib64/libcurl.so.4.4.0)
==19307==    by 0x4018F8: GetWebPage (web.c:101)
==19307==    by 0x4013A1: crawl (crawler.c:85)
==19307==    by 0x4012EA: main (crawler.c:57)
==19307== 
==19307== 32 bytes in 1 blocks are still reachable in loss record 2 of 2
==19307==    at 0x4C28BF6: malloc (vg_replace_malloc.c:299)
==19307==    by 0x5BA5097: CRYPTO_malloc (in /usr/lib64/libcrypto.so.1.0.2h)
==19307==    by 0x5C5A4EC: sk_new (in /usr/lib64/libcrypto.so.1.0.2h)
==19307==    by 0x590EDA9: ??? (in /usr/lib64/libssl.so.1.0.2h)
==19307==    by 0x5911238: SSL_COMP_get_compression_methods (in /usr/lib64/libssl.so.1.0.2h)
==19307==    by 0x5917448: SSL_library_init (in /usr/lib64/libssl.so.1.0.2h)
==19307==    by 0x4E87CF5: Curl_ossl_init (in /usr/lib64/libcurl.so.4.4.0)
==19307==    by 0x4E62CA4: global_init (in /usr/lib64/libcurl.so.4.4.0)
==19307==    by 0x4E62EA7: curl_easy_init (in /usr/lib64/libcurl.so.4.4.0)
==19307==    by 0x4018F8: GetWebPage (web.c:101)
==19307==    by 0x4013A1: crawl (crawler.c:85)
==19307==    by 0x4012EA: main (crawler.c:57)
==19307== 
==19307== LEAK SUMMARY:
==19307==    definitely lost: 0 bytes in 0 blocks
==19307==    indirectly lost: 0 bytes in 0 blocks
==19307==      possibly lost: 0 bytes in 0 blocks
==19307==    still reachable: 64 bytes in 2 blocks
==19307==         suppressed: 0 bytes in 0 blocks
==19307== 
==19307== For counts of detected and suppressed errors, rerun with: -v
==19307== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
[awerchniak@flume ]$ 
```
NOTE: 64 bytes still reachable, because curl_easy_init() does not work perfectly with the
	curl_ _cleanup() functions

## test various depths:
      http://old-www.cs.dartmouth.edu/~cs50/index.html:
      	Depth 0: 1
      	Depth 1: 10
      	Depth 2: 41
      	Depth 3: 59
      http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html
      	Depth 0: 1
      	Depth 1: 7
      	Depth 2: 1705

## test decent depth with larger webpage
```
[awerchniak@flume ]$ crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html ./test 2
 0   Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html
 0     Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html
 0  Scanning: http://old-www.cs.dartmouth.edu/~cs50/data/tse/index.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computer_science.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computer_science.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/C_(programming_language).html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/C_(programming_language).html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Unix.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Unix.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Dartmouth_College.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Dartmouth_College.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Hash_table.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Hash_table.html
 0     Found: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
 0     Added: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
 1    Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
 1      Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
 1   Scanning: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
 1   IgnExtrn: http://en.wikipedia.org/wiki/File:Question_book-new.svg
 1   IgnExtrn: http://en.wikipedia.org/wiki/Wikipedia:Verifiability
... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ... ...
 2       Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computational_problem.html
 2     Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computational_complexity_theory.html
 2       Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computational_complexity_theory.html
 2     Fetched: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computation.html
 2       Saved: http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Computation.html
[awerchniak@flume ]$ ls -v ./test
1   67   133  199  265  331  397  463  529  595  661  727  793  859  925  991   1057  1123  1189  1255  1321  1387  1453  1519  1585  1651
2   68   134  200  266  332  398  464  530  596  662  728  794  860  926  992   1058  1124  1190  1256  1322  1388  1454  1520  1586  1652
3   69   135  201  267  333  399  465  531  597  663  729  795  861  927  993   1059  1125  1191  1257  1323  1389  1455  1521  1587  1653
4   70   136  202  268  334  400  466  532  598  664  730  796  862  928  994   1060  1126  1192  1258  1324  1390  1456  1522  1588  1654
5   71   137  203  269  335  401  467  533  599  665  731  797  863  929  995   1061  1127  1193  1259  1325  1391  1457  1523  1589  1655
6   72   138  204  270  336  402  468  534  600  666  732  798  864  930  996   1062  1128  1194  1260  1326  1392  1458  1524  1590  1656
7   73   139  205  271  337  403  469  535  601  667  733  799  865  931  997   1063  1129  1195  1261  1327  1393  1459  1525  1591  1657
8   74   140  206  272  338  404  470  536  602  668  734  800  866  932  998   1064  1130  1196  1262  1328  1394  1460  1526  1592  1658
9   75   141  207  273  339  405  471  537  603  669  735  801  867  933  999   1065  1131  1197  1263  1329  1395  1461  1527  1593  1659
10  76   142  208  274  340  406  472  538  604  670  736  802  868  934  1000  1066  1132  1198  1264  1330  1396  1462  1528  1594  1660
11  77   143  209  275  341  407  473  539  605  671  737  803  869  935  1001  1067  1133  1199  1265  1331  1397  1463  1529  1595  1661
12  78   144  210  276  342  408  474  540  606  672  738  804  870  936  1002  1068  1134  1200  1266  1332  1398  1464  1530  1596  1662
13  79   145  211  277  343  409  475  541  607  673  739  805  871  937  1003  1069  1135  1201  1267  1333  1399  1465  1531  1597  1663
14  80   146  212  278  344  410  476  542  608  674  740  806  872  938  1004  1070  1136  1202  1268  1334  1400  1466  1532  1598  1664
15  81   147  213  279  345  411  477  543  609  675  741  807  873  939  1005  1071  1137  1203  1269  1335  1401  1467  1533  1599  1665
16  82   148  214  280  346  412  478  544  610  676  742  808  874  940  1006  1072  1138  1204  1270  1336  1402  1468  1534  1600  1666
17  83   149  215  281  347  413  479  545  611  677  743  809  875  941  1007  1073  1139  1205  1271  1337  1403  1469  1535  1601  1667
18  84   150  216  282  348  414  480  546  612  678  744  810  876  942  1008  1074  1140  1206  1272  1338  1404  1470  1536  1602  1668
19  85   151  217  283  349  415  481  547  613  679  745  811  877  943  1009  1075  1141  1207  1273  1339  1405  1471  1537  1603  1669
20  86   152  218  284  350  416  482  548  614  680  746  812  878  944  1010  1076  1142  1208  1274  1340  1406  1472  1538  1604  1670
21  87   153  219  285  351  417  483  549  615  681  747  813  879  945  1011  1077  1143  1209  1275  1341  1407  1473  1539  1605  1671
22  88   154  220  286  352  418  484  550  616  682  748  814  880  946  1012  1078  1144  1210  1276  1342  1408  1474  1540  1606  1672
23  89   155  221  287  353  419  485  551  617  683  749  815  881  947  1013  1079  1145  1211  1277  1343  1409  1475  1541  1607  1673
24  90   156  222  288  354  420  486  552  618  684  750  816  882  948  1014  1080  1146  1212  1278  1344  1410  1476  1542  1608  1674
25  91   157  223  289  355  421  487  553  619  685  751  817  883  949  1015  1081  1147  1213  1279  1345  1411  1477  1543  1609  1675
26  92   158  224  290  356  422  488  554  620  686  752  818  884  950  1016  1082  1148  1214  1280  1346  1412  1478  1544  1610  1676
27  93   159  225  291  357  423  489  555  621  687  753  819  885  951  1017  1083  1149  1215  1281  1347  1413  1479  1545  1611  1677
28  94   160  226  292  358  424  490  556  622  688  754  820  886  952  1018  1084  1150  1216  1282  1348  1414  1480  1546  1612  1678
29  95   161  227  293  359  425  491  557  623  689  755  821  887  953  1019  1085  1151  1217  1283  1349  1415  1481  1547  1613  1679
30  96   162  228  294  360  426  492  558  624  690  756  822  888  954  1020  1086  1152  1218  1284  1350  1416  1482  1548  1614  1680
31  97   163  229  295  361  427  493  559  625  691  757  823  889  955  1021  1087  1153  1219  1285  1351  1417  1483  1549  1615  1681
32  98   164  230  296  362  428  494  560  626  692  758  824  890  956  1022  1088  1154  1220  1286  1352  1418  1484  1550  1616  1682
33  99   165  231  297  363  429  495  561  627  693  759  825  891  957  1023  1089  1155  1221  1287  1353  1419  1485  1551  1617  1683
34  100  166  232  298  364  430  496  562  628  694  760  826  892  958  1024  1090  1156  1222  1288  1354  1420  1486  1552  1618  1684
35  101  167  233  299  365  431  497  563  629  695  761  827  893  959  1025  1091  1157  1223  1289  1355  1421  1487  1553  1619  1685
36  102  168  234  300  366  432  498  564  630  696  762  828  894  960  1026  1092  1158  1224  1290  1356  1422  1488  1554  1620  1686
37  103  169  235  301  367  433  499  565  631  697  763  829  895  961  1027  1093  1159  1225  1291  1357  1423  1489  1555  1621  1687
38  104  170  236  302  368  434  500  566  632  698  764  830  896  962  1028  1094  1160  1226  1292  1358  1424  1490  1556  1622  1688
39  105  171  237  303  369  435  501  567  633  699  765  831  897  963  1029  1095  1161  1227  1293  1359  1425  1491  1557  1623  1689
40  106  172  238  304  370  436  502  568  634  700  766  832  898  964  1030  1096  1162  1228  1294  1360  1426  1492  1558  1624  1690
41  107  173  239  305  371  437  503  569  635  701  767  833  899  965  1031  1097  1163  1229  1295  1361  1427  1493  1559  1625  1691
42  108  174  240  306  372  438  504  570  636  702  768  834  900  966  1032  1098  1164  1230  1296  1362  1428  1494  1560  1626  1692
43  109  175  241  307  373  439  505  571  637  703  769  835  901  967  1033  1099  1165  1231  1297  1363  1429  1495  1561  1627  1693
44  110  176  242  308  374  440  506  572  638  704  770  836  902  968  1034  1100  1166  1232  1298  1364  1430  1496  1562  1628  1694
45  111  177  243  309  375  441  507  573  639  705  771  837  903  969  1035  1101  1167  1233  1299  1365  1431  1497  1563  1629  1695
46  112  178  244  310  376  442  508  574  640  706  772  838  904  970  1036  1102  1168  1234  1300  1366  1432  1498  1564  1630  1696
47  113  179  245  311  377  443  509  575  641  707  773  839  905  971  1037  1103  1169  1235  1301  1367  1433  1499  1565  1631  1697
48  114  180  246  312  378  444  510  576  642  708  774  840  906  972  1038  1104  1170  1236  1302  1368  1434  1500  1566  1632  1698
49  115  181  247  313  379  445  511  577  643  709  775  841  907  973  1039  1105  1171  1237  1303  1369  1435  1501  1567  1633  1699
50  116  182  248  314  380  446  512  578  644  710  776  842  908  974  1040  1106  1172  1238  1304  1370  1436  1502  1568  1634  1700
51  117  183  249  315  381  447  513  579  645  711  777  843  909  975  1041  1107  1173  1239  1305  1371  1437  1503  1569  1635  1701
52  118  184  250  316  382  448  514  580  646  712  778  844  910  976  1042  1108  1174  1240  1306  1372  1438  1504  1570  1636  1702
53  119  185  251  317  383  449  515  581  647  713  779  845  911  977  1043  1109  1175  1241  1307  1373  1439  1505  1571  1637  1703
54  120  186  252  318  384  450  516  582  648  714  780  846  912  978  1044  1110  1176  1242  1308  1374  1440  1506  1572  1638  1704
55  121  187  253  319  385  451  517  583  649  715  781  847  913  979  1045  1111  1177  1243  1309  1375  1441  1507  1573  1639  1705
56  122  188  254  320  386  452  518  584  650  716  782  848  914  980  1046  1112  1178  1244  1310  1376  1442  1508  1574  1640
57  123  189  255  321  387  453  519  585  651  717  783  849  915  981  1047  1113  1179  1245  1311  1377  1443  1509  1575  1641
58  124  190  256  322  388  454  520  586  652  718  784  850  916  982  1048  1114  1180  1246  1312  1378  1444  1510  1576  1642
59  125  191  257  323  389  455  521  587  653  719  785  851  917  983  1049  1115  1181  1247  1313  1379  1445  1511  1577  1643
60  126  192  258  324  390  456  522  588  654  720  786  852  918  984  1050  1116  1182  1248  1314  1380  1446  1512  1578  1644
61  127  193  259  325  391  457  523  589  655  721  787  853  919  985  1051  1117  1183  1249  1315  1381  1447  1513  1579  1645
62  128  194  260  326  392  458  524  590  656  722  788  854  920  986  1052  1118  1184  1250  1316  1382  1448  1514  1580  1646
63  129  195  261  327  393  459  525  591  657  723  789  855  921  987  1053  1119  1185  1251  1317  1383  1449  1515  1581  1647
64  130  196  262  328  394  460  526  592  658  724  790  856  922  988  1054  1120  1186  1252  1318  1384  1450  1516  1582  1648
65  131  197  263  329  395  461  527  593  659  725  791  857  923  989  1055  1121  1187  1253  1319  1385  1451  1517  1583  1649
66  132  198  264  330  396  462  528  594  660  726  792  858  924  990  1056  1122  1188  1254  1320  1386  1452  1518  1584  1650
[awerchniak@flume ]$ head -3 ./test/1231
http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Operator_(programming).html
2
<!DOCTYPE html>
[awerchniak@flume ]$ head -3 ./test/20
http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/B-tree.html
2
<!DOCTYPE html>
[awerchniak@flume ]$ head -3 ./test/2
http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Linked_list.html
1
<!DOCTYPE html>
[awerchniak@flume ]$ head -3 ./test/360
http://old-www.cs.dartmouth.edu/~cs50/data/tse/wiki/Helsinki.html
2
<!DOCTYPE html>
```
 
