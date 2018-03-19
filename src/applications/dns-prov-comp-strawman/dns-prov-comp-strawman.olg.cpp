materialize(url,infinity,infinity,keys(1,2:str,3,4:int32)).
materialize(address_record,infinity,infinity,keys(1,2:str,3)).
materialize(name_server,infinity,infinity,keys(1,2:str,3:str)).
materialize(result,infinity,infinity,keys(1,2:str,3,4:int32)).

/*Provenance table*/
materialize(ruleExec,infinity,infinity,keys(4:list)).
materialize(resultAux, infinity, infinity, keys(2:cid)).
/*materialize(prov,infinity,infinity,keys(1,2:cid,3:cid,4)).*/


/*Rule r1, Initialize URL tables ar root*/
prov_r1_1 eRequestTemp(@RLOC,X,URL,HOST,REQUESTID,RID,R,List,HashList) :-
     url(@X,URL,HOST,REQUESTID),
  PIDev := f_sha1("url"+X+URL+HOST+REQUESTID),
  List := f_empty(),
  HashList := f_append(PIDev),
     RLOC := X,
     R := "r1",
     RID := f_sha1((R+RLOC)+List).

prov_r1_2 eRequestCount(@RLOC, RID, R, List, a_COUNT<*>):-
  eRequestTemp(@RLOC,X,URL,HOST,REQUESTID,RID,R,List,HashList),
  ruleExec(@RLOC, RID, R, List).

prov_r1_3 ruleExec(@RLOC, RID, R, List) :-
    eRequestCount(@RLOC, RID, R, List, Rcount),
    Rcount == 0.

prov_r1_4 request(@X,URL,HOST,REQUESTID,NewHashList) :-
    eRequestTemp(@RLOC,X,URL,HOST,REQUESTID,RID,R,List,HashList),
     Hash := f_append(RID),
     NewHashList := f_concat(HashList, Hash).

/*Rule r2, traverse the DNS tree*/
prov_r2_1 eRequestTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List,HashList) :-
  request(@X,URL,HOST,REQUESTID,HashList),
    name_server(@X,DOMAIN,SERVER),
    address_record(@X,SERVER,ADDR),
    f_indexof(URL,DOMAIN)!=-1,
    URL!=DOMAIN,
    List := f_empty(),
    PID2 := f_sha1(("name_server"+X)+DOMAIN+SERVER),
    List2 := f_append(PID2),
    List := f_concat(List,List2),
    PID3 := f_sha1(("address_record"+X)+SERVER+ADDR),
    List3 := f_append(PID3),
    List := f_concat(List,List3),
    RLOC := X,
    R := "r2",
    RID := f_sha1((R+RLOC)+List).

/*Rule r3, Generate the final results*/
prov_r3_1 eResultTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List,HashList) :-
   request(@X,URL,HOST,REQUESTID,HashList),
   name_server(@X,DOMAIN,SERVER),
   address_record(@X,SERVER,ADDR),
   f_indexof(URL,DOMAIN)!=-1,
   URL==DOMAIN,
   PID1:= f_sha1(("request"+X)+URL+HOST+REQUESTID),
   List := f_append(PID1),
   PID2 := f_sha1(("name_server"+X)+DOMAIN+SERVER),
   List2 := f_append(PID2),
   List := f_concat(List,List2),
   PID3 := f_sha1(("address_record"+X)+SERVER+ADDR),
   List3 := f_append(PID3),
   List := f_concat(List,List3),
   RLOC := X,
   R :="r3",
   RID := f_sha1((R+RLOC)+List).

prov_r3_2 eResultCount(@RLOC, RID, R, List, a_COUNT<*>) :-
    eResultTemp(@RLOC, ADDR, URL, HOST, REQUESTID,RID, R, List, HashList),
    ruleExec(@RLOC, RID, R, List).

prov_r3_3 ruleExec(@RLOC, RID, R, List) :-
    eResultCount(@RLOC, RID, R, List, Count),
    Count == 0.

prov_r3_4 eResult(@ADDR,URL,HOST,REQUESTID,RID,RLOC,HashList) :-
    eResultTemp(@RLOC,ADDR,URL,HOST,REQUESTID,RID,R,List,HashList),
    Hash := f_append(RID),
    NewHashList := f_concat(HashList, Hash).

prov_r3_5 result(@ADDR,URL,HOST,REQUESTID) :-
   eResult(@ADDR,URL,HOST,REQUESTID,RID,RLOC,HashList).

prov_r3_6 resultAux(@ADDR,PID,HashList) :-
   eResult(@ADDR,URL,HOST,REQUESTID,RID,RLOC,HashList),
   PID := f_sha1("result" + ADDR + URL + HOST+REQUESTID).
