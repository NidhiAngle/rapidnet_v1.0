materialize(url,infinity,infinity,keys(1,2:str,3)).
materialize(address_record,infinity,infinity,keys(1,2:str,3)).
materialize(name_server,infinity,infinity,keys(1,2:str,3:str)).
materialize(result,infinity,infinity,keys(1,2:str,3)).

r1 request(@X,URL,HOST) :- url(@X,URL,HOST).

r2 request(@ADDR,URL,HOST) :- request(@X,URL,HOST),name_server(@X,DOMAIN,SERVER),
          address_record(@X,SERVER,ADDR),f_indexof(URL,DOMAIN)!=-1,
       URL!=DOMAIN.

r3 result(@ADDR,URL,HOST) :- request(@X,URL,HOST),name_server(@X,DOMAIN,SERVER),
    address_record(@X,SERVER,ADDR),f_indexof(URL,DOMAIN)!=-1,
 URL==DOMAIN.
