materialize(records, infinity, infinity, keys(1,2:cid,3:cid,4:str)).
materialize(tuple, infinity, infinity, keys(1,2:str)).

rq1 provQuery(@Loc, QID, VID, Ret) :-
    tuple(@Ret, Name, Loc, Src, Dst, Data),
    VID := f_sha1(Name+Loc+Src+Dst+Data),
    Time := f_now(),
    QID := f_sha1(""+VID+Time).

rq2 records(@Ret, QID, RID, Prov) :-
    rReturn(@Ret, QID, RID, Prov).
