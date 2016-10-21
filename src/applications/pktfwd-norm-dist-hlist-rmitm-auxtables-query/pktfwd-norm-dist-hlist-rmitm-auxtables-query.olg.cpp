/******************************
*
* Basic optimization (removal of intermediate provenance nodes) with auxiliary tables
* for maintenance of concrete provenance information.
* 
*******************************/

/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(link,infinity,infinity,keys(2)). /*Links between routers and other devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(2:cid,5,6:cid)).
materialize(prov, infinity, infinity, keys(2:cid,3,4:cid)).

/* Tuple recording tables*/
materialize(provStr, infinity, infinity, keys(2:cid)).


/* Rule recording tables*/
materialize(recordRule, infinity, infinity, keys(1)).
materialize(ruleInfo, infinity, infinity, keys(2:str)).

/* Provenance querying tables*/
materialize(provResult, infinity, infinity, keys(2:cid)).
materialize(slowChangeQuery, infinity, infinity, keys(2:cid)).


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
 RBody := "packet(Node,SrcAdd,DstAdd,Data);DstAdd==Node.".

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


/* Forward a packet*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist) :-
    epacket(@Node, SrcAdd, DstAdd, Data, PreLoc, PreRID),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PIDpk := f_sha1("packet"+Node+SrcAdd+DstAdd+Data),
    PID3 := f_sha1((("flowEntry"+ Node)+ DstAdd)+ Next),
    List3 := f_append(PID3),
    PID4 := f_sha1(("link"+ Node)+ Next),
    List4 := f_append(PID4),
    List := f_concat(List3, List4),
    RLOC := Node,
    R := "rs1",
    RID := f_sha1((R+ RLOC)+List+PIDpk),
    PreLoclist := f_append(PreLoc),
    PreRIDlist := f_append(PreRID),
    PreInfolist := f_concat(PreLoclist, PreRIDlist).

prov_rs1_2 ruleExec(@RLOC, RID, R, List, PreLoc, PreRID) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist),
    PreLoc := f_first(PreInfolist),
    Remainlist := f_removeFirst(PreInfolist),
    PreRID := f_first(Remainlist).

prov_rs1_3 epacket(@Next, SrcAdd, DstAdd, Data, RLOC, RID) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist).

prov_rs1_4 packet(@Next, SrcAdd, DstAdd, Data) :-
    epacket(@Next, SrcAdd, DstAdd, Data, RLOC, RID).

/* Packet initialization*/
prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID1 := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List1 := f_append(PID1),
    PID2 := f_sha1("flowEntry"+Node+DstAdd+Next),
    List2 := f_append(PID2),
    PID3 := f_sha1(("link"+ Node)+ Next),
    List3 := f_append(PID3),
    List4 := f_concat(List1, List2),
    List := f_concat(List4, List3),
    PreLoc := Node,
    PreLoclist := f_append(PreLoc),
    PreRID := f_sha1("NULL"),
    PreRIDlist := f_append(PreRID),
    RLOC := Node,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List),
    PreInfolist := f_concat(PreLoclist, PreRIDlist).

/*Receive a packet*/
prov_rh2_1 erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist) :-
    epacket(@Node, SrcAdd, DstAdd, Data, PreLoc, PreRID),
    DstAdd == Node,
    List := f_empty(),
    RLOC := Node,
    R := "rh2",
    PIDpk := f_sha1("packet"+Node+SrcAdd+DstAdd+Data),
    RID := f_sha1((R+ RLOC)+ List+PIDpk),
    PreLoclist := f_append(PreLoc),
    PreRIDlist := f_append(PreRID),
    PreInfolist := f_concat(PreLoclist, PreRIDlist).

prov_rh2_2 ruleExec(@RLOC, RID, R, List, PreLoc, PreRID) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist),
    PreLoc := f_first(PreInfolist),
    Remainlist := f_removeFirst(PreInfolist),
    PreRID := f_first(Remainlist).

prov_rh2_3 erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC) :-
    erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List, PreInfolist).

prov_rh2_5 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
    erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC).

prov_rh2_4 prov(@Node, VID, RLOC, RID) :-
    erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC),
    VID := f_sha1(((("recvPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data).


/* Provenance Query*/
/* Query initialization*/
rqini rQuery(@RLoc,NQID,RID,Ret) :-
 provQuery(@X,QID,PID,Ret),
 prov(@X,PID,RLoc,RID),
 NQID := f_sha1(""+QID+RID).

/* Recursively query the RIDs*/
rqrec rQuery(@Preloc,NNQID,PreRID,X) :-
 rQuery(@X,NQID,RID,Ret),
 ruleExec(@X,RID,R,List,Preloc,PreRID),
 PreRID != f_sha1("NULL"),
 NNQID := f_sha1(""+NQID+PreRID).

/* Query the slow-changing tuples*/
rqsc1 slowChangeQuery(@X,NQID,List,TupleRet) :-
 rQuery(@X,NQID,RID,Ret),
 ruleExec(@X,RID,R,List,PreLoc,PreRID),
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
 rQuery(@X,NQID,RID,Ret),
 ruleExec(@X,RID,R,List,PreLoc,PreRID),
 PreRID == f_sha1("NULL"),
 BodyNum := f_size(List),
 Buff := f_empty().

rqrt2 provResult(@X,NQID,RID,Buff,R,BodyNum,Ret):-
 rQuery(@X,NQID,RID,Ret),
 ruleExec(@X,RID,R,List,PreLoc,PreRID),
 PreRID != f_sha1("NULL"),
 BodyNum := f_size(List)+1,
 Buff := f_empty().

rqrt3 provResult(@X,NQID,RID,NewBuff,R,BodyNum,Ret) :-
 tReturn(@X,NNQID,VID,Prov),
 provResult(@X,NQID,RID,Buff,R,BodyNum,Ret),
 NNQID == f_sha1(""+NQID+VID),
 ProvBuff := f_append(Prov),
 NewBuff := f_concat(Buff,ProvBuff).

rqrt4 provResult(@X,NQID,RID,NewBuff,R,BodyNum,Ret) :-
 rReturn(@X,NNQID,PreRID,Prov),
 provResult(@X,NQID,RID,Buff,R,BodyNum,Ret),
 NNQID == f_sha1(""+NQID+PreRID),
 ProvBuff := f_append(Prov),
 NewBuff := f_concat(Buff,ProvBuff).

rqrt5 rReturnSig(@X,NQID,RID,Buff,R,BodyNum,Ret) :-
 provResult(@X,NQID,RID,Buff,R,BodyNum,Ret),
 f_size(Buff) == BodyNum.

rqrt6 rReturn(@Ret,NQID,RID,Prov) :-
 rReturnSig(@X,NQID,RID,Buff,R,BodyNum,Ret),
 ruleInfo(@X,R,RHead,RBody),
 Prov := f_pRULEITM(Buff,X,R,RHead,RBody).
