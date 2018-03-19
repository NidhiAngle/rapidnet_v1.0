/*Materialized table*/
materialize(initPacket,infinity,infinity,keys(2,3,4:str)). /*Input packets at hosts*/
materialize(recvPacket,infinity,infinity,keys(2,3,4:str)). /*Received packets at hosts*/
materialize(link,infinity,infinity,keys(2)). /*Links between network devices*/
materialize(flowEntry,infinity,infinity,keys(2)). /*Links between routers and other devices*/

/* Provenance tables*/
materialize(ruleExec, infinity, infinity, keys(4:list)).
materialize(prov, infinity, infinity, keys(2:cid,3:cid)).

/* Provenance query tables*/
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(pQList,infinity,infinity,keys(1,2:cid)).
materialize(rQList,infinity,infinity,keys(1,2:cid)).
materialize(pResultTmp,infinity,infinity,keys(1,2:cid)).
materialize(rResultTmp,infinity,infinity,keys(1,2:cid)).


/* Edb provenance rules*/
r00 prov(@Node, VID, RID, Node) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    VID := f_sha1("initPacket" + Node + SrcAdd + DstAdd + Data),
    RID := VID.

r01 prov(@Node, VID, RID, Node) :-
    link(@Node, Next),
    VID := f_sha1("link" + Node + Next),
    RID := VID.

r03 prov(@Node, VID, RID, Node) :-
    flowEntry(@Node, DstEntry, Next),
    VID := f_sha1("flowEntry" + Node + DstEntry + Next),
    RID := VID.

