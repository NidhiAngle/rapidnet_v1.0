/****************
*
* Online compression with distributed hash list,
* with auxiliary tables maintaining rule and tuple information
*
*****************/

/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(link,infinity,infinity,keys(2)). /*Links between nodes*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Routing information between nodes*/
materialize(programID,infinity,infinity,keys(2)). /*Allow the user to specify program ID*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(2:cid)).
materialize(provLink, infinity, infinity, keys(2:cid,3:int32,4,5:cid,6:int32,7:cid)).
materialize(equiHashTable,infinity,infinity,keys(2)). /*Hash table for equivalence detection*/
materialize(provHashTable,infinity,infinity,keys(2:cid,3:str)). /*Hash table for provenance query*/
materialize(provRef, infinity, infinity, keys(2:cid))./*Reference to the provenance tree*/

/* Tuple recording tables*/
materialize(provStr, infinity, infinity, keys(2:cid)).

/* Rule recording tables*/
materialize(recordRule, infinity, infinity, keys(1)).
materialize(ruleInfo, infinity, infinity, keys(2:str)).

/* Provenance querying tables*/
materialize(provResult, infinity, infinity, keys(2:cid)).
materialize(slowChangeQuery, infinity, infinity, keys(2:cid)).


/* 'ID' is a keyword. Parse error on using id for type!
The workaround is to call it cid or chord id. */

/* Record rule information */
rr01 ruleInfo(@Node, RName, RHead, RBody) :-
 recordRule(@Node),
 RName := "rh1",
 RHead := "packet(Next,SrcAdd,DstAdd,Data)",
 RBody := "initPacket(Node,SrcAdd,DstAdd,Data);link(Node,Next).".

rr02 ruleInfo(@Node, RName, RHead, RBody) :-
 recordRule(@Node),
 RName := "rs1",
 RHead := "packet(Next,SrcAdd,DstAdd,Data)",
 RBody := "packet(Node,SrcAdd,DstAdd,Data);flowEntry(Node,DstAdd,Next);link(Node,Next).".

rr03 ruleInfo(@Node, RName, RHead, RBody) :-
 recordRule(@Node),
 RName := "rh2",
 RHead := "recvPacket(Node,SrcAdd,DstAdd,Data)",
 RBody := "packet(Node,SrcAdd,DstAdd,Data);DstAdd == Node.".

/* Provenance information for the slow-changing tables and input events*/
prov_r00 provStr(@Node, VID, TpStrList) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    VID := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    TpStrList1 := f_append("initPacket"),
    TpStrList2 := f_append(Node),
    TpStrList3 := f_concat(TpStrList1, TpStrList2),
    TpStrList4 := f_append(SrcAdd),
    TpStrList5 := f_concat(TpStrList3, TpStrList4),
    TpStrList6 := f_append(DstAdd),
    TpStrList7 := f_concat(TpStrList5, TpStrList6),
    TpStrList8 := f_append(Data),
    TpStrList := f_concat(TpStrList7, TpStrList8).

prov_r01 provStr(@Node, VID, TpStrList) :-
    link(@Node, Next),
    VID := f_sha1(("link"+ Node)+ Next),
    TpStrList1 := f_append("link"),
    TpStrList2 := f_append(Node),
    TpStrList3 := f_concat(TpStrList1, TpStrList2),
    TpStrList4 := f_append(Next),
    TpStrList := f_concat(TpStrList3, TpStrList4).


prov_r03 provStr(@Node, VID, TpStrList) :-
    flowEntry(@Node, DstEntry, Next),
    VID := f_sha1((("flowEntry"+ Node)+ DstEntry)+ Next),
    TpStrList1 := f_append("flowEntry"),
    TpStrList2 := f_append(Node),
    TpStrList3 := f_concat(TpStrList1, TpStrList2),
    TpStrList4 := f_append(DstEntry),
    TpStrList5 := f_concat(TpStrList3, TpStrList4),
    TpStrList6 := f_append(Next),
    TpStrList := f_concat(TpStrList5, TpStrList6).


