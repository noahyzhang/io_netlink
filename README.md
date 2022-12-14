获取 linux 下所有进程/线程的 IO 信息的一个 demo，通过 netlink 的方式

编译

```shell
# 在项目根目录下
mkdir build
cd build
cmake ..
make 
```

运行
由于线程过多，因此如下只展示一部分

```shell
./io_netlink
time: 2022-10-08-15.03.23
     pid     tid          read_bytes         write_bytes  swapin_delay_total   blkio_delay_total
       1       1            18067456                   0                   0           215175403
    ......
    ......
    ......
    1022    1077                   0                   0                   0                   0
    1024    1024             1937408                   0                   0            37465814
    1029    1029                   0                   0                   0                   0
    1053    1053            14864384               16384                   0           213080852
    1053    1307                4096              491520                   0            24741353
    1053    1324                   0                   0                   0                   0
    1053    1325                4096              540672                   0             4710835
    1053    1328              282624              163840                   0            13348488
    1053    1387                   0                   0                   0                   0
    1053    1388                   0                   0                   0                   0
    1053    1389               90112               49152                   0             4659399
    1053    1390                   0                   0                   0                   0
    1053   13056                   0                   0                   0                   0
    1053   13057                   0                   0                   0                   0
    1078    1078             6950912               20480              773948           233485116
    1078    1267                   0                   0                   0                   0
    1078    1276                   0                   0                   0                   0
    1079    1079             3510272                   0                   0            80084211
    1099    1099            36523008                   0                   0           312786080
    1099    1515                   0                   0                   0                   0
    1099    1516              422912           122187776                   0           353707770
    1099    1517              169984           163688448                   0           283556937
    1099    1518                   0                   0                   0                   0
    1099    1519                   0                   0                   0                   0
    1099    1551              134144           168292352                   0           328193399
    1099    1565              198656            64872448                   0           456353779
    1099    1566              110592           155283456                   0           412829950
    1099    1567               36864           147255296                   0           381090994
    1099    1568               15360            55697408                   0           333790509
    1099    1570              417792            50106368                   0           461506199
    1099    1571              204800            65716224                   0           442744246
    1099    1586              135168            37113856                   0           377498025
    1099    1721              271360            41852928                   0           337896643
    1099    1748                8192           123453440                   0           396543975
    1099    1765              976896            81137664                   0           307463049
    1099    4034               26624           155299840                   0           325150830
    1099    4035                   0            92966912                   0           156865730
    1099    9909                9216             5591040                   0           279613436
    1099   23314                   0           112791552                   0           358044182
    1099   23331              266240           142983168                   0           326995182
    1117    1195             2547712            57315328                   0            27858711
    1117    1196              212992            53002240                   0            18718959
    1117    1197                   0            52645888                   0            19748177
    1117    1204                   0                   0                   0                   0
    1117    1278              413696            45871104              368490             9099982
    1117    1297           102981632            49238016                   0           193137073
    1117    1305                   0            54161408                   0            21937659
    1117    1306            34586624            37969920                   0           119122195
    1117    1310               40960            61845504                   0            13838825
    1117    1311              266240            33120256                   0            14378088
    1117    1313               16384            60313600                   0            19049442
    1117    1314                4096            22089728              114624            11607675
    1117    1319            15241216            63119360                   0            57442482
    1117    1321              253952            54120448                   0            20540859
    1117    1322                   0            34422784                   0            18953631
    1117    3151              667648            56971264                   0            33725011
    1117    3152             8695808            58028032                   0            61386116
    1117    3153                   0                   0                   0                   0
    1117    3192              548864            56844288                   0            41389210
    1117    3193            47136768            52756480                   0           122637813
    1117    3194               73728            62955520                   0            22341874
    1117    3195                   0                   0                   0                   0
    1117    3306            27062272            68894720                   0            43838811
    1117    3307            55021568            29302784                   0           129325374
    1117    3317               20480            50900992              274181            19989659
    1117    8222                4096            58941440                   0            20452177
    1117   13215                   0            64970752                   0             8995230
    1117   24001                   0            53272576                   0            26878636
    1117   16473                4096            52215808                   0             7682924
    1119    1119           278425600            12193792            15023506           907784721
```