/*A hit in the routing table, forward the packet accordingly*/
prov_rs1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List) :-
    packet(@Node, SrcAdd, DstAdd, Data),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID2 := f_sha1(((("packet"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    PID3 := f_sha1((("flowEntry"+ Node)+ DstAdd)+ Next),
    List3 := f_append(PID3),
    List := f_concat(List, List3),
    PID4 := f_sha1(("link"+ Node)+ Next),
    List4 := f_append(PID4),
    List := f_concat(List, List4),
    RLOC := Node,
    R := "rs1",
    RID := f_sha1((R+ RLOC)+ List).

prov_rs1_2 ruleExec(@RLOC, RID, R, List) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List).

prov_rs1_3 epacket(@Next, SrcAdd, DstAdd, Data, RID, RLOC) :-
    epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List).

prov_rs1_4 packet(@Next, SrcAdd, DstAdd, Data) :-
    epacket(@Next, SrcAdd, DstAdd, Data, RID, RLOC).

prov_rs1_5 prov(@Next, VID, RID, RLOC) :-
    epacket(@Next, SrcAdd, DstAdd, Data, RID, RLOC),
    VID := f_sha1(((("packet"+ Next)+ SrcAdd)+ DstAdd)+ Data).

/*Packet initialization*/
prov_rh1_1 epacketTemp(@RLOC, Next, SrcAdd, DstAdd, Data, RID, R, List) :-
    initPacket(@Node, SrcAdd, DstAdd, Data),
    flowEntry(@Node, DstAdd, Next),
    link(@Node, Next),
    PID2 := f_sha1(((("initPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List2 := f_append(PID2),
    PID3 := f_sha1(("link"+ Node)+ Next),
    List3 := f_append(PID3),
    PID4 := f_sha1("flowEntry"+Node+DstAdd+Next),
    List4 := f_append(PID4),
    List5 := f_concat(List2, List3),
    List := f_concat(List5, List4),
    RLOC := Node,
    R := "rh1",
    RID := f_sha1((R+ RLOC)+ List).

/*Receive a packet*/
prov_rh2_1 erecvPacketTemp(@RLOC, Node, SrcAdd, DstAdd, Data, RID, R, List) :-
    packet(@Node, SrcAdd, DstAdd, Data),
    DstAdd == Node,
    PID2 := f_sha1(((("packet"+ Node)+ SrcAdd)+ DstAdd)+ Data),
    List := f_append(PID2),
    RLOC := Node,
    R := "rh2",
    RID := f_sha1((R+ RLOC)+ List).

prov_rh2_2 ruleExec(@RLOC, RID, R, List) :-
    erecvPacketTemp(@RLOC,Node, SrcAdd, DstAdd, Data, RID, R, List).

prov_rh2_3 erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC) :-
    erecvPacketTemp(@RLOC,Node, SrcAdd, DstAdd, Data, RID, R, List).

prov_rh2_4 recvPacket(@Node, SrcAdd, DstAdd, Data) :-
    erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC).

prov_rh2_5 prov(@Node, VID, RID, RLOC) :-
    erecvPacket(@Node, SrcAdd, DstAdd, Data, RID, RLOC),
    VID := f_sha1(((("recvPacket"+ Node)+ SrcAdd)+ DstAdd)+ Data).

/* Query program*/
/* EDB vertex */
edb1 pReturn(@Ret,QID,VID,Prov) :- provQuery(@X,QID,VID,Ret),
       prov(@X,VID,RID,RLoc), RID==VID, Prov:=f_pEDB(VID,X).

/* IDB vertex */
idb1 pQList(@X,QID,a_LIST<RID>) :- provQuery(@X,QID,VID,Ret),
       prov(@X,VID,RID,RLoc), RID!=VID.
idb2 pResultTmp(@X,QID,Ret,VID,Buf) :-
       provQuery(@X,QID,VID,Ret), Buf:=f_empty().

idb3 pIterate(@X,QID,N) :- provQuery(@X,QID,VID,Ret), N:=1.
idb4 pIterate(@X,QID,N) :- pIterate(@X,QID,N1),
       pQList(@X,QID,List), N1<f_size(List), N:=N1+1.

idb5 eRuleQuery(@X,NQID,RID) :- pIterate(@X,QID,N),
       pQList(@X,QID,List), N<=f_size(List),
       RID:=f_item(List,N), NQID:=f_sha1(""+QID+RID).
idb6 ruleQuery(@RLoc,NQID,RID,X) :- eRuleQuery(@X,NQID,RID),
       prov(@X,VID,RID,RLoc).

idb7 pResultTmp(@X,QID,Ret,VID,Buf) :- rReturn(@X,NQID,RID,Prov),
       pResultTmp(@X,QID,Ret,VID,Buf1), NQID==f_sha1(""+QID+RID),
       Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

idb8 ePReturn(@X,QID) :- pResultTmp(@X,QID,Ret,VID,Buf),
       pQList(@X,QID,List),
       f_size(Buf)==f_size(List), f_size(Buf)!=0.

idb9 pReturn(@Ret,QID,VID,Prov) :- ePReturn(@X,QID),
       pResultTmp(@X,QID,Ret,VID,Buf), Prov:=f_pIDB(Buf,X).

/* Rule Vertex */
rv1 rQList(@X,NQID,List) :- ruleQuery(@X,NQID,RID,Ret),
      ruleExec(@X,RID,R,List).
rv2 rResultTmp(@X,NQID,Ret,RID,Buf) :-
      ruleQuery(@X,NQID,RID,Ret), Buf:=f_empty().

rv3 rIterate(@X,NQID,N) :- ruleQuery(@X,NQID,RID,Ret), N:=1.
rv4 rIterate(@X,NQID,N) :- rIterate(@X,NQID,N1),
      rQList(@X,NQID,List), N1<f_size(List), N:=N1+1.

rv5 eProvQuery(@X,NNQID,VID) :- rIterate(@X,NQID,N),
      rQList(@X,NQID,List),
      VID:=f_item(List,N), NNQID:=f_sha1(""+NQID+VID).
rv6 provQuery(@X,NNQID,VID,X) :- eProvQuery(@X,NNQID,VID).

rv7 rResultTmp(@X,NQID,Ret,RID,Buf) :- pReturn(@X,NNQID,VID,Prov),
      rResultTmp(@X,NQID,Ret,RID,Buf1), NNQID==f_sha1(""+NQID+VID),
      Buf2:=f_append(Prov), Buf:=f_concat(Buf1,Buf2).

rv8 eRReturn(@X,NQID) :- rResultTmp(@X,NQID,Ret,RID,Buf),
      rQList(@X,NQID,List), f_size(Buf)==f_size(List).
rv9 rReturn(@Ret,NQID,RID,Prov) :- eRReturn(@X,NQID),
      rResultTmp(@X,NQID,Ret,RID,Buf),
      ruleExec(@X,RID,R,List), Prov:=f_pRULE(Buf,X,R).