/* Packet forwarding*/
/* Execution of rs1 with provenance enabled*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    packetProv(@Node, SrcAdd, DstAdd, Data, Tag),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID3 := f_sha1((("flowEntry"+ Node)+ DstAdd)+ Next),
    List := f_append(PID3),
    PID4 := f_sha1(("link"+ Node)+ Next),
    List4 := f_append(PID4),
    List := f_concat(List, List4),
    RLOC := Node,
    R := "rs1",
    RID := f_sha1((R+ RLOC)+ List).

prov_rs1_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rs1_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

/* Update provLink, which associates the current RID with the RID of the previous rule.*/
prov_rs1_4 provLink(@RLOC, RID, CurCount, Preloc, PreRID, PreCount, PIDequi) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    PreCount := f_first(Tag),
    CurCount := PreCount + 1,
    TempTag1 := f_removeFirst(Tag),
    Preloc := f_first(TempTag1),
    TempTag2 := f_removeFirst(TempTag1),
    PreRID := f_first(TempTag2),
    TempTag3 := f_removeFirst(TempTag2),
    PIDequi := f_first(TempTag3).

prov_rs1_5 packetProv(@Next, SrcAdd, DstAdd, Data, NewTag) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    Count := f_first(Tag),
    NewCount := Count + 1,
    NewCountlist := f_append(NewCount),
    TempNewTag1 := f_removeFirst(Tag),
    TempNewTag2 := f_removeFirst(TempNewTag1),
    TempNewTag3 := f_removeFirst(TempNewTag2),
    RIDlist := f_append(RID),
    Loclist := f_append(RLOC),
    TempNewTag4 := f_concat(RIDlist, TempNewTag3),
    TempNewTag5 := f_concat(Loclist, TempNewTag4),
    NewTag := f_concat(NewCountlist, TempNewTag5).

/* Execution for original rs1*/
rs1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
        packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 flowEntry(@Node, DstAdd, Next),
 link(@Node, Next).

/* Packet initialization*/
/* prov_ri1 to prov_ri3 hashes on equivalence-class identifier,*/
/* and check whether the hash value "PIDequi" exists in the hash set "equiHashTable"*/
prov_ri1 initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    programID(@Node, DstAdd, ProgID),
    PIDequi := f_sha1("" + Node + DstAdd).

prov_ri2 initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, a_COUNT<*>) :-
    initPacketHash(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID),
    equiHashTable(@Node, DstAdd, PIDequi).

prov_ri3 equiHashTable(@Node, DstAdd, PIDequi) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    PIDcount == 0.

/* Execution of original rh1*/
rh1 packet(@Next, SrcAdd, DstAdd, Data, PIDHash) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PIDev := f_sha1("initPacket" + Node + SrcAdd + DstAdd + Data),
    PIDcount != 0,
    PIDequiHash := f_append(PIDequi),
    PIDevHash := f_append(PIDev),
    ProgIDlist := f_append(ProgID),
    PIDHash1 := f_concat(PIDequiHash, PIDevHash),
    PIDHash := f_concat(PIDHash1, ProgIDlist).

/* Execution of rh1 with provenance enabled*/
/* Tag = (count, preNode, preRID, equiKey, eventHash, progID)*/
prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    initPacketCount(@Node, SrcAdd, DstAdd, Data, PIDequi, ProgID, PIDcount),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PIDcount == 0,
    PID1 := f_sha1("flowEntry"+Node+DstAdd+Next),
    List1 := f_append(PID1),
    PID2 := f_sha1(("link"+ Node)+ Next),
    List2 := f_append(PID2),
    List := f_concat(List1, List2),
    RLOC := Node,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List),
    Equilist := f_append(PIDequi),
    PIDev := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
           Evlist := f_append(PIDev),
    ProgIDlist := f_append(ProgID),
    Count := 0,
    Countlist := f_append(Count),
    InitProvNode := Node,
    InitProvNodelist := f_append(InitProvNode),
    InitProvHash := f_sha1("NULL"),
    InitProvHashlist := f_append(InitProvHash),
    TempTag1 := f_concat(Countlist, InitProvNodelist),
    TempTag2 := f_concat(TempTag1, InitProvHashlist),
    TempTag3 := f_concat(TempTag2, Equilist),
           TempTag4 := f_concat(TempTag3, Evlist),
    Tag := f_concat(TempTag4, ProgIDlist).

/* Packet receipt*/
/* Receive a packet with provenance maintenance enabled*/
prov_rh2_1 erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag) :-
    packetProv(@Node, SrcAdd, DstAdd, Data, Tag),
    DstAdd == Node,
    List := f_empty(),
    RLOC := Node,
    R := "rh2",
    RID := f_sha1((R+ RLOC)+ List).

prov_rh2_2 epacketCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    ruleExec(@RLOC, RID, R, List).

prov_rh2_3 ruleExec(@RLOC, RID, R, List) :-
    epacketCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_rh2_4 provLink(@RLOC, RID, CurCount, Preloc, PreRID, PreCount, PIDequi) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    PreCount := f_first(Tag),
    CurCount := PreCount + 1,
    TempTag1 := f_removeFirst(Tag),
    Preloc := f_first(TempTag1),
    TempTag2 := f_removeFirst(TempTag1),
    PreRID := f_first(TempTag2),
    TempTag3 := f_removeFirst(TempTag2),
    PIDequi := f_first(TempTag3).

prov_rh2_5 recvPacketProv(@Node, SrcAdd, DstAdd, Data, NewTag) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, Tag),
    Count := f_first(Tag),
    NewCount := Count + 1,
    NewCountlist := f_append(NewCount),
    TempNewTag1 := f_removeFirst(Tag),
    TempNewTag2 := f_removeFirst(TempNewTag1),
    TempNewTag3 := f_removeFirst(TempNewTag2),
    RIDlist := f_append(RID),
    Loclist := f_append(RLOC),
    TempNewTag4 := f_concat(RIDlist, TempNewTag3),
    TempNewTag5 := f_concat(Loclist, TempNewTag4),
    NewTag := f_concat(NewCountlist, TempNewTag5).


/* Handle provenance information for the output recv tuple*/
/* Create a tuple in provHashTable. The tuple connects the output tuple to a provenance tree*/
ro1 provHashTable(@Node, PIDequi, ProgID, Loc, RID, Count) :-
    recvPacketProv(@Node, SrcAdd, DstAdd, Data, Tag),
    Count := f_first(Tag),
    RemainTag1 := f_removeFirst(Tag),
    Loc := f_first(RemainTag1),
    RemainTag2 := f_removeFirst(RemainTag1),
    RID := f_first(RemainTag2),
    RemainTag3 := f_removeFirst(RemainTag2),
    PIDequi := f_first(RemainTag3),
    RemainTag4 := f_removeFirst(RemainTag3),
    PIDev := f_first(RemainTag4),
    RemainTag5 := f_removeFirst(RemainTag4),
    ProgID := f_first(RemainTag5).

ro2 provRef(@Node, PID, PIDequi, PIDev) :-
    recvPacketProv(@Node, SrcAdd, DstAdd, Data, Tag),
           PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data),
    Count := f_first(Tag),
    RemainTag1 := f_removeFirst(Tag),
    Loc := f_first(RemainTag1),
    RemainTag2 := f_removeFirst(RemainTag1),
    RID := f_first(RemainTag2),
    RemainTag3 := f_removeFirst(RemainTag2),
    PIDequi := f_first(RemainTag3),
    RemainTag4 := f_removeFirst(RemainTag3),
    PIDev := f_first(RemainTag4).


ro3 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
 recvPacketProv(@Node, SrcAdd, DstAdd, Data, Tag).


/* Execution of original rh2*/
rh2 recvPacketNP(@Node, SrcAdd, DstAdd, Data, PIDHash) :-
 packet(@Node, SrcAdd, DstAdd, Data, PIDHash),
 DstAdd == Node.

rho3 provRef(@Node, PID, PIDequi, PIDev) :-
 recvPacketNP(@Node, SrcAdd, DstAdd, Data, PIDHash),
        PID := f_sha1("recvPacket" + Node + SrcAdd + DstAdd + Data),
 PIDequi := f_first(PIDHash),
 PIDHash1 := f_removeFirst(PIDHash),
 PIDev := f_first(PIDHash1).

rho4 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
 recvPacketNP(@Node, SrcAdd, DstAdd, Data, PIDHash).

/* Provenance querying */
/* Query initialization*/
rqini rQuery(@Loc,NQID,RID,Count,PIDev,PIDequi,Ret) :-
 provQuery(@X,QID,PID,Ret),
 provRef(@X,PID,PIDequi,PIDev),
 provHashTable(@X,PIDequi,ProgID,Loc,RID,Count),
 NQID := f_sha1(""+QID+RID).

/* Query the event tuple*/
rqev tQuery(@X,NNQID,PIDev,TupleRet) :-
 rQuery(@X,NQID,RID,Count,PIDev,PIDequi,Ret),
 provLink(@X,RID,CurCount,Preloc,PreRID,PreCount,PIDequi),
 Preloc == X,
 PreRID == f_sha1("NULL"),
 TupleRet := X,
 NNQID := f_sha1(""+NQID+PIDev).

/* Recursively query the RIDs*/
rqrec rQuery(@Preloc,NNQID,PreRID,PreCount,PIDev,PIDequi,X) :-
 rQuery(@X,NQID,RID,Count,PIDev,PIDequi,Ret),
 provLink(@X,RID,CurCount,Preloc,PreRID,PreCount),
 PreRID != f_sha1("NULL"),
 NNQID := f_sha1(""+NQID+PreRID).

/* Query the slow-changing tuples*/
rqsc1 slowChangeQuery(@X,NQID,List,TupleRet) :-
 rQuery(@X,NQID,RID,Count,PIDev,PIDequi,Ret),
 ruleExec(@X,RID,R,List),
 TupleRet := X.

rqsc2 tupleItr(@X,NQID,N) :-
 slowChangeQuery(@X,NQID,List,TupleRet),
 N := 1.

rqsc3 tupleItr(@X,NQID,N) :-
 tupleItr(@X,NQID,N1),
 slowChangeQuery(@X,NQID,List,TupleRet),
 N1 < f_size(List),
 N := N1 + 1.

rqsc4 tQuery(@X,NNQID,VID,TupleRet) :-
 tupleItr(@X,NQID,N),
 slowChangeQuery(@X,NQID,List,TupleRet),
 VID := f_item(List,N),
 NNQID := f_sha1(""+NQID+VID).

rqsc5 tReturn(@TupleRet,NNQID,VID,Prov) :-
 tQuery(@X,NNQID,VID,TupleRet),
 provStr(@X,VID,TpStrList),
 Prov := f_pEDBTP(TpStrList,X).

/* Collect provenance results*/
rqrt1 provResult(@X,NQID,RID,Buff,R,BodyNum,Ret):-
 rQuery(@X,NQID,RID,Count,PIDev,PIDequi,Ret),
 ruleExec(@X,RID,R,List),
 BodyNum := f_size(List)+1,
 Buff := f_empty().

rqrt2 provResult(@X,NQID,RID,NewBuff,R,BodyNum,Ret) :-
 tReturn(@X,NNQID,VID,Prov),
 provResult(@X,NQID,RID,Buff,R,BodyNum,Ret),
 NNQID == f_sha1(""+NQID+VID),
 ProvBuff := f_append(Prov),
 NewBuff := f_concat(Buff,ProvBuff).

rqrt3 provResult(@X,NQID,RID,NewBuff,R,BodyNum,Ret) :-
 rReturn(@X,NNQID,PreRID,Prov),
 provResult(@X,NQID,RID,Buff,R,BodyNum,Ret),
 NNQID == f_sha1(""+NQID+PreRID),
 ProvBuff := f_append(Prov),
 NewBuff := f_concat(Buff,ProvBuff).

rqrt4 rReturnSig(@X,NQID,RID,Buff,R,BodyNum,Ret) :-
 provResult(@X,NQID,RID,Buff,R,BodyNum,Ret),
 f_size(Buff) == BodyNum.

rqrt5 rReturn(@Ret,NQID,RID,Prov) :-
 rReturnSig(@X,NQID,RID,Buff,R,BodyNum,Ret),
 ruleInfo(@X,R,RHead,RBody),
 Prov := f_pRULEITM(Buff,X,R,RHead,RBody).
